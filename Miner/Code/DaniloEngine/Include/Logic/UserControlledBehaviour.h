#pragma once

#include <Logic\IBehaviourComponent.h>
#include <3rdParty\Input.h>

namespace AI
{
/// <summary>
/// User controlled behaviour. This is the Behaviour that reacts to user input like Keyboard
/// </summary>
class UserControlledBehaviour : public IBehaviourComponent
{
private:
	int														m_MovementSpeed;
	bool													m_FirePressed;
	uint32_t												m_FireRate;
	uint32_t												m_LastFire;
	uint32_t												m_Lives;
	std::shared_ptr<Input>									m_InputController;
	std::string												m_Sound;


public:

	static const std::string COMPONENT_NAME;
	/// <summary>
	/// Initializes a new instance of the <see cref="UserControlledBehaviour"/> class.
	/// </summary>
	/// <param name="actorId">The actor identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	/// <param name="movementSpeed">The movement speed.</param>
	/// <param name="fireRate">The fire rate.</param>
	/// <param name="sound">The sound.</param>
	UserControlledBehaviour() :IBehaviourComponent(UserControlledBehaviour::COMPONENT_NAME),
		m_MovementSpeed(0), m_FireRate(0), m_LastFire(UINT32_MAX)
	{}
	/// <summary>
	/// Initializes a new instance of the <see cref="UserControlledBehaviour"/> class.
	/// </summary>
	/// <param name="owner">The entity owner.</param>
	/// <param name="movementSpeed">The movement speed.</param>
	/// <param name="fireRate">The fire rate.</param>
	/// <param name="sound">The sound.</param>
	UserControlledBehaviour(Entity *owner, int movementSpeed, uint32_t fireRate, const std::string &sound);
	/// <summary>
	/// Finalizes an instance of the <see cref="UserControlledBehaviour"/> class.
	/// </summary>
	virtual ~UserControlledBehaviour(){
	}

	/// <summary>
	/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
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
	virtual std::shared_ptr<IComponent> Clone(Entity *entity) override;

	/// <summary>
	/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void OnEntityEvent(IEventData const & event) override;
};
}