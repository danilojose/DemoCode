#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\FireBehaviour.h>
using namespace AI;

const std::string FireBehaviour::COMPONENT_NAME = "FireBehaviour";
/// <summary>
/// Initializes a new instance of the <see cref="FireBehaviour"/> class.
/// </summary>
/// <param name="owner">The owner Entity</param>
/// <param name="movementSpeed">The movement speed.</param>
FireBehaviour::FireBehaviour(Entity* owner, int movementSpeed) :IBehaviourComponent(FireBehaviour::COMPONENT_NAME, owner, 0),
				m_MovementSpeed(movementSpeed)
{
}

/// <summary>
/// Provides the behaviour to the component. It just goes in the direction provided by the enemy or player class and gets destroyed once it gets beyond the limits of the board
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void FireBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{
	if (GetAlive())
	{
		int posY = m_Entity->GetPosY();
		posY += m_MovementSpeed;
		if ((posY < 768) && (posY>0))
		{
			m_Entity->SetPosY(posY);
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_Entity->GetID(), m_Entity->GetPosX(), m_Entity->GetPosY())));
		}
		else
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(m_Entity->GetID())));
		}
	}
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void FireBehaviour::Build(JSONNode *descriptor)
{

	m_MovementSpeed=descriptor->GetUInteger("Speed");
	ASSERT_DESCRIPTION(m_MovementSpeed > 0, "movement speed must be greater than 0");
	m_Alive = true;
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> FireBehaviour::Clone()
{
	std::shared_ptr<FireBehaviour> cloned = std::shared_ptr<FireBehaviour>(GCC_NEW FireBehaviour());
	cloned->m_MovementSpeed = this->m_MovementSpeed;
	cloned->m_Alive = true;
	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void FireBehaviour::OnEntityEvent(IEventData const & event)
{

}