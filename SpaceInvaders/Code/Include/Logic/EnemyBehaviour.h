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
		std::string m_SoundFire;

	public:

		static const std::string COMPONENT_NAME;

		/// <summary>
		/// Initializes a new instance of the <see cref="EnemyBehaviour"/> class.
		/// </summary>
		EnemyBehaviour() :IBehaviourComponent(EnemyBehaviour::COMPONENT_NAME),
						m_MovementXSpeed(0), m_MovementYSpeed(0), m_LastFire(0), m_FireRate(0)
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="EnemyBehaviour" /> class.
		/// </summary>
		/// <param name="owner">The entity owner</param>
		/// <param name="posX">The position x.</param>
		/// <param name="posY">The position y.</param>
		/// <param name="movementXSpeed">The movement x speed.</param>
		/// <param name="movementYSpeed">The movement y speed.</param>
		/// <param name="shot">The shot.</param>
		/// <param name="points">The points.</param>
		EnemyBehaviour(Entity* owner,int movementXSpeed,int movementYSpeed,const std::string &shot,uint16_t points);
		/// <summary>
		/// Finalizes an instance of the <see cref="EnemyBehaviour"/> class.
		/// </summary>
		virtual ~EnemyBehaviour(){
		}
		virtual void OnUpdate(uint32_t deltaMilliseconds) override;

		/// <summary>
		/// Builds the specified Component using the specified descriptor.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void Build(const IniValuesMap &descriptor) override;
	};
}