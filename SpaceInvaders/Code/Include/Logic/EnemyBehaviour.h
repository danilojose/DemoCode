#pragma once

#include <Logic\IBehaviourComponent.h>
#include <3rdParty\Input.h>
#include <System\Entity.h>

namespace AI
{
	/// <summary>
	/// Enemy Behaviour in charge of the AI for the enemies.Controlled by the computer entirely
	/// </summary>
	class EnemyBehaviour : public IBehaviourComponent
	{
	private:
		int m_MovementXSpeed;
		int m_MovementYSpeed;
		uint16_t m_FireRate;
		uint16_t m_LastFire;
		std::string m_Shot;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="EnemyBehaviour" /> class.
		/// </summary>
		/// <param name="actorId">The actor identifier.</param>
		/// <param name="posX">The position x.</param>
		/// <param name="posY">The position y.</param>
		/// <param name="movementXSpeed">The movement x speed.</param>
		/// <param name="movementYSpeed">The movement y speed.</param>
		/// <param name="shot">The shot.</param>
		/// <param name="points">The points.</param>
		EnemyBehaviour(uint32_t actorId,uint32_t posX,uint32_t posY,int movementXSpeed,int movementYSpeed,const std::string &shot,uint16_t points);
		/// <summary>
		/// Finalizes an instance of the <see cref="EnemyBehaviour"/> class.
		/// </summary>
		virtual ~EnemyBehaviour(){
		}
		virtual void behave(uint32_t deltaMilliseconds) override;

	};
}