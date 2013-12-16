#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\FireBehaviour.h>
using namespace AI;
/// <summary>
/// Initializes a new instance of the <see cref="FireBehaviour"/> class.
/// </summary>
/// <param name="actorId">The actor identifier.</param>
/// <param name="posX">The position x.</param>
/// <param name="posY">The position y.</param>
/// <param name="movementSpeed">The movement speed.</param>
FireBehaviour::FireBehaviour(uint32_t actorId,uint32_t posX, uint32_t posY, int movementSpeed) :IBehaviourComponent(actorId,posX,posY,0)
{
	m_MovementSpeed = movementSpeed;
}

/// <summary>
/// Provides the behaviour to the component. It just goes in the direction provided by the enemy or player class and gets destroyed once it gets beyond the limits of the board
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void FireBehaviour::behave(uint32_t deltaMilliseconds)
{
	if (VGetAlive())
	{
		int posY = m_PosY;
		posY += m_MovementSpeed;
		if ((posY < 768) && (posY>0))
		{
			m_PosY=posY;
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_ActorId, m_PosX, m_PosY)));
		}
		else
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(m_ActorId)));
		}
	}
}
