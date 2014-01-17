#pragma once
#include <System\StdLibraries.h>
#include <Physics\CollisionComponent.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\Entity.h>

///Namespaces
using namespace GameSystem;

namespace Physics
{
// Typedef declarations and forward declarations.
enum Actortype;

typedef std::unordered_map<uint32_t, std::shared_ptr<ICollisionComponent>> CollisionComponentMap;

/// <summary>
/// System in charge of detecting collisions between our ingame collision components
/// </summary>
class PhysicsSystem
{

protected:
	CollisionComponentMap							m_CollisionEntityList;
	EventListenerPtr								m_pCollisionListener;			// Physics event listener
	std::shared_ptr<GameSystem::World>				m_pWorld; // world
	bool											m_RenderDiagnostics;						// Are we rendering diagnostics?

	/// <summary>
	/// Calculates the collision.
	/// </summary>
	/// <param name="playerEntity">The player entity.</param>
	/// <param name="enemyEntity">The enemy entity .</param>
	/// <returns></returns>
	bool  CalculateCollision(const std::shared_ptr<ICollisionComponent> &playerEntity, const std::shared_ptr<ICollisionComponent> &enemyEntity);

public:

	/// <summary>
	/// Initializes a new instance of the <see cref="PhysicsSystem" /> class.
	/// </summary>
	PhysicsSystem();
	/// <summary>
	/// Initializes a new instance of the <see cref="PhysicsSystem" /> class.
	/// </summary>
	PhysicsSystem(PhysicsSystem&& other);
	/// <summary>
	/// Finalizes an instance of the <see cref="PhysicsSystem"/> class.
	/// </summary>
	virtual ~PhysicsSystem() {}

	// Pausing
	/// <summary>
	/// Toggles the pause.
	/// </summary>
	/// <param name="active">The active.</param>
	void TogglePause(bool active);
	// Init
	/// <summary>
	/// Initializes this instance.
	/// </summary>
	void Reset();
	/// <summary>
	/// Adds the collision entity.
	/// </summary>
	/// <param name="collisionEntity">The render entity.</param>
	/// <param name="type">The type.</param>
	virtual void AddCollisionEntity(std::shared_ptr<ICollisionComponent> collisionEntity);
	/// <summary>
	/// Gets the collision entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns></returns>
	virtual std::shared_ptr<ICollisionComponent> GetCollisionEntity(const uint32_t id);

	/// <summary>
	/// Sets World
	/// </summary>
	/// <param name="world">The actual world.</param>
	/// <returns></returns>
	virtual inline void SetWorld(std::shared_ptr<World> world) { m_pWorld = world; }

	/// <summary>
	/// Gets World
	/// </summary>
	/// <returns> the actual world</returns>
	virtual inline std::shared_ptr<World> GetWorld() { return m_pWorld; }

	/// <summary>
	/// Vs the remove collision entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	virtual void RemoveCollisionEntity(uint32_t id);
	/// <summary>
	/// Called when an update happens. This function is the one in charge of detecting the collisions
	/// By now we are having a brute force algorithm to detect the collisions. The only optimizations is that we do not waste time detecting collisions between enemies or between players.
	/// because the game is currently single player.
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	/// <returns></returns>
	virtual bool OnUpdate(uint32_t deltaMilliseconds);
};


/// <summary>
/// Event Listener that receives notifications for the Physics System
/// </summary>
class PhysicsSystemListener : public IEventListener
{
	PhysicsSystem *m_PhysicsSystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="PhysicsSystemListener"/> class.
	/// </summary>
	/// <param name="PhysicsSystem">The collision system.</param>
	explicit PhysicsSystemListener(PhysicsSystem * PhysicsSystem) : IEventListener() { m_PhysicsSystem = PhysicsSystem; }
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns></returns>
	virtual char const* GetName(void) const override { return "PhysicsSystemListener"; }
	/// <summary>
	/// Called when an update happens. This function is the one in charge of detecting the collisions
	/// By now we are having a brute force algorithm to detect the collisions. The only optimizations is that we do not waste time detecting collisions between enemies or between players.
	/// because the game is currently single player.
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	/// <returns></returns>
	virtual bool HandleEvent(IEventData const & event) const override;
};

}