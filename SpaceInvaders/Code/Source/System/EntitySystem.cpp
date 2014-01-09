#include <Collision\CollisionSystem.h>
#include <GameView\AnimatedSpritesComponent.h>
#include <GameView\StaticAnimatedSpritesComponent.h>
#include <Logic\FireBehaviour.h>
#include <Logic\EmptyBehaviour.h>
#include <Logic\EnemyBehaviour.h>
#include <Logic\UserControlledBehaviour.h>
#include <System\EntitySystem.h>
#include <GameView\RenderSystem.h>
#include <Logic\SimBinVadersLogic.h>
#include <System\Assert.h>


//namespaces
using namespace GameSystem;
using namespace AI;
using namespace Collision;
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

/// <summary>
/// Initializes a new instance of the <see cref="EntitySystem"/> class.
/// </summary>
/// <param name="game">The game.</param>
/// <param name="renderSystem">The render system.</param>
/// <param name="collisionSystem">The collision system.</param>
/// <param name="gameOptions">The game options.</param>
EntitySystem::EntitySystem(std::shared_ptr<SimBinGameLogic> game, std::shared_ptr<RenderSystem> renderSystem, std::shared_ptr<CollisionSystem> collisionSystem, std::shared_ptr<GameOptions> gameOptions):
						  m_pGame(game), m_pRenderSystem(renderSystem), m_pCollisionSystem(collisionSystem), m_pGameOptions(gameOptions), m_LastActorId(0)
{
	m_ComponentManager = std::unique_ptr <ComponentManager>( GCC_NEW ComponentManager());
	m_pEntityListener = EventListenerPtr(GCC_NEW EntitySystemListener(this));
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreatePlayerFire::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreateEnemyFire::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreateEnemyExplosion::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_CreatePlayerExplosion::sk_EventType);
	IEventManager::Get()->VAddListener(m_pEntityListener, EvtData_DestroyActor::sk_EventType);
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
	for (auto& e : m_EntityMap)
	{
		if (e.second->GetType() == ActorType::AT_ALIEN1 || e.second->GetType() == ActorType::AT_ALIEN2 || e.second->GetType() == ActorType::AT_ALIEN3 || e.second->GetType() == ActorType::AT_ALIEN4)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// Initializes the game.
/// </summary>
void EntitySystem::InitGame()
{
	m_ComponentManager->Init();
	// Reset the entities available
	m_EntityMap.clear();
	m_pCollisionSystem->Reset();
	m_pRenderSystem->Reset();
	m_pGame->Reset();

	CreatePlayer();
	CreateSpawn();


}
/// <summary>
/// Creates the player.
/// </summary>
void EntitySystem::CreatePlayer()
{
	const IniValuesMap& playerOptions = m_pGameOptions->GetValuesForAGivenKey("Player");
	uint32_t actorId = GetNewActorID();

	uint32_t initialPositionX = PLAYER_POSX;
	uint32_t initialPositionY = PLAYER_POSY;

	if (playerOptions.find("InitialPositionX") != playerOptions.end())
	{
		initialPositionX = std::strtoul(playerOptions.at("InitialPositionX").c_str(), NULL, 0);
	}
	if (playerOptions.find("InitialPositionY") != playerOptions.end())
	{
		initialPositionY = std::strtoul(playerOptions.at("InitialPositionY").c_str(), NULL, 0);
	}

	std::shared_ptr<Entity> player = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, ActorType::AT_PLAYER));

	std::shared_ptr<IComponent> behaviour= m_ComponentManager->GetNewComponent("UserControlledBehaviour",playerOptions);
	player->AddComponent(behaviour);
	m_pGame->AddBehaviour(std::dynamic_pointer_cast<IBehaviourComponent>(behaviour));


	std::shared_ptr<IComponent> renderComponent = m_ComponentManager->GetNewComponent("AnimatedSpritesComponent", playerOptions);
	player->AddComponent(renderComponent);
	m_pRenderSystem->AddRenderEntity(std::dynamic_pointer_cast<IGraphicsComponent>(renderComponent));

	std::shared_ptr<ICollisionComponent> collisionEntity = std::dynamic_pointer_cast<ICollisionComponent>(m_ComponentManager->GetNewComponent("ICollisionComponent", playerOptions));
	player->AddComponent(collisionEntity);
	m_pCollisionSystem->AddCollisionEntity(collisionEntity, ActorType::AT_PLAYER);

	AddEntity(player);
}
/// <summary>
/// Creates the spawn.
/// </summary>
void EntitySystem::CreateSpawn()
{
	const IniValuesMap &spawnOptions			= m_pGameOptions->GetValuesForAGivenKey("Spawn");
	uint32_t numberOfRows				= SPAWN_NUMBEROFROWS;
	uint32_t speedX						= SPAWN_SPEEDX;
	uint32_t speedY						= SPAWN_SPEEDY;
	uint32_t numberOfEnemiesByRows		= SPAWN_NUMBEROFENEMIESBYROW;
	uint32_t initialPositionX			= SPAWN_INITIALPOSITIONX;
	uint32_t initialPositionY			= SPAWN_INITIALPOSITIONY;
	uint32_t numberOfEnemyKinds			= SPAWN_NUMBEROFENEMYKINDS;

	if (spawnOptions.find("NumberOfRows")!=spawnOptions.end())
	{
		numberOfRows = std::strtoul(spawnOptions.at("NumberOfRows").c_str(),NULL, 0);
	}
	if (spawnOptions.find("SpeedX")!=spawnOptions.end())
	{
		speedX = std::strtoul(spawnOptions.at("SpeedX").c_str(), NULL, 0);
	}
	if (spawnOptions.find("SpeedY")!=spawnOptions.end())
	{
		speedY = std::strtoul(spawnOptions.at("SpeedY").c_str(), NULL, 0);
	}

	if (spawnOptions.find("NumberOfEnemiesByRow")!=spawnOptions.end())
	{
		numberOfEnemiesByRows = std::strtoul(spawnOptions.at("NumberOfEnemiesByRow").c_str(), NULL, 0);
	}
	if (spawnOptions.find("InitialPositionX")!=spawnOptions.end())
	{
		initialPositionX = std::strtoul(spawnOptions.at("InitialPositionX").c_str(), NULL, 0);
	}
	if (spawnOptions.find("InitialPositionY")!=spawnOptions.end())
	{
		initialPositionY = std::strtoul(spawnOptions.at("InitialPositionY").c_str(), NULL, 0);
	}

	ASSERT_DESCRIPTION(spawnOptions.find("FireSound") != spawnOptions.end(), "Number of Sprites not found for the given kind");
	const std::string  fireSound = spawnOptions.at("FireSound");

	//TODO Chequear numberOfEnemyKinds menor que maxnumberOfEnemyKinds
	for (uint32_t i = 0; i < numberOfEnemyKinds; ++i)
	{
		std::stringstream kind;
		kind <<"Kind["<<i<<"]";

		//TODO Asegurar que numberOfSprites esté bien. lo que devuelva spawnoptions debe estar correcto o tener algún IsValid() .
		
		ASSERT_DESCRIPTION(spawnOptions.find(kind.str()+".NumberOfSprites")!=spawnOptions.end(), "Number of Sprites not found for the given kind");
		uint32_t numberOfSprites = std::strtoul(spawnOptions.at(kind.str() + ".NumberOfSprites").c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".IdleSprite") != spawnOptions.end(), "Idle Sprite not found for the given kind");
		uint32_t idleSprite = std::strtoul(spawnOptions.at(kind.str() + ".IdleSprite").c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".CollisionRadius") != spawnOptions.end(), "CollisionRadius not found for the given kind");
		uint32_t collisionRadius = std::strtoul(spawnOptions.at(kind.str() + ".CollisionRadius").c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".Score") != spawnOptions.end(), "Score not found for the given kind");
		uint32_t points = std::strtoul(spawnOptions.at(kind.str() + ".Score").c_str(), NULL, 0);

		std::vector<std::string> sprites;

		ActorType actorType = ActorType::AT_UNKNOWN;

		switch (i)
		{
			case 0:
				actorType = ActorType::AT_ALIEN1;
				break;
			case 1:
				actorType = ActorType::AT_ALIEN2;
				break;

			case 2:
				actorType = ActorType::AT_ALIEN3;
				break;

			case 3:
				actorType = ActorType::AT_ALIEN4;
				break;
			default:
				ASSERT_DESCRIPTION(false, "Unexpected Alien found, we only support 4 different types of Alien");

		}

		for (uint32_t j = 0; j < numberOfSprites; j++)
		{
			std::stringstream spriteFromKind;
			spriteFromKind << kind.str() << ".Sprite[" << j << "]";

			ASSERT_DESCRIPTION(spawnOptions.find(spriteFromKind.str())!=spawnOptions.end(), "Sprite Not Found in ini file!!");
			sprites.push_back(spawnOptions.at(spriteFromKind.str()));
		}

		// Defenderse contra una modificación de datos que haga que numberOfEnemiesByRows sea mil millones de filas.
		for (uint32_t j = 0; j < numberOfEnemiesByRows; j++)
		{
			uint32_t actorId = GetNewActorID();

			std::shared_ptr<Entity> enemy = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX + 100 * (j), initialPositionY + 100 * i, actorType));
			AddEntity(enemy);

			std::shared_ptr<IBehaviourComponent> behaviour = std::shared_ptr<IBehaviourComponent>(GCC_NEW EnemyBehaviour(enemy.get(),speedX, speedY,fireSound,points));
			m_pGame->AddBehaviour(behaviour);
			enemy->AddComponent(behaviour);

			std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW AnimatedSpritesComponent(enemy.get(), m_pRenderSystem.get(), sprites, idleSprite));
			m_pRenderSystem->AddRenderEntity(renderEntity);
			enemy->AddComponent(renderEntity);

			std::shared_ptr<ICollisionComponent> collisionEntity = std::shared_ptr<ICollisionComponent>(GCC_NEW ICollisionComponent(enemy.get(), collisionRadius));
			m_pCollisionSystem->AddCollisionEntity(collisionEntity, actorType);
			enemy->AddComponent(collisionEntity);

		}
	}


}

/// <summary>
/// Creates the player fire.
/// </summary>
/// <param name="initialPositionX">The initial position x.</param>
/// <param name="initialPositionY">The initial position y.</param>
void EntitySystem::CreatePlayerFire(uint32_t initialPositionX, unsigned initialPositionY)
{
	const IniValuesMap &playerFireOptions = m_pGameOptions->GetValuesForAGivenKey("PlayerFire");
	uint32_t actorId = GetNewActorID();

	std::shared_ptr<Entity> playerFire = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, ActorType::AT_PLAYERFIRE));

	std::shared_ptr<IComponent> behaviour = m_ComponentManager->GetNewComponent("FireBehaviour", playerFireOptions);
	playerFire->AddComponent(behaviour);
	m_pGame->AddBehaviour(std::dynamic_pointer_cast<IBehaviourComponent>(behaviour));

	std::shared_ptr<IComponent> renderComponent = m_ComponentManager->GetNewComponent("AnimatedSpritesComponent", playerFireOptions);
	playerFire->AddComponent(renderComponent);
	m_pRenderSystem->AddRenderEntity(std::dynamic_pointer_cast<IGraphicsComponent>(renderComponent));

	std::shared_ptr<ICollisionComponent> collisionEntity = std::dynamic_pointer_cast<ICollisionComponent>(m_ComponentManager->GetNewComponent("ICollisionComponent", playerFireOptions));
	playerFire->AddComponent(collisionEntity);
	m_pCollisionSystem->AddCollisionEntity(std::dynamic_pointer_cast<ICollisionComponent>(collisionEntity), ActorType::AT_PLAYERFIRE);

	AddEntity(playerFire);


}
/// <summary>
/// Creates the enemy fire.
/// </summary>
/// <param name="initialPositionX">The initial position x.</param>
/// <param name="initialPositionY">The initial position y.</param>
void EntitySystem::CreateEnemyFire(uint32_t initialPositionX, uint32_t initialPositionY)
{
	const IniValuesMap &enemyFireOptions = m_pGameOptions->GetValuesForAGivenKey("EnemyFire");
	uint32_t actorId = GetNewActorID();

	std::shared_ptr<Entity> enemyFire = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, ActorType::AT_ALIENFIRE));


	std::shared_ptr<IComponent> behaviour = m_ComponentManager->GetNewComponent("FireBehaviour", enemyFireOptions);
	enemyFire->AddComponent(behaviour);
	m_pGame->AddBehaviour(std::dynamic_pointer_cast<IBehaviourComponent>(behaviour));

	std::shared_ptr<IComponent> renderComponent = m_ComponentManager->GetNewComponent("AnimatedSpritesComponent", enemyFireOptions);
	enemyFire->AddComponent(renderComponent);
	m_pRenderSystem->AddRenderEntity(std::dynamic_pointer_cast<IGraphicsComponent>(renderComponent));

	std::shared_ptr<ICollisionComponent> collisionEntity = std::dynamic_pointer_cast<ICollisionComponent>(m_ComponentManager->GetNewComponent("ICollisionComponent", enemyFireOptions));
	enemyFire->AddComponent(collisionEntity);
	m_pCollisionSystem->AddCollisionEntity(std::dynamic_pointer_cast<ICollisionComponent>(collisionEntity), ActorType::AT_ALIENFIRE);

	AddEntity(enemyFire);
}

/// <summary>
/// Creates the explosion.
/// </summary>
/// <param name="initialPositionX">The initial position x.</param>
/// <param name="initialPositionY">The initial position y.</param>
/// <param name="parentId">The parent identifier.</param>
/// <param name="actorType">Type of the actor.</param>
void EntitySystem::CreateExplosion(uint32_t initialPositionX, uint32_t initialPositionY,uint32_t parentId,ActorType explosionType)
{
	const IniValuesMap &explosionOptions = m_pGameOptions->GetValuesForAGivenKey("Explosion");
	uint32_t actorId = GetNewActorID();

	std::shared_ptr<Entity> explosion = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, explosionType));
	std::shared_ptr<IComponent> renderComponent = m_ComponentManager->GetNewComponent("StaticAnimatedSpritesComponent", explosionOptions);
	explosion->AddComponent(renderComponent);
	m_pRenderSystem->AddRenderEntity(std::dynamic_pointer_cast<IGraphicsComponent>(renderComponent));
	AddEntity(explosion);
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
		m_EntitySystem->CreatePlayerFire(ed.m_PosX, ed.m_PosY);
	}

	else if (eventType == EvtData_CreateEnemyFire::sk_EventType)
	{
		EvtData_CreateEnemyFire const & ed = static_cast< const EvtData_CreateEnemyFire & >(event);
		m_EntitySystem->CreateEnemyFire(ed.m_PosX, ed.m_PosY);
	}

	else if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_CreatePlayerFire const & ed = static_cast< const EvtData_CreatePlayerFire & >(event);
		std::shared_ptr<Entity> entity=m_EntitySystem->GetEntity(ed.m_id);
		if (entity != nullptr)
		{
			if (entity->GetType() == ActorType::AT_PLAYEREXPLOSION)
			{
				m_EntitySystem->InitGame();
			}
			else
			{
				m_EntitySystem->RemoveEntity(ed.m_id);
			}
		}
	}

	else if (eventType == EvtData_CreateEnemyExplosion::sk_EventType)
	{
		EvtData_CreateEnemyExplosion const & ed = static_cast< const EvtData_CreateEnemyExplosion & >(event);
		m_EntitySystem->CreateExplosion(ed.m_PosX, ed.m_PosY, ed.m_id,ActorType::AT_ENEMYEXPLOSION);
	}

	else if (eventType == EvtData_CreatePlayerExplosion::sk_EventType)
	{
		EvtData_CreateEnemyExplosion const & ed = static_cast< const EvtData_CreateEnemyExplosion & >(event);
		m_EntitySystem->CreateExplosion(ed.m_PosX, ed.m_PosY, ed.m_id,ActorType::AT_PLAYEREXPLOSION);
	}

	return true;
}
