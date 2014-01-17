#include <Physics\PhysicsSystem.h>
#include <GameView\AnimatedSpritesComponent.h>
#include <GameView\StaticAnimatedSpritesComponent.h>
#include <Logic\FireBehaviour.h>
#include <Logic\EmptyBehaviour.h>
#include <Logic\EnemyBehaviour.h>
#include <Logic\UserControlledBehaviour.h>
#include <System\EntitySystem.h>
#include <GameView\RenderSystem.h>
#include <Logic\GameLogic.h>
#include <System\Assert.h>


//namespaces
using namespace GameSystem;
using namespace AI;
using namespace Physics;
using namespace Graphics;

//typedefs and forward declarations

const uint32_t PLAYER_SPEED = 20;
const uint32_t PLAYER_FIRERATE = 500;
const uint32_t PLAYER_LIVES = 500;
const uint32_t PLAYER_POSX = 320;
const uint32_t PLAYER_POSY = 700;
const uint32_t PLAYERFIRE_SPEED = 2;
const uint32_t SPAWN_NUMBEROFROWS = 4;
const uint32_t SPAWN_SPEEDX = 5;
const uint32_t SPAWN_SPEEDY = 50;
const uint32_t SPAWN_NUMBEROFENEMIESBYROW = 5;
const uint32_t SPAWN_INITIALPOSITIONX = 100;
const uint32_t SPAWN_INITIALPOSITIONY = 100;
const uint32_t SPAWN_NUMBEROFENEMYKINDS = 4;


EntitySystem * g_pEntitySystem;

/// <summary>
/// Initializes a new instance of the <see cref="EntitySystem"/> class.
/// </summary>
EntitySystem::EntitySystem()
{
	m_ComponentManager = std::unique_ptr<ComponentManager>(GCC_NEW ComponentManager());
	m_pEntityListener = EventListenerPtr(GCC_NEW EntitySystemListener(this));
	g_pEntitySystem = this;
}

/// <summary>
/// Initializes a new instance of the <see cref="EntitySystem"/> class.
/// </summary>
EntitySystem::EntitySystem(std::unique_ptr<ComponentManager> componentManager) : m_LastActorId(0)
{
	m_ComponentManager = std::move(componentManager);
	m_pEntityListener = EventListenerPtr(GCC_NEW EntitySystemListener(this));
	g_pEntitySystem = this;
}
EntitySystem::EntitySystem(EntitySystem&& other)
{

	m_ComponentManager = std::unique_ptr<ComponentManager>(std::move(other.m_ComponentManager));
	m_pEntityListener = EventListenerPtr(GCC_NEW EntitySystemListener(this));
	g_pEntitySystem = this;
}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void EntitySystem::TogglePause(bool active)
{
	//TODO pending to implement
}
/// <summary>
/// Adds the entity.
/// </summary>
/// <param name="entity">The entity.</param>
void EntitySystem::AddEntity(std::shared_ptr<Entity> entity)
{
	ASSERT_DESCRIPTION(entity->GetID() > 0, "Attempted to add an actor with no valid ID")

	m_EntityMap[entity->GetID()] = entity;

}
/// <summary>
/// Gets the entity.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<Entity> EntitySystem::GetEntity(const uint32_t id)
{
	auto it = m_EntityMap.find(id);
	if (it == m_EntityMap.end())
	{
		std::shared_ptr<Entity> null;
		return null;
	}
	return it->second;

}
/// <summary>
/// Removes the entity.
/// </summary>
/// <param name="id">The identifier.</param>
void EntitySystem::RemoveEntity(uint32_t id)
{
	std::shared_ptr<Entity> actor = GetEntity(id);
	if (actor)
	{
		m_EntityMap.erase(id);
	}
	else
	{
		ASSERT_DESCRIPTION(false, _T("UNKNOWN ACTOR") + id);
	}
}

// Entity Update
/// <summary>
/// Called when It is required to update entity
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
/// <returns></returns>
bool EntitySystem::OnUpdate(uint32_t deltaMilliseconds)
{
	return true;
}

/// <summary>
/// Initializes the game.
/// </summary>
void EntitySystem::InitGame(const char *entitiesFile)
{

	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreatePlayerFire::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreateEnemyFire::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreateEnemyExplosion::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreatePlayerExplosion::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_DestroyActor::sk_EventType);

	m_ComponentManager->Init();
	// Reset the entities available
	m_EntityMap.clear();

	FileSystemReader f(entitiesFile);
	JSONFileSystemParser parser(f.GetContents());
	parser.Init();
	const std::vector<JSONNode *> entitiesVector=parser.GetNode("Entities")->GetArrayValues();
	for (const auto & entityDescriptor : entitiesVector)
	{
		std::shared_ptr<Entity> protoType = CreatePrototype(entityDescriptor);
		m_PrototypeMap[HashedString(protoType->GetEntityName()).getHashValue()] = protoType;
	}
}

/// <summary>
/// Creates the CreatePrototype.
/// </summary>
std::shared_ptr<Entity> EntitySystem::CreatePrototype(JSONNode * descriptor)
{
	uint32_t actorId = GetNewActorID();


	uint32_t initialPositionX = descriptor->GetUInteger("PosX");
	uint32_t initialPositionY = descriptor->GetUInteger("PosY");
	const char* entityName = descriptor->GetString("Name");

	std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, HashedString(entityName)));

	const std::vector<JSONNode *> componentsVector = descriptor->GetChild("components")->GetArrayValues();
	for (const auto& componentDescriptor : componentsVector)
	{
		std::shared_ptr<IComponent> component = m_ComponentManager->GetNewComponent(componentDescriptor->GetString("Name"), componentDescriptor);
		entity->AddComponent(component);
	}

	return entity;
}

/// <summary>
/// Creates the CreatePrototype.
/// </summary>
std::shared_ptr<Entity> EntitySystem::CreateEntity(const char * entityName)
{
	std::shared_ptr<Entity> protoType = m_PrototypeMap[HashedString(entityName).getHashValue()];

	std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(GCC_NEW Entity(GetNewActorID(), protoType->GetPosX(), protoType->GetPosY(), protoType->GetEntityName()));

	for (const auto &component : protoType->GetComponents())
	{
		entity->AddComponent(component->Clone(entity.get()));
	}

	AddEntity(entity);
	return entity;
}

/// <summary>
/// Loads Level
/// </summary>
void LoadLevel(const char *levelfile)
{
	
}

/// <summary>
/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool EntitySystemListener::HandleEvent(IEventData const & event) const
{

	EventType eventType = event.GetEventType();

	if (eventType == EvtData_CreatePlayerFire::sk_EventType)
	{
		EvtData_CreatePlayerFire const & ed = static_cast< const EvtData_CreatePlayerFire & >(event);
	}

	else if (eventType == EvtData_CreateEnemyFire::sk_EventType)
	{
		EvtData_CreateEnemyFire const & ed = static_cast< const EvtData_CreateEnemyFire & >(event);
	}

	else if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_CreatePlayerFire const & ed = static_cast< const EvtData_CreatePlayerFire & >(event);
		std::shared_ptr<Entity> entity=m_EntitySystem->GetEntity(ed.m_id);
		if (entity != nullptr)
		{

			if (entity->GetEntityName() == HashedString("Player"))
			{
				m_EntitySystem->InitGame("settings\\entities.json");
			}
			else
			{

				m_EntitySystem->RemoveEntity(ed.m_id);
			}
		}
	}


	return true;
}
