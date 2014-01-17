#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\EnemyBehaviour.h>
#include <time.h> 

const uint16_t MAX_FIRERATE = 30000;
const uint16_t MIN_FIRERATE = 2000;
const uint32_t SPAWN_SPEEDX = 5;
const uint32_t SPAWN_SPEEDY = 50;
using namespace AI;

const std::string EnemyBehaviour::COMPONENT_NAME = "EnemyBehaviour";

extern GameLogic * g_pGameLogic;
/// <summary>
/// Initializes a new instance of the <see cref="EnemyBehaviour" /> class.
/// </summary>
/// <param name="owner">The owner identifier.</param>
/// <param name="movementXSpeed">The movement x speed.</param>
/// <param name="movementYSpeed">The movement y speed.</param>
/// <param name="shot">The shot.</param>
/// <param name="points">The points.</param>
EnemyBehaviour::EnemyBehaviour(Entity *owner, int movementXSpeed, int movementYSpeed,const std::string &shot,uint16_t points) :IBehaviourComponent(EnemyBehaviour::COMPONENT_NAME,owner,points),
							  m_MovementXSpeed(movementXSpeed), m_MovementYSpeed(movementYSpeed), m_LastFire(0), m_SoundFire(shot)
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
void EnemyBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{
	m_LastFire += deltaMilliseconds;

	int posX = m_Entity->GetPosX() + m_MovementXSpeed;
	if ((posX<0) || (posX>800))
	{
		int posY = m_Entity->GetPosY()+ m_MovementYSpeed;
		if ((posY<0) || (posY>730))
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(m_Entity->GetID())));
		}
		else
		{
			m_MovementXSpeed = m_MovementXSpeed*(-1);
			m_Entity->SetPosY(posY);
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_Entity->GetID(), m_Entity->GetPosX(), m_Entity->GetPosY())));

		}
	}
	else
	{
		m_Entity->SetPosX(posX);
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_Entity->GetID(), m_Entity->GetPosX(),m_Entity->GetPosY())));
	}

	if (m_LastFire > m_FireRate)
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreateEnemyFire(m_Entity->GetID(), m_Entity->GetPosX(), m_Entity->GetPosY())));

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlaySound("sounds\\player_shot.wav")));
		m_LastFire = 0;
	}


}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void EnemyBehaviour::Build(JSONNode *descriptor)
{

	m_MovementXSpeed = descriptor->GetUInteger("SpeedX");
	m_MovementYSpeed = descriptor->GetUInteger("SpeedY");

	std::string fireSound = descriptor->GetString("FireSound");
	ASSERT_DESCRIPTION(!fireSound.empty(), "Not valid fire sound for the given entity");
	m_SoundFire = fireSound;

	m_FireRate = rand() % MAX_FIRERATE;
	if (m_FireRate < MIN_FIRERATE)
	{
		m_FireRate += MIN_FIRERATE;
	}

}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> EnemyBehaviour::Clone(Entity *entity)
{
	std::shared_ptr<EnemyBehaviour> cloned = std::shared_ptr<EnemyBehaviour>(GCC_NEW EnemyBehaviour());
	cloned->m_MovementXSpeed = this->m_MovementXSpeed;
	cloned->m_MovementYSpeed = this->m_MovementYSpeed;
	cloned->m_SoundFire = this->m_SoundFire;
	cloned->m_FireRate = this->m_FireRate;
	cloned->m_Entity = entity;
	g_pGameLogic->AddBehaviour(cloned);
	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void EnemyBehaviour::OnEntityEvent(IEventData const & event)
{

}