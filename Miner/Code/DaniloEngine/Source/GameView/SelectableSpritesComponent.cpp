#include <GameView\SelectableSpritesComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>
#include <System\EntityEvents.h>
#include <System\EventManager.h>
#include <GameView\ImageResource.h>

using namespace GameSystem;
using namespace Graphics;
const uint32_t ANIMATION_RATE = 50;
extern RenderSystem* g_pRenderSystem;

const std::string Graphics::SelectableSpritesComponent::COMPONENT_NAME = "SelectableSpritesComponent";

/// <summary>
/// Initializes a new instance of the <see cref="SelectableSpritesComponent"/> class.
/// </summary>
/// <param name="owner">The owner entity.</param>
/// <param name="sprites">The sprites.</param>
/// <param name="idleSprite">The idle sprite.</param>
SelectableSpritesComponent::SelectableSpritesComponent(Entity *owner, RenderSystem* renderSystem, std::vector<std::string> sprites, uint32_t idleSprite,uint32_t selectedSprite)
:IGraphicsComponent(SelectableSpritesComponent::COMPONENT_NAME, owner, renderSystem),
m_IdleSprite(idleSprite), m_SelectedSprite(selectedSprite), m_CurrentSprite(idleSprite)
{

	for (const auto &sprite : sprites)
	{
		std::shared_ptr<ImageResource> image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(sprite));
		m_SpriteList.push_back(image);
	}
	ASSERT_DESCRIPTION((idleSprite < m_SpriteList.size()) && (idleSprite >= 0), "The idle sprite must be in the scope of the amount of sprites created");
}
/// <summary>
/// Finalizes an instance of the <see cref="SelectableSpritesComponent"/> class.
/// </summary>
SelectableSpritesComponent::~SelectableSpritesComponent()
{

	m_SpriteList.clear();

	IGraphicsComponent::~IGraphicsComponent();

}
/// <summary>
/// Renders this instance. This function based on the time decides what should be the sprite that requires to be rendered. When reaching the end it triggers an event saying the animation was finished
/// </summary>
void SelectableSpritesComponent::OnRender()
{
	g_pRenderSystem->RenderImage(m_SpriteList[m_CurrentSprite], m_Entity->GetPosX(), m_Entity->GetPosY());
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void SelectableSpritesComponent::Build(JSONNode *descriptor)
{


	ASSERT_DESCRIPTION(descriptor, "Not a valid descriptor");
	uint32_t numberOfSprites = descriptor->GetUInteger("NumberOfSprites");
	ASSERT_DESCRIPTION(numberOfSprites>0, "Number of Sprites must be greater than 0");

	uint32_t idleSprite = descriptor->GetUInteger("IdleSprite");
	ASSERT_DESCRIPTION(idleSprite<numberOfSprites, "idle sprite must be less than the number of Sprites");

	uint32_t selectedSprite = descriptor->GetUInteger("OnSelectedSprite");
	ASSERT_DESCRIPTION(selectedSprite<numberOfSprites, "idle sprite must be less than the number of Sprites");

	std::vector<JSONNode *> spritesDescriptorVector = descriptor->GetChild("Sprites")->GetArrayValues();

	for (const auto & spriteDescriptor : spritesDescriptorVector)
	{
		const std::string spritesFile = spriteDescriptor->GetString("Sprite");
		ASSERT_DESCRIPTION(!spritesFile.empty(), "Empty sprite");
		std::shared_ptr<ImageResource> image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(spritesFile));
		m_SpriteList.push_back(image);
	}
	m_IdleSprite = idleSprite;
	m_SelectedSprite = selectedSprite;
	m_CurrentSprite = m_IdleSprite;

}


/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> SelectableSpritesComponent::Clone()
{
	std::shared_ptr<SelectableSpritesComponent> cloned = std::shared_ptr<SelectableSpritesComponent>(GCC_NEW SelectableSpritesComponent());

	cloned->m_CurrentSprite = this->m_CurrentSprite;
	cloned->m_IdleSprite = this->m_IdleSprite;
	cloned->m_SpriteList = this->m_SpriteList;
	cloned->m_SelectedSprite = this->m_SelectedSprite;

	return cloned;
}


/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void SelectableSpritesComponent::OnEntityEvent(IEventData const & event)
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_OnEntitySelected::sk_EventType)
	{
		EvtData_OnEntitySelected const & ed = static_cast< const EvtData_OnEntitySelected & >(event);
		if (ed.m_EntityId == this->GetEntity()->GetID())
		{
			m_CurrentSprite = m_SelectedSprite;
		}
	}
	else
	{
		if (eventType == EvtData_OnEntityUnSelected::sk_EventType)
		{
			EvtData_OnEntityUnSelected const & ed = static_cast< const EvtData_OnEntityUnSelected & >(event);
			if (ed.m_EntityId == this->GetEntity()->GetID())
			{
				m_CurrentSprite = m_IdleSprite;
			}
		}

	}

}

