#include <GameView\StaticAnimatedSpritesComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <GameView\ImageResource.h>

using namespace GameSystem;
using namespace Graphics;
const uint32_t ANIMATION_RATE = 50;
extern RenderSystem* g_pRenderSystem;

const std::string Graphics::StaticAnimatedSpritesComponent::COMPONENT_NAME = "StaticAnimatedSpritesComponent";

/// <summary>
/// Initializes a new instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
/// </summary>
/// <param name="owner">The owner entity.</param>
/// <param name="sprites">The sprites.</param>
/// <param name="idleSprite">The idle sprite.</param>
StaticAnimatedSpritesComponent::StaticAnimatedSpritesComponent(Entity *owner, RenderSystem* renderSystem,std::vector<std::string> sprites, uint32_t idleSprite)
									:IGraphicsComponent(StaticAnimatedSpritesComponent::COMPONENT_NAME,owner,renderSystem),
									m_IdleSprite(idleSprite), m_CurrentSprite(idleSprite)
{

	for (const auto &sprite : sprites)
	{
		std::shared_ptr<ImageResource> image=std::shared_ptr<ImageResource>(GCC_NEW ImageResource(sprite));
		m_SpriteList.push_back(image);
	}
	ASSERT_DESCRIPTION((idleSprite < m_SpriteList.size()) && (idleSprite >= 0), "The idle sprite must be in the scope of the amount of sprites created");
	m_LastAnimationWasPainted = SDL_GetTicks();
}
/// <summary>
/// Finalizes an instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
/// </summary>
StaticAnimatedSpritesComponent::~StaticAnimatedSpritesComponent()
{

	m_SpriteList.clear();

	IGraphicsComponent::~IGraphicsComponent();

}
/// <summary>
/// Renders this instance. This function based on the time decides what should be the sprite that requires to be rendered. When reaching the end it triggers an event saying the animation was finished
/// </summary>
void StaticAnimatedSpritesComponent::OnRender()
{
	uint32_t deltaTime = SDL_GetTicks() - m_LastAnimationWasPainted;
	if (deltaTime>ANIMATION_RATE)
	{
		if (m_CurrentSprite < m_SpriteList.size() - 1)
		{
			++m_CurrentSprite;
		}
		else
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_AnimationFinished(m_Entity->GetID())));
		}
	}
	g_pRenderSystem->RenderImage(m_SpriteList[m_CurrentSprite], m_Entity->GetPosX(), m_Entity->GetPosY());
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void StaticAnimatedSpritesComponent::Build(const IniValuesMap &descriptor)
{

	ASSERT_DESCRIPTION(descriptor.find("NumberOfSprites") != descriptor.end(), "Number of Sprites not found");
	uint32_t numberOfSprites = std::strtoul(descriptor.at("NumberOfSprites").c_str(), NULL, 0);
	ASSERT_DESCRIPTION(numberOfSprites>0, "Number of Sprites must be greater than 0");

	ASSERT_DESCRIPTION(descriptor.find("IdleSprite") != descriptor.end(), "Idle Sprite not found");
	uint32_t idleSprite = std::strtoul(descriptor.at("IdleSprite").c_str(), NULL, 0);
	ASSERT_DESCRIPTION(idleSprite<numberOfSprites, "idle sprite must be less than the number of Sprites");


	for (uint32_t i = 0; i < numberOfSprites; ++i)
	{
		std::stringstream sprite;
		sprite << "Sprite[" << i << "]";
		ASSERT_DESCRIPTION(descriptor.find(sprite.str()) != descriptor.end(), "Sprite not found for player");
		ASSERT_DESCRIPTION(!descriptor.at(sprite.str()).empty(), "Empty sprite");
		std::shared_ptr<ImageResource> image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(descriptor.at(sprite.str())));
		m_SpriteList.push_back(image);
	}
	m_IdleSprite = idleSprite;
	m_CurrentSprite = m_IdleSprite;
}