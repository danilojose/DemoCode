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
	uint32_t m_LastAnimationWasPainted;

public:

	static const std::string COMPONENT_NAME;

	/// <summary>
	/// Initializes a new instance of the <see cref="AnimatedSpritesComponent" /> class.
	/// </summary>
	StaticAnimatedSpritesComponent() :IGraphicsComponent(StaticAnimatedSpritesComponent::COMPONENT_NAME),
		m_IdleSprite(-1), m_CurrentSprite(-1), m_LastAnimationWasPainted(std::numeric_limits<uint32_t>::max())
	{
	}
	/// <summary>
	/// Initializes a new instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
	/// </summary>
	/// <param name="owner">The owner entity.</param>
	/// <param name="sprites">The sprites.</param>
	/// <param name="idleSprite">The idle sprite.</param>
	StaticAnimatedSpritesComponent(Entity *owner,RenderSystem* renderSystem,std::vector<std::string> sprites, uint32_t idleSprite);
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

public:
	virtual void OnRender() override;

	/// <summary>
	/// Builds the specified Component using the specified descriptor.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void Build(const IniValuesMap &descriptor) override;
};

}