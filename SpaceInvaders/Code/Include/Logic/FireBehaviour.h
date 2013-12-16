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
		/// <summary>
		/// Initializes a new instance of the <see cref="FireBehaviour"/> class.
		/// </summary>
		/// <param name="actorId">The actor identifier.</param>
		/// <param name="posX">The position x.</param>
		/// <param name="posY">The position y.</param>
		/// <param name="movementSpeed">The movement speed.</param>
		FireBehaviour(uint32_t actorId, uint32_t posX, uint32_t posY,int movementSpeed);
		/// <summary>
		/// Finalizes an instance of the <see cref="FireBehaviour"/> class.
		/// </summary>
		virtual ~FireBehaviour(){
		}
		/// <summary>
		/// Provides the behaviour to the component. It just goes in the direction provided by the enemy or player class and gets destroyed once it gets beyond the limits of the board
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void behave(uint32_t deltaMilliseconds) override;

	};
}