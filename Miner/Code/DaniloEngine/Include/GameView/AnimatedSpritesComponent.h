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
	std::vector<std::shared_ptr<ImageResource>>			m_SpriteList;
	uint32_t											m_IdleSprite;
	uint32_t											m_CurrentSprite;

public:

	static const std::string COMPONENT_NAME;
	/// <summary>
	/// Initializes a new instance of the <see cref="AnimatedSpritesComponent" /> class.
	/// </summary>
	AnimatedSpritesComponent::AnimatedSpritesComponent() :IGraphicsComponent(AnimatedSpritesComponent::COMPONENT_NAME),
		m_IdleSprite(-1), m_CurrentSprite(-1)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="AnimatedSpritesComponent"/> class.
	/// </summary>
	/// <param name="owner">The owner entity.</param>
	/// <param name="sprites">The sprites.</param>
	/// <param name="idleSprite">The idle sprite.</param>
	AnimatedSpritesComponent(Entity* ownerId,RenderSystem* renderSystem,std::vector<std::string> &sprites, uint32_t idleSprite);
	/// <summary>
	/// Finalizes an instance of the <see cref="AnimatedSpritesComponent"/> class.
	/// </summary>
	virtual ~AnimatedSpritesComponent();


	/// <summary>
	/// This function is in charge of rendering the current sprite... 
	/// </summary>
	virtual void OnRender() override;

	/// <summary>
	/// Builds the specified Component using the specified descriptor.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void Build(JSONNode *descriptor) override;
	/// <summary>
	/// Clones the current Component
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual std::shared_ptr<IComponent> Clone(Entity *entity) override;
	/// <summary>
	/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void OnEntityEvent(IEventData const & event)  override;
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



};

}