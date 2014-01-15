#pragma once
#include <System\ComponentManager.h>
#include <System\StdLibraries.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\GameOptions.h>
#include <Logic\SimBinVadersLogic.h>
#include <GameView\RenderSystem.h>
#include <Collision\PhysicsSystem.h>
#include <System\FileSystemReader.h>

//namespaces
using namespace AI;
using namespace Graphics;
using namespace Collision;

namespace GameSystem
{

//typedefs and forward declarations
	typedef std::unordered_map<uint32_t, std::shared_ptr<Entity> > EntityMap;

//typedefs and forward declarations
	typedef std::unordered_map<uint32_t, std::shared_ptr<Entity> > PrototypeMap;


/// <summary>
/// Entity System that is in charge of storing the entitys, provide accessibility for the entities and receive entities related events
/// </summary>
class EntitySystem
{

protected:

	EntityMap m_EntityMap;
	PrototypeMap m_PrototypeMap;
	EventListenerPtr m_pEntityListener;			// Entity event listener
	uint32_t m_LastActorId;
	bool m_RenderDiagnostics;						// Are we rendering diagnostics?
	std::unique_ptr<ComponentManager> m_ComponentManager;
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
	EntitySystem();

	EntitySystem(std::unique_ptr<ComponentManager> componentManager);

	EntitySystem(EntitySystem&& other);

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
	void InitGame(const char *entitiesFile);

	/// <summary>
	/// Creates an entity.
	/// </summary>
	std::shared_ptr<Entity> CreateEntity(const char * entityName);

protected:
	/// <summary>
	/// Creates a prototype.
	/// </summary>
	std::shared_ptr<Entity> CreatePrototype(JSONNode * descriptor);

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
