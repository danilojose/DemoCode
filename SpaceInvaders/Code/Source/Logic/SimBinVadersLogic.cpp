#include <Logic\SimBinVadersLogic.h>
#include <System\Assert.h>



using namespace AI;
/// <summary>
/// Initializes a new instance of the <see cref="SimBinGameLogic"/> class.
/// </summary>
/// <param name="lives">The lives.</param>
SimBinGameLogic::SimBinGameLogic(uint32_t lives)
{
	m_Score = 0;
	m_Lives = lives;
	m_State = SB_Initializing;
	m_RenderDiagnostics = false;
	m_pAiEventListener = EventListenerPtr(GCC_NEW AiEventListener(this));
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_DestroyActor::sk_EventType);
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_EnemyCollisionDetected::sk_EventType);
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_PlayerCollisionDetected::sk_EventType);

}
/// <summary>
/// Finalizes an instance of the <see cref="SimBinGameLogic"/> class.
/// </summary>
SimBinGameLogic::~SimBinGameLogic()
{

	m_BehaviourList.clear();

}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void SimBinGameLogic::TogglePause(bool active)
{
	ASSERT_DESCRIPTION(active, "NOT Implemented")
}

/// <summary>
/// Reset this instance.
/// </summary>
void SimBinGameLogic::Reset()
{
	m_BehaviourList.clear();
}

/// <summary>
/// Adds the behaviour.
/// </summary>
/// <param name="actor">The actor.</param>
void SimBinGameLogic::AddBehaviour(std::shared_ptr<IBehaviourComponent> behaviour)
{
	ASSERT_DESCRIPTION(behaviour->VGetOwner() > 0, "Attempted to add an actor with no valid ID")

		m_BehaviourList[behaviour->VGetOwner()] = behaviour;

}
/// <summary>
/// Gets the behaviour.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<IBehaviourComponent> SimBinGameLogic::GetBehaviour(const uint32_t id)
{
	auto it = m_BehaviourList.find(id);
	if (it == m_BehaviourList.end())
	{
		std::shared_ptr<IBehaviourComponent> null;
		return null; 
	}
	return it->second;
}

/// <summary>
/// Removes the behaviour.
/// </summary>
/// <param name="id">The identifier.</param>
void SimBinGameLogic::RemoveBehaviour(uint32_t id)
{
	std::shared_ptr<IBehaviourComponent> actor = GetBehaviour(id);
	if (actor)
	{
		m_BehaviourList.erase(id);
	}

}

/// <summary>
/// Called when Update is called from the mainloop
/// </summary>
/// <param name="elapsedTime">The elapsed time.</param>
/// <returns></returns>
bool SimBinGameLogic::OnUpdate(uint32_t elapsedTime)
{
	for (const auto &behaviour : m_BehaviourList)
	{
		behaviour.second->behave(elapsedTime);
	}
	return m_ContinueRunning;
}

// Changing Game Logic State
/// <summary>
/// Changes the state.
/// </summary>
/// <param name="newState">The new state.</param>
void SimBinGameLogic::ChangeState(SimBinGameState newState)
{
	if (newState == SB_LoadingInGame)
	{
		// Get rid of the Main Menu...
	}

	m_State = newState;
	///safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Game_State(m_State)));
}

/// <summary>
/// Updates the current leader.
/// </summary>
void SimBinGameLogic::UpdateLeader()
{
	///TODO Pending to implement
}


/// <summary>
/// Handles the event. React when the other systems trigger events that might be interesting for the AI.
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool AiEventListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();
	if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_DestroyActor const & ed = static_cast< const EvtData_DestroyActor & >(event);
		m_LogicSystem->RemoveBehaviour(ed.m_ActorId);
		return true;
	}

	if (eventType == EvtData_EnemyCollisionDetected::sk_EventType)
	{
		EvtData_EnemyCollisionDetected const & ed = static_cast< const EvtData_EnemyCollisionDetected & >(event);

		std::shared_ptr<IBehaviourComponent> friendlyEntity = m_LogicSystem->GetBehaviour(ed.m_ActorPlayerId);
		std::shared_ptr<IBehaviourComponent> enemyEntity = m_LogicSystem->GetBehaviour(ed.m_ActorEnemyId);

		if ((!friendlyEntity) || (!enemyEntity))
		{
			//In this case we are receiving a collisiondetected event from actors that were destroyed in the same tick
			return false;
		}
		else
		{
			friendlyEntity->VSetAlive(false);
			enemyEntity->VSetAlive(false);
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(friendlyEntity->VGetOwner())));
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(enemyEntity->VGetOwner())));
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreateEnemyExplosion(enemyEntity->VGetOwner(),enemyEntity->VGetPositionX(),enemyEntity->VGetPositionY())));
			uint16_t score=m_LogicSystem->AddScore(enemyEntity->VGetPoints());
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PointsObtained(score)));
		}
		return true;
	}
	if (eventType == EvtData_PlayerCollisionDetected::sk_EventType)
	{
		EvtData_PlayerCollisionDetected const & ed = static_cast< const EvtData_PlayerCollisionDetected & >(event);

		std::shared_ptr<IBehaviourComponent> friendlyEntity = m_LogicSystem->GetBehaviour(ed.m_ActorPlayerId);
		std::shared_ptr<IBehaviourComponent> enemyEntity = m_LogicSystem->GetBehaviour(ed.m_ActorEnemyId);

		if ((!friendlyEntity) || (!enemyEntity))
		{
			//In this case we are receiving a collisiondetected event from actors that were destroyed in the same tick
			return false;
		}
		else
		{
			friendlyEntity->VSetAlive(false);
			enemyEntity->VSetAlive(false);
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(friendlyEntity->VGetOwner())));
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(enemyEntity->VGetOwner())));
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_CreatePlayerExplosion(friendlyEntity->VGetOwner(), friendlyEntity->VGetPositionX(), friendlyEntity->VGetPositionY())));
			if (m_LogicSystem->GetCurrentLives() > 0)
			{
				m_LogicSystem->SetCurrentLives(m_LogicSystem->GetCurrentLives() - 1);
				IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_LivesRemaining(m_LogicSystem->GetCurrentLives())));
			}
			else
			{
				m_LogicSystem->AbortGame();
			}
		}
		return true;
	}

	return false;

}