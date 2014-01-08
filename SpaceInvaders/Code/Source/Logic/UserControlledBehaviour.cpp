#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\UserControlledBehaviour.h>

const int PlayerFireDirection = 1;
using namespace AI;

const std::string AI::UserControlledBehaviour::COMPONENT_NAME = "UserControlledBehaviour";
/// <summary>
/// Initializes a new instance of the <see cref="UserControlledBehaviour"/> class.
/// </summary>
/// <param name="actorId">The actor identifier.</param>
/// <param name="posX">The position x.</param>
/// <param name="posY">The position y.</param>
/// <param name="movementSpeed">The movement speed.</param>
/// <param name="fireRate">The fire rate.</param>
/// <param name="sound">The sound.</param>
UserControlledBehaviour::UserControlledBehaviour(uint32_t actorId, uint32_t posX, uint32_t posY, int movementSpeed, uint32_t fireRate,const std::string &shot) 
						:IBehaviourComponent(UserControlledBehaviour::COMPONENT_NAME,
						actorId, posX, posY,0), m_MovementSpeed(movementSpeed), m_FireRate(fireRate), m_LastFire(UINT32_MAX)
{
}

/// <summary>
/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void UserControlledBehaviour::behave(uint32_t deltaMilliseconds)
{
	m_LastFire += deltaMilliseconds;
	bool updatedPosition=false; 
	if (Input::keyRight())
	{
		int posX = m_PosX;
		posX += m_MovementSpeed;
		if (posX < 800)
		{
			m_PosX=posX;
			updatedPosition = true;
		}
	}
	else
	{
		if (Input::keyLeft())
		{
			int posX = m_PosX;
			posX -= m_MovementSpeed;
			if (posX >-20)
			{
				m_PosX = posX;
				updatedPosition = true;
			}
		}
	}

	if (updatedPosition)
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_ActorId, m_PosX, m_PosY)));
	}

	if (Input::keyFire())
	{
		if (m_LastFire > m_FireRate)
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreatePlayerFire(m_ActorId, m_PosX, m_PosY)));

			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlaySound("sounds\\player_shot.wav")));
			m_LastFire = 0;
		}
	}
}

