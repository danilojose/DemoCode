#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\UserControlledBehaviour.h>

const int PlayerFireDirection = 1;
const uint32_t PLAYER_SPEED = 20;
const uint32_t PLAYER_FIRERATE = 500;
using namespace AI;

const std::string AI::UserControlledBehaviour::COMPONENT_NAME = "UserControlledBehaviour";

extern GameLogic * g_pGameLogic;
/// <summary>
/// Initializes a new instance of the <see cref="UserControlledBehaviour"/> class.
/// </summary>
/// <param name="owner">The Entity owner identifier.</param>
/// <param name="movementSpeed">The movement speed.</param>
/// <param name="fireRate">The fire rate.</param>
/// <param name="sound">The sound.</param>
UserControlledBehaviour::UserControlledBehaviour(Entity *owner, int movementSpeed, uint32_t fireRate, const std::string &shot)
:IBehaviourComponent(UserControlledBehaviour::COMPONENT_NAME,
						owner, 0), m_MovementSpeed(movementSpeed), m_FireRate(fireRate), m_LastFire(UINT32_MAX)
{
}

/// <summary>
/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void UserControlledBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{
	m_LastFire += deltaMilliseconds;
	bool updatedPosition=false; 
	if (Input::keyRight())
	{
		int posX = m_Entity->GetPosX();
		posX += m_MovementSpeed;
		if (posX < 800)
		{
			m_Entity->SetPosX(posX);
			updatedPosition = true;
		}
	}
	else
	{
		if (Input::keyLeft())
		{
			int posX = m_Entity->GetPosX();
			posX -= m_MovementSpeed;
			if (posX >-20)
			{
				m_Entity->SetPosX(posX);
				updatedPosition = true;
			}
		}
	}

	if (updatedPosition)
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_Entity->GetID(), m_Entity->GetPosX(),m_Entity->GetPosY())));
	}

	if (Input::keyFire())
	{
		if (m_LastFire > m_FireRate)
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreatePlayerFire(m_Entity->GetID(), m_Entity->GetPosX(), m_Entity->GetPosY())));

			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlaySound("sounds\\player_shot.wav")));
			m_LastFire = 0;
		}
	}
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void UserControlledBehaviour::Build(JSONNode *descriptor)
{
	//default values;
	m_MovementSpeed = PLAYER_SPEED;
	m_FireRate = PLAYER_FIRERATE;

	m_MovementSpeed = descriptor->GetUInteger("Speed");
	ASSERT_DESCRIPTION(m_MovementSpeed > 0, "Movement Speed must be greater than zero");
	m_FireRate = descriptor->GetUInteger("FireRate");
	ASSERT_DESCRIPTION(m_FireRate > 0, "Movement Speed must be greater than zero");
	m_Sound = descriptor->GetString("FireSound");

	ASSERT_DESCRIPTION(!m_Sound.empty(), "Movement Speed must be greater than zero");
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> UserControlledBehaviour::Clone(Entity *entity)
{
	std::shared_ptr<UserControlledBehaviour> cloned = std::shared_ptr<UserControlledBehaviour>(GCC_NEW UserControlledBehaviour());
	cloned->m_MovementSpeed = this->m_MovementSpeed;
	cloned->m_FireRate = this->m_FireRate;
	cloned->m_Sound = this->m_Sound;
	cloned->m_Entity = entity;
	g_pGameLogic->AddBehaviour(cloned);
	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void UserControlledBehaviour::OnEntityEvent(IEventData const & event)
{

}