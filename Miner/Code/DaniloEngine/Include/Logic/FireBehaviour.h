#pragma once

#include <Logic\IBehaviourComponent.h>
#include <3rdParty\Input.h>
#include <System\Entity.h>
namespace AI
{
	/// <summary>
	/// The Class that implements the behaviour of a fire done by either an enemy or a player
	/// </summary>
	class FireBehaviour : public IBehaviourComponent
	{
	private:
		int m_MovementSpeed;

	public:

		static const std::string COMPONENT_NAME;
		/// <summary>
		/// Initializes a new instance of the <see cref="FireBehaviour"/> class.
		/// </summary>
		FireBehaviour() :IBehaviourComponent(FireBehaviour::COMPONENT_NAME),
			m_MovementSpeed(0)
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="FireBehaviour"/> class.
		/// </summary>
		/// <param name="owner">The entity owner.</param>
		/// <param name="movementSpeed">The movement speed.</param>
		FireBehaviour(Entity *owner,int movementSpeed);
		/// <summary>
		/// Finalizes an instance of the <see cref="FireBehaviour"/> class.
		/// </summary>
		virtual ~FireBehaviour(){
		}
		/// <summary>
		/// Provides the behaviour to the component. It just goes in the direction provided by the enemy or player class and gets destroyed once it gets beyond the limits of the board
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void OnUpdate(uint32_t deltaMilliseconds) override;

		/// <summary>
		/// Builds the specified Component using the specified descriptor.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void Build(JSONNode *descriptor) override;

		/// <summary>
		/// Clones the current Component
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual std::shared_ptr<IComponent> Clone() override;

		/// <summary>
		/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void OnEntityEvent(IEventData const & event) override;
	};
}