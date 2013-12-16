#pragma once 
#include <System\StdLibraries.h>
#include <GameView\IGraphicsComponent.h>
#include <GameView\ImageResource.h>

namespace Graphics
{

/// <summary>
/// This class implements a different Graphics component where there are several sprites stored as animations and we just 
/// </summary>
class StaticAnimatedSpritesComponent :public IGraphicsComponent
{
protected:
	std::vector<std::shared_ptr<ImageResource>> m_SpriteList;
	uint32_t m_IdleSprite;
	uint32_t m_CurrentSprite;
	bool m_Dirty;
	uint32_t m_LastAnimationWasPainted;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	/// <param name="sprites">The sprites.</param>
	/// <param name="idleSprite">The idle sprite.</param>
	explicit StaticAnimatedSpritesComponent(uint32_t ownerId, uint32_t posX, uint32_t posY, std::vector<std::string> sprites, uint32_t idleSprite);
	/// <summary>
	/// Finalizes an instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
	/// </summary>
	virtual ~StaticAnimatedSpritesComponent();

private:
	/// <summary>
	/// Prevents a default instance of the <see cref="StaticAnimatedSpritesComponent"/> class from being created.
	/// </summary>
	/// <param name="other">The other.</param>
	StaticAnimatedSpritesComponent(const StaticAnimatedSpritesComponent &other);
	/// <summary>
	/// Operator=s the specified other.
	/// </summary>
	/// <param name="other">The other.</param>
	/// <returns></returns>
	StaticAnimatedSpritesComponent operator=(const StaticAnimatedSpritesComponent &other);
	/// <summary>
	/// Updates the position.
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	virtual void UpdatePosition(uint32_t x, uint32_t y) override;

public:
	virtual void Render();

};

}