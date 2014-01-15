#include <Logic\StoneBehaviour.h>
#include <System\Assert.h>

const std::string StoneBehaviour::COMPONENT_NAME = "StoneBehaviour";

/// <summary>
/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void StoneBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{
	
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void StoneBehaviour::Build(JSONNode *descriptor)
{
	ASSERT_DESCRIPTION(descriptor, "Not a valid descriptor");

	m_Points = descriptor->GetUInteger("Points");
	m_TimeBonus = descriptor->GetUInteger("TimeBonus");
	ASSERT_DESCRIPTION(m_Points>0, "Points Value must be higher than 0");
	ASSERT_DESCRIPTION(m_TimeBonus>0, "Time Bonus Value must be higher than 0");
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> StoneBehaviour::Clone()
{
	std::shared_ptr<StoneBehaviour> cloned = std::shared_ptr<StoneBehaviour>(GCC_NEW StoneBehaviour());
	cloned->m_TimeBonus = this->m_TimeBonus;
	cloned->m_Alive = this->m_Alive;
	cloned->m_Points = this->m_Points;
	IEventManager::Get()->VAddListener(cloned->m_pStoneBehaviourListener, EvtData_StoneMovementRequested::sk_EventType);

	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void StoneBehaviour::OnEntityEvent(IEventData const & event)
{

}

/// <summary>
/// Evaluates the hit.
/// </summary>
/// <param name="targetPosition">The target position.</param>
/// <param name="middlePosition">The middle position.</param>
void StoneBehaviour::CreateAIPath(const std::pair<uint16_t, uint16_t> &targetPosition, const std::pair<uint16_t, uint16_t> &middlePosition)
{
	if (m_AIState == AISTATE::IDLE)
	{
		m_AIPath.clear();
		std::pair<uint16_t, uint16_t> currentPosition(m_Entity->GetPosX(), m_Entity->GetPosY());
		if ((middlePosition.first != 0) && (middlePosition.second != 0))
		{
			int8_t incX = (middlePosition.first - m_Entity->GetPosX())? 1:-1;
			int8_t incY = (middlePosition.second - m_Entity->GetPosY())? 1:-1;

			while (currentPosition.first != middlePosition.first&&currentPosition.second != middlePosition.second)
			{
				if (currentPosition.first != middlePosition.first)
				{
					currentPosition.first = currentPosition.first + (incX * 10);
				}
				if (currentPosition.second != middlePosition.second)
				{
					currentPosition.second = currentPosition.second + (incY * 10);
				}
				m_AIPath.push_back(std::pair<uint16_t, uint16_t>(currentPosition.first, currentPosition.second));
			}
		}

		int8_t incX = (targetPosition.first - currentPosition.first) ? 1 : -1;
		int8_t incY = (targetPosition.second - currentPosition.second) ? 1 : -1;

		while (currentPosition.first != targetPosition.first&&currentPosition.second != targetPosition.second)
		{
			if (currentPosition.first != targetPosition.first)
			{
				currentPosition.first = currentPosition.first + (incX * 10);
			}
			if (currentPosition.second != targetPosition.second)
			{
				currentPosition.second = currentPosition.second + (incY * 10);
			}
			m_AIPath.push_back(std::pair<uint16_t, uint16_t>(currentPosition.first, currentPosition.second));
		}

		m_AIState = AISTATE::MOVING;
	}


}


/// <summary>
/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool StoneBehaviourListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_StoneMovementRequested::sk_EventType)
	{
		EvtData_StoneMovementRequested const & ed = static_cast< const EvtData_StoneMovementRequested & >(event);
		m_Stone->CreateAIPath(ed.m_TargetPosition, ed.m_MiddlePosition);
	}

	return true;
}