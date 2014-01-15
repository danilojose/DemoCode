#include <GameView\AnimatedSpritesComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>

using namespace Graphics;

const std::string Graphics::AnimatedSpritesComponent::COMPONENT_NAME = "AnimatedSpritesComponent";
extern RenderSystem *g_pRenderSystem;


/// <summary>
/// Initializes a new instance of the <see cref="AnimatedSpritesComponent"/> class.
/// </summary>
/// <param name="owner">The owner Entity.</param>
/// <param name="sprites">The sprites.</param>
/// <param name="idleSprite">The idle sprite.</param>
AnimatedSpritesComponent::AnimatedSpritesComponent(Entity* owner,RenderSystem* renderSystem,std::vector<std::string> &sprites, uint32_t idleSprite)
						:IGraphicsComponent(AnimatedSpritesComponent::COMPONENT_NAME,owner,renderSystem)
{
	
	for (const auto &sprite : sprites)
	{
		std::shared_ptr<ImageResource> image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(sprite));
		m_SpriteList.push_back(image);
	}

	ASSERT_DESCRIPTION((idleSprite < m_SpriteList.size()) && (idleSprite >= 0), "The idle sprite must be in the scope of the amount of sprites created");

	m_IdleSprite = idleSprite;
	m_CurrentSprite = m_IdleSprite;

}
/// <summary>
/// Updates the position. In this kind of component when the position gets updated the current sprite to be rendered gets updated too
/// </summary>
void AnimatedSpritesComponent::OnRender()
{
	g_pRenderSystem->RenderImage(m_SpriteList[m_CurrentSprite], m_Entity->GetPosX(),m_Entity->GetPosY());
}

///// <summary>
///// Updates the position.
///// </summary>
///// <param name="x">The x.</param>
///// <param name="y">The y.</param>
//void AnimatedSpritesComponent::UpdatePosition(uint32_t x, uint32_t y) 
//{
//	if (m_SpriteList.size() > 1)
//	{
//		if (x < m_PosX)
//		{
//			if (m_CurrentSprite>0)
//			{
//				--m_CurrentSprite;
//			}
//		}
//		else
//		{
//			if (x > m_PosX)
//			{
//				if (m_CurrentSprite < (m_SpriteList.size()-1))
//				{
//					++m_CurrentSprite;
//				}
//			}
//		}
//	}
//	m_PosX = x;
//	m_PosY = y;
//}
/// <summary>
/// Finalizes an instance of the <see cref="AnimatedSpritesComponent"/> class.
/// </summary>
AnimatedSpritesComponent::~AnimatedSpritesComponent()
{

	m_SpriteList.clear();

	IGraphicsComponent::~IGraphicsComponent();

}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void AnimatedSpritesComponent::Build(JSONNode *descriptor)
{

	ASSERT_DESCRIPTION(descriptor, "Not a valid descriptor");
	uint32_t numberOfSprites = descriptor->GetUInteger("NumberOfSprites");
	ASSERT_DESCRIPTION(numberOfSprites>0, "Number of Sprites must be greater than 0");

	uint32_t idleSprite = descriptor->GetUInteger("IdleSprite");
	ASSERT_DESCRIPTION(idleSprite<numberOfSprites, "idle sprite must be less than the number of Sprites");

	std::vector<JSONNode *> spritesDescriptorVector = descriptor->GetChild("Sprites")->GetArrayValues();

	for (const auto & spriteDescriptor:spritesDescriptorVector)
	{
		const std::string spritesFile = spriteDescriptor->GetString("Sprite");
		ASSERT_DESCRIPTION(!spritesFile.empty(), "Empty sprite");
		std::shared_ptr<ImageResource> image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(spritesFile));
		m_SpriteList.push_back(image);
	}
	m_IdleSprite = idleSprite;
	m_CurrentSprite = m_IdleSprite;
}


/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> AnimatedSpritesComponent::Clone()
{
	std::shared_ptr<AnimatedSpritesComponent> cloned = std::shared_ptr<AnimatedSpritesComponent>(GCC_NEW AnimatedSpritesComponent());
			
	cloned->m_CurrentSprite = this->m_CurrentSprite;
	cloned->m_IdleSprite = this->m_IdleSprite;
	cloned->m_SpriteList = this->m_SpriteList;

	return cloned;
}
/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void AnimatedSpritesComponent::OnEntityEvent(IEventData const & event)
{

}