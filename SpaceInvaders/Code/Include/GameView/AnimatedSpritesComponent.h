#pragma once 
#include <System\StdLibraries.h>
#include <GameView\IGraphicsComponent.h>
#include <GameView\ImageResource.h>


namespace Graphics
{

/// <summary>
/// Graphics component in charge of delivering the Sprites that compose the animation of a given entity.
/// </summary>
class AnimatedSpritesComponent:public IGraphicsComponent
{
protected:
	std::vector<std::shared_ptr<ImageResource>> m_SpriteList;
	uint32_t m_IdleSprite;
	uint32_t m_CurrentSprite;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="AnimatedSpritesComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	/// <param name="sprites">The sprites.</param>
	/// <param name="idleSprite">The idle sprite.</param>
	explicit AnimatedSpritesComponent(uint32_t ownerId, uint32_t posX, uint32_t posY,RenderSystem* renderSystem,std::vector<std::string> &sprites, uint32_t idleSprite);
	/// <summary>
	/// Finalizes an instance of the <see cref="AnimatedSpritesComponent"/> class.
	/// </summary>
	virtual ~AnimatedSpritesComponent();

private:
	/// <summary>
	/// Prevents a default instance of the <see cref="AnimatedSpritesComponent"/> class from being created.
	/// </summary>
	/// <param name="other">The other.</param>
	AnimatedSpritesComponent(const AnimatedSpritesComponent &other);
	/// <summary>
	/// Prevents the copy constructor from being created
	/// </summary>
	/// <param name="other">The other.</param>
	/// <returns></returns>
	AnimatedSpritesComponent operator=(const AnimatedSpritesComponent &other);
	/// <summary>
	/// Updates the position. In this kind of component when the position gets updated the current sprite to be rendered gets updated too
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	virtual void UpdatePosition(uint32_t x, uint32_t y) override;

public:

	/// <summary>
	/// This function is in charge of rendering the current sprite... 
	/// </summary>
	virtual void Render();

};

}