#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\EnemyBehaviour.h>
#include <time.h> 

const uint16_t MAX_FIRERATE = 30000;
const uint16_t MIN_FIRERATE = 2000;
using namespace AI;

const std::string EnemyBehaviour::COMPONENT_NAME = "EnemyBehaviour";
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
EnemyBehaviour::EnemyBehaviour(uint32_t actorId, uint32_t posX, uint32_t posY,int movementXSpeed, int movementYSpeed,const std::string &shot,uint16_t points) :IBehaviourComponent(EnemyBehaviour::COMPONENT_NAME,actorId,posX,posY,points),
							  m_MovementXSpeed(movementXSpeed), m_MovementYSpeed(movementYSpeed), m_LastFire(0), m_Shot(shot)
{
	m_FireRate = rand() % MAX_FIRERATE;
	if(m_FireRate < MIN_FIRERATE)
	{
		m_FireRate += MIN_FIRERATE;
	}
}

/// <summary>
/// Implements the behaviour of the enemy. Expected behaviour is to move towards current direction until a barrier is found. At that moment change the direction and move towards the opposite.
/// When the last fire was done longer than the current fire rate the enemy fires again
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void EnemyBehaviour::behave(uint32_t deltaMilliseconds)
{
	m_LastFire += deltaMilliseconds;

	int posX = m_PosX + m_MovementXSpeed;
	if ((posX<0) || (posX>800))
	{
		int posY = m_PosY+ m_MovementYSpeed;
		if ((posY<0) || (posY>730))
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(m_ActorId)));
		}
		else
		{
			m_MovementXSpeed = m_MovementXSpeed*(-1);
			m_PosY=posY;
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_ActorId, m_PosX, m_PosY)));

		}
	}
	else
	{
		m_PosX = posX;
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_ActorId, m_PosX,m_PosY)));
	}

	if (m_LastFire > m_FireRate)
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreateEnemyFire(m_ActorId, m_PosX, m_PosY)));

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlaySound("sounds\\player_shot.wav")));
		m_LastFire = 0;
	}


}
