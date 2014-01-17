#pragma once
#include <System\StdLibraries.h>
#include <Logic\IBehaviourComponent.h>
#include <System\Events.h>
#include <System\EntityEvents.h>
#include <System\EventManager.h>
#include <System\FileSystemReader.h>
#include <random>
///namespaces
namespace GameSystem
{
	class World;
}
// Typedef declarations and forward declarations.

namespace AI
{
// Typedef declarations and forward declarations.
	typedef std::unordered_map<uint32_t, std::shared_ptr<IBehaviourComponent> > BehaviourMap;
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
class GameLogic 
{

protected:
	BehaviourMap							m_BehaviourList;
	SimBinGameState							m_State;			// game state: loading, running, etc.
	EventListenerPtr						m_pAiEventListener;	// AI event listener
	uint16_t								m_Score;						
	uint32_t								m_Lives;
	bool									m_bProxy;			// set if this is a proxy game logic, not a real one
	int										m_remotePlayerId;	// if we are a remote player - what is out socket number on the server
	uint32_t								m_EnemyLeader;				
	bool									m_RenderDiagnostics;	// Are we rendering diagnostics?
	bool									m_ContinueRunning;
	std::shared_ptr<GameSystem::World>		m_pWorld;
	std::mt19937_64							m_RandomNumber;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GameLogic"/> class.
	/// </summary>
	/// <param name="lives">The lives.</param>
	/// <param name="world">The world the entities live. AI needs to query the world.</param>
	GameLogic();
	/// <summary>
	/// Finalizes an instance of the <see cref="GameLogic"/> class.
	/// </summary>
	virtual ~GameLogic();
	/// <summary>
	/// Toggles the pause.
	/// </summary>
	/// <param name="active">The active.</param>
	void TogglePause(bool active);
	/// <summary>
	/// Resets the instance
	/// </summary>
	virtual void Reset(uint32_t lives);
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
	virtual const uint32_t GetCurrentLives() const { return m_Lives; }
	/// <summary>
	/// Sets the current lives.
	/// </summary>
	/// <param name="lives">The lives.</param>
	virtual void SetCurrentLives(uint32_t lives) { m_Lives = lives; }

	/// <summary>
	/// Aborts the game.
	/// </summary>
	virtual void AbortGame() { m_ContinueRunning = false; }

	/// <summary>
	/// Gets World
	/// </summary>
	virtual inline std::shared_ptr<World> GetWorld() { return m_pWorld; }

	/// <summary>
	/// Gets World
	/// </summary>
	virtual inline void  SetWorld(std::shared_ptr<World> world) { m_pWorld=world; }


	/// <summary>
	/// Gets the random engine.
	/// </summary>
	/// <returns></returns>
	virtual inline std::mt19937_64 & GetRandomEngine()
	{
		return m_RandomNumber;
	}


	///////////Bindings//////////
	/// <summary>
	/// Gets the score.
	/// </summary>
	/// <returns></returns>
	std::string BindingGetScore() { return std::to_string(m_Score); }

	/// <summary>
	/// Binding for getting lives.
	/// </summary>
	/// <returns></returns>
	std::string BindingGetLives();
};


/// <summary>
/// Event Listener for the events that involves the AI System
/// </summary>
class AiEventListener : public IEventListener
{
	GameLogic*										m_LogicSystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="AiEventListener"/> class.
	/// </summary>
	/// <param name="logicSystem">The logic system.</param>
	explicit AiEventListener(GameLogic * logicSystem) : IEventListener() { m_LogicSystem = logicSystem; }
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