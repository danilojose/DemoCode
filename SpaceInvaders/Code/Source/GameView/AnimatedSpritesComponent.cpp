#include <GameView\AnimatedSpritesComponent.h>
#include <System\Assert.h>

using namespace Graphics;
extern RenderSystem *g_pRenderSystem;
/// <summary>
/// Initializes a new instance of the <see cref="AnimatedSpritesComponent"/> class.
/// </summary>
/// <param name="ownerId">The owner identifier.</param>
/// <param name="posX">The position x.</param>
/// <param name="posY">The position y.</param>
/// <param name="sprites">The sprites.</param>
/// <param name="idleSprite">The idle sprite.</param>
AnimatedSpritesComponent::AnimatedSpritesComponent(uint32_t ownerId,uint32_t posX, uint32_t posY,RenderSystem* renderSystem,std::vector<std::string> &sprites, uint32_t idleSprite) :IGraphicsComponent(ownerId,posX,posY,renderSystem)
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
void AnimatedSpritesComponent::Render()
{
	g_pRenderSystem->RenderImage(m_SpriteList[m_CurrentSprite], m_PosX, m_PosY);
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
void AnimatedSpritesComponent::UpdatePosition(uint32_t x, uint32_t y) 
{
	if (m_SpriteList.size() > 1)
	{
		if (x < m_PosX)
		{
			if (m_CurrentSprite>0)
			{
				--m_CurrentSprite;
			}
		}
		else
		{
			if (x > m_PosX)
			{
				if (m_CurrentSprite < (m_SpriteList.size()-1))
				{
					++m_CurrentSprite;
				}
			}
		}
	}
	m_PosX = x;
	m_PosY = y;
}
/// <summary>
/// Finalizes an instance of the <see cref="AnimatedSpritesComponent"/> class.
/// </summary>
AnimatedSpritesComponent::~AnimatedSpritesComponent()
{

	m_SpriteList.clear();

	IGraphicsComponent::~IGraphicsComponent();

}
