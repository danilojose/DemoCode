#include <Logic\GameLogic.h>
#include <System\Assert.h>
#include <System\Entity.h>


GameLogic * g_pGameLogic;
using namespace AI;
/// <summary>
/// Initializes a new instance of the <see cref="GameLogic"/> class.
/// </summary>
/// <param name="lives">The lives.</param>
GameLogic::GameLogic() :m_Score(0), m_Lives(0), m_State(SB_Initializing), m_RenderDiagnostics(false), m_ContinueRunning(true)
{
	m_pAiEventListener = EventListenerPtr(GCC_NEW AiEventListener(this));
	g_pGameLogic = this;

}
/// <summary>
/// Finalizes an instance of the <see cref="GameLogic"/> class.
/// </summary>
GameLogic::~GameLogic()
{

	m_BehaviourList.clear();

}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void GameLogic::TogglePause(bool active)
{
	ASSERT_DESCRIPTION(active, "NOT Implemented")
}

/// <summary>
/// Reset this instance.
/// </summary>
void GameLogic::Reset(uint32_t lives)
{
	m_Lives = lives;
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_DestroyActor::sk_EventType);
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_EnemyCollisionDetected::sk_EventType);
	IEventManager::Get()->VAddListener(m_pAiEventListener, EvtData_PlayerCollisionDetected::sk_EventType);

	m_BehaviourList.clear();
}

/// <summary>
/// Adds the behaviour.
/// </summary>
/// <param name="actor">The actor.</param>
void GameLogic::AddBehaviour(std::shared_ptr<IBehaviourComponent> behaviour)
{
	ASSERT_DESCRIPTION(behaviour->GetEntity()!=nullptr&&behaviour->GetEntity()->GetID() > 0, "Attempted to add an actor with no valid ID")

	m_BehaviourList[behaviour->GetEntity()->GetID()] = behaviour;

}
/// <summary>
/// Gets the behaviour.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<IBehaviourComponent> GameLogic::GetBehaviour(const uint32_t id)
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
void GameLogic::RemoveBehaviour(uint32_t id)
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
bool GameLogic::OnUpdate(uint32_t elapsedTime)
{
	for (const auto &behaviour : m_BehaviourList)
	{
		behaviour.second->OnUpdate(elapsedTime);
	}
	return m_ContinueRunning;
}

// Changing Game Logic State
/// <summary>
/// Changes the state.
/// </summary>
/// <param name="newState">The new state.</param>
void GameLogic::ChangeState(SimBinGameState newState)
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
void GameLogic::UpdateLeader()
{
	///TODO Pending to implement
}
/// <summary>
/// Binding for getting lives.
/// </summary>
/// <returns></returns>
std::string GameLogic::BindingGetLives()
{
	std::ostringstream convert;   // stream used for the conversion

	uint16_t seconds = (m_Lives / 1000);
	uint16_t minutes = seconds / 60;

	seconds = seconds % 60;
	uint16_t miliseconds = m_Lives % 1000;

	convert << minutes << "::";
	if (seconds < 10)
	{
		convert << "0";
		convert << seconds;
	}
	else
	{
		convert << seconds;
	}
	convert << "::";
	if (miliseconds < 100)
	{
		convert << "0";
	}
	if (miliseconds < 10)
	{
		convert << "0";
	}
	convert << miliseconds;

	return convert.str().c_str();
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

	return false;

}