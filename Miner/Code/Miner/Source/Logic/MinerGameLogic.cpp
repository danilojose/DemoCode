#include <Logic\MinerGameLogic.h>
#include <System\EntityEvents.h>
#include <System\MinerEntityEvents.h>
#include <Logic\StoneBehaviour.h>
#include <UI\UISystem.h>
#include <ostream>

using namespace AI;
using namespace GameSystem;
extern UISystem *g_pUISystem; 
/// <summary>
/// Reset this instance.
/// </summary>
/// <param name="lives">The lives.</param>
void MinerGameLogic::Reset(uint32_t lives)
{
	GameLogic::Reset(lives);

	m_pMinerLogicListener = EventListenerPtr(GCC_NEW MinerLogicListener(this));
	IEventManager::Get()->VAddListener(m_pMinerLogicListener, EvtData_StoneMovementRequested::sk_EventType);
	IEventManager::Get()->VAddListener(m_pMinerLogicListener, EvtData_ReleasePosition::sk_EventType);

	g_pUISystem->RegisterGameLogicBindings(HashedString("GetScore"),std::mem_fn(&GameLogic::BindingGetScore));
	g_pUISystem->RegisterGameLogicBindings(HashedString("GetTime"), std::mem_fn(&GameLogic::BindingGetLives));
}

/// <summary>
/// Requests the entity to move.
/// </summary>
/// <param name="entityId">The entity identifier.</param>
/// <param name="targetPosition">The target position.</param>
/// <param name="middlePosition">The middle position.</param>
/// <returns></returns>
void MinerGameLogic::RequestEntityToMove(uint32_t entityId, const std::pair<uint16_t, uint16_t> &targetPosition, const std::pair<uint16_t, uint16_t> &middlePosition)
{
	const auto & iter = m_BehaviourList.find(entityId);
	if (iter != m_BehaviourList.end())
	{
		StoneBehaviour *stone = std::dynamic_pointer_cast<StoneBehaviour>(iter->second).get();
		stone->CreateAIPath(targetPosition, middlePosition);
	}
}
void MinerGameLogic::ReleaseCell(const std::pair<uint16_t, uint16_t> & position)
{
	std::shared_ptr<Cell> cell=m_pWorld->GetCellAtWorldPosition(position.first, position.second);
	cell->Clear();
	cell->UnLock();
}

/// <summary>
/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool MinerLogicListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_StoneMovementRequested::sk_EventType)
	{
		EvtData_StoneMovementRequested const & ed = static_cast< const EvtData_StoneMovementRequested & >(event);
		m_MinerLogic->RequestEntityToMove(ed.m_EntityId, ed.m_TargetPosition, ed.m_MiddlePosition);
	}
	else
	{
		if (eventType == EvtData_ReleasePosition::sk_EventType)
		{
			EvtData_ReleasePosition const & ed = static_cast< const EvtData_ReleasePosition & >(event);
			m_MinerLogic->ReleaseCell(ed.m_TargetPosition);
		}

	}


	return true;
}

/// <summary>
/// Called when Update is called from the mainloop
/// </summary>
/// <param name="elapsedTime">The elapsed time.</param>
/// <returns></returns>
bool MinerGameLogic::OnUpdate(uint32_t elapsedTime)
{
	bool bContinueUpdating=GameLogic::OnUpdate(elapsedTime);
	if (m_Lives <= elapsedTime)
	{
		m_ContinueRunning = false;
	}
	else
	{
		m_Lives -= elapsedTime;
	}
	return bContinueUpdating&&m_ContinueRunning;
}

