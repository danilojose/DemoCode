#pragma once
#include <System\StdLibraries.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\GameOptions.h>
#include <Logic\SimBinVadersLogic.h>
#include <GameView\RenderSystem.h>
#include <Collision\CollisionSystem.h>

//namespaces
using namespace AI;
using namespace Graphics;
using namespace Collision;

namespace GameSystem
{

//typedefs and forward declarations
typedef std::map<uint32_t, std::shared_ptr<Entity> > EntityMap;


/// <summary>
/// Entity System that is in charge of storing the entitys, provide accessibility for the entities and receive entities related events
/// </summary>
class EntitySystem
{

protected:

	EntityMap m_EntityMap;
	EventListenerPtr m_pEntityListener;			// Collision event listener
	uint32_t m_LastActorId;
	bool m_RenderDiagnostics;						// Are we rendering diagnostics?
	std::shared_ptr<AI::SimBinGameLogic> m_pGame; //The AI GameLogic
	std::shared_ptr<RenderSystem> m_pRenderSystem; //The RenderSystem
	std::shared_ptr<CollisionSystem> m_pCollisionSystem; //The Collision system
	std::shared_ptr<GameOptions> m_pGameOptions; //The GameOptions

	/// <summary>
	/// Gets the new actor identifier.
	/// </summary>
	/// <returns></returns>
	uint32_t GetNewActorID(void)
	{
		return ++m_LastActorId;
	}

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="EntitySystem"/> class.
	/// </summary>
	/// <param name="game">The game.</param>
	/// <param name="renderSystem">The render system.</param>
	/// <param name="collisionSystem">The collision system.</param>
	/// <param name="gameOptions">The game options.</param>
	explicit EntitySystem(std::shared_ptr<SimBinGameLogic> game, std::shared_ptr<RenderSystem> renderSystem, std::shared_ptr<CollisionSystem> collisionSystem, std::shared_ptr<GameOptions> gameOptions);

	/// <summary>
	/// Finalizes an instance of the <see cref="EntitySystem"/> class.
	/// </summary>
	virtual ~EntitySystem()
	{}
	

	/// <summary>
	/// Toggles the pause.
	/// </summary>
	/// <param name="active">The active.</param>
	void TogglePause(bool active);
	/// <summary>
	/// Adds the entity.
	/// </summary>
	/// <param name="entity">The entity.</param>
	virtual void AddEntity(std::shared_ptr<Entity> entity);
	/// <summary>
	/// Gets the entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns></returns>
	virtual std::shared_ptr<Entity> GetEntity(const uint32_t id);
	/// <summary>
	/// Removes the entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	virtual void RemoveEntity(uint32_t id);
	/// <summary>
	/// Called when It is required to update entity
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	/// <returns></returns>
	virtual bool OnUpdate(uint32_t deltaMilliseconds);

	/// <summary>
	/// Initializes the game.
	/// </summary>
	void InitGame();
	/// <summary>
	/// Creates the player.
	/// </summary>
	void CreatePlayer();
	/// <summary>
	/// Creates the spawn.
	/// </summary>
	void CreateSpawn();

	/// <summary>
	/// Creates the player fire.
	/// </summary>
	/// <param name="initialPositionX">The initial position x.</param>
	/// <param name="initialPositionY">The initial position y.</param>
	void CreatePlayerFire(uint32_t initialPositionX, uint32_t initialPositionY);
	/// <summary>
	/// Creates the enemy fire.
	/// </summary>
	/// <param name="initialPositionX">The initial position x.</param>
	/// <param name="initialPositionY">The initial position y.</param>
	void CreateEnemyFire(uint32_t initialPositionX,uint32_t initialPositionY);
	/// <summary>
	/// Creates the explosion.
	/// </summary>
	/// <param name="initialPositionX">The initial position x.</param>
	/// <param name="initialPositionY">The initial position y.</param>
	/// <param name="parentId">The parent identifier.</param>
	/// <param name="actorType">Type of the actor.</param>
	void CreateExplosion(uint32_t initialPositionX, uint32_t initialPositionY,uint32_t parentId,ActorType actorType);

};


/// <summary>
/// Listener to Entity Events
/// </summary>
class EntitySystemListener : public IEventListener
{
	EntitySystem *m_EntitySystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="EntitySystemListener"/> class.
	/// </summary>
	/// <param name="entitySystem">The entity system.</param>
	explicit EntitySystemListener(EntitySystem * entitySystem) : IEventListener() { m_EntitySystem = entitySystem; }
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns></returns>
	virtual char const* GetName(void) const override { return "EntitySystemListener"; }
	/// <summary>
	/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
	/// </summary>
	/// <param name="event">The event.</param>
	/// <returns></returns>
	virtual bool HandleEvent(IEventData const & event) const override;
};
}
