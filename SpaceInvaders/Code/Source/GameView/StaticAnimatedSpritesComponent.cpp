#include <GameView\StaticAnimatedSpritesComponent.h>
#include <System\Assert.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <GameView\ImageResource.h>
using namespace GameSystem;
using namespace Graphics;
const uint32_t ANIMATION_RATE = 50;

extern RenderSystem* g_pRenderSystem;
/// <summary>
/// Initializes a new instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
/// </summary>
/// <param name="ownerId">The owner identifier.</param>
/// <param name="posX">The position x.</param>
/// <param name="posY">The position y.</param>
/// <param name="sprites">The sprites.</param>
/// <param name="idleSprite">The idle sprite.</param>
StaticAnimatedSpritesComponent::StaticAnimatedSpritesComponent(uint32_t ownerId, uint32_t posX, uint32_t posY, RenderSystem* renderSystem,std::vector<std::string> sprites, uint32_t idleSprite) :IGraphicsComponent(ownerId, posX, posY,renderSystem)
{

	for (const auto &sprite : sprites)
	{
		std::shared_ptr<ImageResource> image=std::shared_ptr<ImageResource>(GCC_NEW ImageResource(sprite));
		m_SpriteList.push_back(image);
	}

	ASSERT_DESCRIPTION((idleSprite < m_SpriteList.size()) && (idleSprite >= 0), "The idle sprite must be in the scope of the amount of sprites created");

	m_IdleSprite = idleSprite;
	m_CurrentSprite = m_IdleSprite;

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
void StaticAnimatedSpritesComponent::Render()
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
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_AnimationFinished(m_ActorId)));
		}
	}
	g_pRenderSystem->RenderImage(m_SpriteList[m_CurrentSprite], m_PosX, m_PosY);
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
void StaticAnimatedSpritesComponent::UpdatePosition(uint32_t x, uint32_t y)
{
	IGraphicsComponent::UpdatePosition(x, y);
}

