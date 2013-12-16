#pragma once
#include <System\StdLibraries.h>
#include <Logic\IBehaviourComponent.h>
#include <System\Events.h>
#include <System\EventManager.h>


///namespaces
using namespace GameSystem;

// Typedef declarations and forward declarations.

namespace AI
{
// Typedef declarations and forward declarations.
typedef std::map<uint32_t, std::shared_ptr<IBehaviourComponent> > BehaviourMap;
//
// BaseGameState Enum. In charge of knowing where are we currently in the game
//
enum SimBinGameState
{
	SB_Initializing,
	SB_MainMenu,
	SB_LoadingInGame,
	SB_Ingame
};

/// <summary>
/// AI System that is in charge of the current logic of the Game and takes care of the behaviour components
/// </summary>
class SimBinGameLogic 
{

protected:
	BehaviourMap m_BehaviourList;

	SimBinGameState m_State;							// game state: loading, running, etc.
	int m_ExpectedPlayers;							// how many local human players
	int m_ExpectedAI;								// how man AI players
	EventListenerPtr m_pAiEventListener;			// AI event listener

	uint16_t m_Score;
	uint16_t m_Lives;


	bool m_bProxy;									// set if this is a proxy game logic, not a real one
	int m_remotePlayerId;							// if we are a remote player - what is out socket number on the server

	uint32_t m_EnemyLeader;				

	bool m_RenderDiagnostics;						// Are we rendering diagnostics?

	bool m_ContinueRunning;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="SimBinGameLogic"/> class.
	/// </summary>
	/// <param name="lives">The lives.</param>
	SimBinGameLogic(uint32_t lives);
	/// <summary>
	/// Finalizes an instance of the <see cref="SimBinGameLogic"/> class.
	/// </summary>
	virtual ~SimBinGameLogic();
	/// <summary>
	/// Toggles the pause.
	/// </summary>
	/// <param name="active">The active.</param>
	void TogglePause(bool active);
	/// <summary>
	/// Reset this instance.
	/// </summary>
	virtual void Reset();
	/// <summary>
	/// Adds the behaviour.
	/// </summary>
	/// <param name="actor">The actor.</param>
	virtual void AddBehaviour(std::shared_ptr<IBehaviourComponent> actor);
	/// <summary>
	/// Gets the behaviour.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns></returns>
	virtual std::shared_ptr<IBehaviourComponent> GetBehaviour(const uint32_t id);
	/// <summary>
	/// Removes the behaviour.
	/// </summary>
	/// <param name="id">The identifier.</param>
	virtual void RemoveBehaviour(uint32_t id);

	/// <summary>
	/// Loads the game.
	/// </summary>
	/// <param name="gameName">Name of the game.</param>
	/// <returns></returns>
	virtual bool LoadGame(std::string gameName) { return true; }
	/// <summary>
	/// Called when Update is called from the mainloop
	/// </summary>
	/// <param name="elapsedTime">The elapsed time.</param>
	/// <returns></returns>
	virtual bool OnUpdate(uint32_t elapsedTime);
	// Changing Game Logic State
	/// <summary>
	/// Changes the state.
	/// </summary>
	/// <param name="newState">The new state.</param>
	virtual void ChangeState(SimBinGameState newState);
	/// <summary>
	/// Updates the current leader.
	/// </summary>	
	virtual void UpdateLeader();

	/// <summary>
	/// Gets the current score.
	/// </summary>
	/// <returns></returns>
	virtual const uint16_t GetCurrentScore() const { return m_Score; }
	/// <summary>
	/// Adds the score.
	/// </summary>
	/// <param name="score">The score.</param>
	/// <returns></returns>
	virtual uint16_t AddScore(uint16_t score) { m_Score += score; return m_Score; }

	/// <summary>
	/// Gets the current lives.
	/// </summary>
	/// <returns></returns>
	virtual const uint16_t GetCurrentLives() const { return m_Lives; }
	/// <summary>
	/// Sets the current lives.
	/// </summary>
	/// <param name="lives">The lives.</param>
	virtual void SetCurrentLives(uint16_t lives) { m_Lives = lives; }

	/// <summary>
	/// Aborts the game.
	/// </summary>
	virtual void AbortGame() { m_ContinueRunning = false; }


};


/// <summary>
/// Event Listener for the events that involves the AI System
/// </summary>
class AiEventListener : public IEventListener
{
	SimBinGameLogic* m_LogicSystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="AiEventListener"/> class.
	/// </summary>
	/// <param name="logicSystem">The logic system.</param>
	explicit AiEventListener(SimBinGameLogic * logicSystem) : IEventListener() { m_LogicSystem = logicSystem; }
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns></returns>
	virtual char const* GetName(void) const override { return "AiEventListener"; }
	/// <summary>
	/// Handles the event. React when the other systems trigger events that might be interesting for the AI.
	/// </summary>
	/// <param name="event">The event.</param>
	/// <returns></returns>
	virtual bool HandleEvent(IEventData const & event) const override;
};

}