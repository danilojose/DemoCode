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
	int m_MovementSpeed;
	bool m_FirePressed;
	uint32_t m_FireRate;
	uint32_t m_LastFire;
	uint32_t m_Lives;
	std::shared_ptr<Input> m_InputController;
	std::string m_Sound;


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
	UserControlledBehaviour(uint32_t actorId, uint32_t posX, uint32_t posY,int movementSpeed,uint32_t fireRate,const std::string &sound);
	/// <summary>
	/// Finalizes an instance of the <see cref="UserControlledBehaviour"/> class.
	/// </summary>
	virtual ~UserControlledBehaviour(){
	}
	virtual void behave(uint32_t deltaMilliseconds) override;

};
}