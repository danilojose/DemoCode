#pragma once 
#include <System\StdLibraries.h>
#include <GameView\IGraphicsComponent.h>
#include <GameView\ImageResource.h>

namespace Graphics
{

	/// <summary>
	/// This class implements a different Graphics component where there are several sprites stored as animations and we just 
	/// </summary>
	class SelectableSpritesComponent :public IGraphicsComponent
	{
	protected:
		std::vector<std::shared_ptr<ImageResource>> m_SpriteList;
		uint32_t m_IdleSprite;
		uint32_t m_SelectedSprite;
		uint32_t m_CurrentSprite;

	public:

		static const std::string COMPONENT_NAME;

		/// <summary>
		/// Initializes a new instance of the <see cref="AnimatedSpritesComponent" /> class.
		/// </summary>
		SelectableSpritesComponent() :IGraphicsComponent(SelectableSpritesComponent::COMPONENT_NAME),
			m_IdleSprite(0), m_SelectedSprite(0)
		{
		}
		/// <summary>
		/// Initializes a new instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
		/// </summary>
		/// <param name="owner">The owner entity.</param>
		/// <param name="sprites">The sprites.</param>
		/// <param name="idleSprite">The idle sprite.</param>
		/// <param name="selectedSprite">The selected sprite.</param>
		SelectableSpritesComponent(Entity *owner, RenderSystem* renderSystem, std::vector<std::string> sprites, uint32_t idleSprite,uint32_t selectedSprite);
		/// <summary>
		/// Finalizes an instance of the <see cref="StaticAnimatedSpritesComponent"/> class.
		/// </summary>
		virtual ~SelectableSpritesComponent();


		virtual void OnRender() override;

		/// <summary>
		/// Builds the specified Component using the specified descriptor.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void Build(JSONNode *descriptor) override;

		/// <summary>
		/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void OnEntityEvent(IEventData const & event) override;
	private:
		/// <summary>
		/// Clones the current Component
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual std::shared_ptr<IComponent> Clone() override;

	public:


	};

}