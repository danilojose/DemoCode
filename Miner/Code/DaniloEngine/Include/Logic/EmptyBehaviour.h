#pragma once

#include <Logic\IBehaviourComponent.h>
#include <3rdParty\Input.h>
#include <System\Entity.h>
namespace AI
{
	/// <summary>
	/// This class is an utily class that I use sometimes to make tests
	/// </summary>
	class EmptyBehaviour : public IBehaviourComponent
	{
	private:

	public:
		static const std::string					COMPONENT_NAME;

		/// <summary>
		/// Initializes a new instance of the <see cref="EmptyBehaviour"/> class.
		/// </summary>
		EmptyBehaviour() :IBehaviourComponent(EmptyBehaviour::COMPONENT_NAME){}

		/// <summary>
		/// Initializes a new instance of the <see cref="EmptyBehaviour"/> class.
		/// </summary>
		/// <param name="owner">The entity owner</param>
		EmptyBehaviour(Entity *owner) :IBehaviourComponent(EmptyBehaviour::COMPONENT_NAME,owner, 0){}
		/// <summary>
		/// Finalizes an instance of the <see cref="EmptyBehaviour"/> class.
		/// </summary>
		virtual ~EmptyBehaviour(){
		}
		/// <summary>
		/// Behaves the specified delta milliseconds.
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void OnUpdate(uint32_t deltaMilliseconds) override {};

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
		virtual void OnEntityEvent(IEventData const & event) override;
	};
}
