#include <Logic\UserControlledBehaviour.h>
#include <Logic\FireBehaviour.h>
#include <Logic\EmptyBehaviour.h>
#include <Logic\EnemyBehaviour.h>
#include <GameView\AnimatedSpritesComponent.h>
#include <GameView\StaticAnimatedSpritesComponent.h>
#include <System\EntitySystem.h>
#include <GameView\RenderSystem.h>
#include <Collision\CollisionSystem.h>
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
m_pGame(game), m_pRenderSystem(renderSystem), m_pCollisionSystem(collisionSystem), m_pGameOptions(gameOptions)
{
	m_LastActorId = 0;
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
		if (e.second->GetType() == AT_Alien1 || e.second->GetType() == AT_Alien2 || e.second->GetType() == AT_Alien3 || e.second->GetType() == AT_Alien4)
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
	IniValuesMap playerOptions = m_pGameOptions->GetValuesForAGivenKey("Player");
	uint32_t actorId = GetNewActorID();
	//default values;
	uint32_t speed = PLAYER_SPEED;
	uint32_t fireRate = PLAYER_FIRERATE;

	if (playerOptions.find("Speed")!=playerOptions.end())
	{
		speed = std::strtoul(playerOptions["Speed"].c_str(),NULL,0);
	}
	if (playerOptions.find("FireRate") != playerOptions.end())
	{
		fireRate = std::strtoul(playerOptions["FireRate"].c_str(), NULL, 0);
	}

	ASSERT_DESCRIPTION(playerOptions.find("PosX")!=playerOptions.end(), "Initial Position X not found for Player");
	uint32_t initialPositionX = std::strtoul(playerOptions["PosX"].c_str(),NULL,0);

	ASSERT_DESCRIPTION(playerOptions.find("PosY")!=playerOptions.end(), "Initial Position Y not found for Player");
	uint32_t initialPositionY = std::strtoul(playerOptions["PosY"].c_str(), NULL, 0);

	ASSERT_DESCRIPTION(playerOptions.find("FireSound") != playerOptions.end(), "Number of Sprites not found for the given kind");
	const std::string  fireSound = playerOptions["FireSound"];


	std::shared_ptr<IBehaviourComponent> behaviour = std::shared_ptr<IBehaviourComponent>(GCC_NEW UserControlledBehaviour(actorId, initialPositionX, initialPositionY, speed, fireRate, fireSound));
	m_pGame->AddBehaviour(behaviour);

	ASSERT_DESCRIPTION(playerOptions.find("NumberOfSprites")!=playerOptions.end(), "Number of Sprites not found");
	uint32_t numberOfSprites = std::strtoul(playerOptions["NumberOfSprites"].c_str(), NULL, 0);

	ASSERT_DESCRIPTION(playerOptions.find("IdleSprite")!=playerOptions.end(), "Idle Sprite not found");
	uint32_t idleSprite = std::strtoul(playerOptions["IdleSprite"].c_str(), NULL, 0);


	std::vector<std::string> sprites;
	for (uint32_t i= 0; i < numberOfSprites; ++i)
	{
		std::stringstream sprite;
		sprite << "Sprite[" << i << "]";
		ASSERT_DESCRIPTION(playerOptions.find(sprite.str())!=playerOptions.end(), "Sprite not found for player");
		sprites.push_back(playerOptions[sprite.str()]);
	}


	std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW AnimatedSpritesComponent(actorId, initialPositionX, initialPositionY,m_pRenderSystem.get(),sprites, idleSprite));

	m_pRenderSystem->AddRenderEntity(renderEntity);

	ASSERT_DESCRIPTION(playerOptions.find("CollisionRadius")!=playerOptions.end(), "Collision Radius Not Found");
	uint32_t collisionRadius = std::strtoul(playerOptions["CollisionRadius"].c_str(), NULL, 0);

	std::shared_ptr<ICollisionComponent> collisionEntity = std::shared_ptr<ICollisionComponent>(GCC_NEW ICollisionComponent(actorId, initialPositionX + collisionRadius, initialPositionY + collisionRadius, collisionRadius));


	m_pCollisionSystem->AddCollisionEntity(collisionEntity, AT_Player);

	std::shared_ptr<Entity> player = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, AT_Player, behaviour,renderEntity,collisionEntity));

	AddEntity(player);
}
/// <summary>
/// Creates the spawn.
/// </summary>
void EntitySystem::CreateSpawn()
{
	IniValuesMap spawnOptions = m_pGameOptions->GetValuesForAGivenKey("Spawn");
	uint32_t numberOfRows=SPAWN_NUMBEROFROWS;
	uint32_t speedX=SPAWN_SPEEDX;
	uint32_t speedY=SPAWN_SPEEDY;
	uint32_t numberOfEnemiesByRows=SPAWN_NUMBEROFENEMIESBYROW;
	uint32_t initialPositionX=SPAWN_INITIALPOSITIONX;
	uint32_t initialPositionY=SPAWN_INITIALPOSITIONY;
	uint32_t numberOfEnemyKinds = SPAWN_NUMBEROFENEMYKINDS;

	if (spawnOptions.find("NumberOfRows")!=spawnOptions.end())
	{
		numberOfRows = std::strtoul(spawnOptions["NumberOfRows"].c_str(),NULL, 0);
	}
	if (spawnOptions.find("SpeedX")!=spawnOptions.end())
	{
		speedX = std::strtoul(spawnOptions["SpeedX"].c_str(), NULL, 0);
	}
	if (spawnOptions.find("SpeedY")!=spawnOptions.end())
	{
		speedY = std::strtoul(spawnOptions["SpeedY"].c_str(), NULL, 0);
	}

	if (spawnOptions.find("NumberOfEnemiesByRow")!=spawnOptions.end())
	{
		numberOfEnemiesByRows = std::strtoul(spawnOptions["NumberOfEnemiesByRow"].c_str(), NULL, 0);
	}
	if (spawnOptions.find("InitialPositionX")!=spawnOptions.end())
	{
		initialPositionX = std::strtoul(spawnOptions["InitialPositionX"].c_str(), NULL, 0);
	}
	if (spawnOptions.find("InitialPositionY")!=spawnOptions.end())
	{
		initialPositionY = std::strtoul(spawnOptions["InitialPositionY"].c_str(), NULL, 0);
	}

	ASSERT_DESCRIPTION(spawnOptions.find("FireSound") != spawnOptions.end(), "Number of Sprites not found for the given kind");
	const std::string  fireSound = spawnOptions["FireSound"];

	for (uint32_t i = 0; i < numberOfEnemyKinds; ++i)
	{
		std::stringstream kind;
		kind <<"Kind["<<i<<"]";

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str()+".NumberOfSprites")!=spawnOptions.end(), "Number of Sprites not found for the given kind");
		uint32_t numberOfSprites = std::strtoul(spawnOptions[kind.str() + ".NumberOfSprites"].c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".IdleSprite") != spawnOptions.end(), "Idle Sprite not found for the given kind");
		uint32_t idleSprite = std::strtoul(spawnOptions[kind.str() + ".IdleSprite"].c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".CollisionRadius") != spawnOptions.end(), "CollisionRadius not found for the given kind");
		uint32_t collisionRadius = std::strtoul(spawnOptions[kind.str() + ".CollisionRadius"].c_str(), NULL, 0);

		ASSERT_DESCRIPTION(spawnOptions.find(kind.str() + ".Score") != spawnOptions.end(), "Score not found for the given kind");
		uint32_t points = std::strtoul(spawnOptions[kind.str() + ".Score"].c_str(), NULL, 0);

		std::vector<std::string> sprites;

		ActorType actorType = AT_Unknown;

		switch (i)
		{
			case 0:
				actorType = AT_Alien1;
				break;
			case 1:
				actorType = AT_Alien2;
				break;

			case 2:
				actorType = AT_Alien3;
				break;

			case 3:
				actorType = AT_Alien4;
				break;
			default:
				ASSERT_DESCRIPTION(false, "Unexpected Alien found, we only support 4 different types of Alien");

		}

		for (uint32_t j = 0; j < numberOfSprites; j++)
		{
			std::stringstream spriteFromKind;
			spriteFromKind << kind.str() << ".Sprite[" << j << "]";

			ASSERT_DESCRIPTION(spawnOptions.find(spriteFromKind.str())!=spawnOptions.end(), "Sprite Not Found in ini file!!");
			sprites.push_back(spawnOptions[spriteFromKind.str()]);
		}


		for (uint32_t j = 0; j < numberOfEnemiesByRows; j++)
		{
			uint32_t actorId = GetNewActorID();

			std::shared_ptr<IBehaviourComponent> behaviour = std::shared_ptr<IBehaviourComponent>(GCC_NEW EnemyBehaviour(actorId, initialPositionX + 100 * (j), initialPositionY + 100 * i,speedX, speedY,fireSound,points));
			m_pGame->AddBehaviour(behaviour);

			std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW AnimatedSpritesComponent(actorId, initialPositionX + 100 * (j), initialPositionY + 100 * i, m_pRenderSystem.get(), sprites, idleSprite));
			m_pRenderSystem->AddRenderEntity(renderEntity);

			std::shared_ptr<ICollisionComponent> collisionEntity = std::shared_ptr<ICollisionComponent>(GCC_NEW ICollisionComponent(actorId, initialPositionX + +collisionRadius+100 * (j), initialPositionY +collisionRadius+ 100 * i, collisionRadius));

			m_pCollisionSystem->AddCollisionEntity(collisionEntity, actorType);

			std::shared_ptr<Entity> enemy = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX + 100 * (j), initialPositionY + 100 * i, actorType, behaviour,renderEntity,collisionEntity));
			AddEntity(enemy);
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
	IniValuesMap playerFireOptions = m_pGameOptions->GetValuesForAGivenKey("PlayerFire");
	uint32_t actorId = GetNewActorID();
	//default values;
	int speed = PLAYERFIRE_SPEED;

	if (playerFireOptions.find("Speed")!=playerFireOptions.end())
	{
		speed = std::strtol(playerFireOptions["Speed"].c_str(),NULL,0);
	}
	ASSERT_DESCRIPTION(playerFireOptions.find("CollisionRadius")!=playerFireOptions.end(), "Collision Radius Not Found");
	uint32_t collisionRadius = std::strtoul(playerFireOptions["CollisionRadius"].c_str(),NULL,0);

	std::shared_ptr<IBehaviourComponent> behaviour = std::shared_ptr<IBehaviourComponent>(GCC_NEW FireBehaviour(actorId,initialPositionX,initialPositionY,speed));
	m_pGame->AddBehaviour(behaviour);

	ASSERT_DESCRIPTION(playerFireOptions.find("NumberOfSprites")!=playerFireOptions.end(), "Number of Sprites not found");
	uint32_t numberOfSprites = std::strtoul(playerFireOptions["NumberOfSprites"].c_str(),NULL,0);

	ASSERT_DESCRIPTION(playerFireOptions.find("IdleSprite")!=playerFireOptions.end(), "Idle Sprite not found");
	uint32_t idleSprite = std::strtoul(playerFireOptions["IdleSprite"].c_str(),NULL,0);

	std::vector<std::string> sprites;
	for (uint32_t i = 0; i < numberOfSprites; ++i)
	{
		//TODO Use a _TCHAR somehow... this is ugly
		std::stringstream sprite;
		sprite <<"Sprite[" <<i<<"]";

		ASSERT_DESCRIPTION(playerFireOptions.find(sprite.str())!=playerFireOptions.end(), "Sprite not found for player fire");
		sprites.push_back(playerFireOptions[sprite.str()]);
	}


	std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW AnimatedSpritesComponent(actorId, initialPositionX, initialPositionY, m_pRenderSystem.get(),sprites, idleSprite));

	m_pRenderSystem->AddRenderEntity(renderEntity);

	std::shared_ptr<ICollisionComponent> collisionEntity = std::shared_ptr<ICollisionComponent>(GCC_NEW ICollisionComponent(actorId, initialPositionX + collisionRadius, initialPositionY + collisionRadius, collisionRadius));
	m_pCollisionSystem->AddCollisionEntity(collisionEntity, AT_PlayerFire);
	std::shared_ptr<Entity> playerFire = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, AT_PlayerFire, behaviour,renderEntity,collisionEntity));
	AddEntity(playerFire);


}
/// <summary>
/// Creates the enemy fire.
/// </summary>
/// <param name="initialPositionX">The initial position x.</param>
/// <param name="initialPositionY">The initial position y.</param>
void EntitySystem::CreateEnemyFire(uint32_t initialPositionX, uint32_t initialPositionY)
{
	IniValuesMap enemyFireOptions = m_pGameOptions->GetValuesForAGivenKey("EnemyFire");
	uint32_t actorId = GetNewActorID();
	//default values;
	int speed = PLAYERFIRE_SPEED;

	if (enemyFireOptions.find("Speed") != enemyFireOptions.end())
	{
		speed = std::strtol(enemyFireOptions["Speed"].c_str(), NULL, 0);
	}
	ASSERT_DESCRIPTION(enemyFireOptions.find("CollisionRadius") != enemyFireOptions.end(), "Collision Radius Not Found");
	uint32_t collisionRadius = std::strtoul(enemyFireOptions["CollisionRadius"].c_str(), NULL, 0);

	std::shared_ptr<IBehaviourComponent> behaviour = std::shared_ptr<IBehaviourComponent>(GCC_NEW FireBehaviour(actorId, initialPositionX, initialPositionY, speed));
	m_pGame->AddBehaviour(behaviour);

	ASSERT_DESCRIPTION(enemyFireOptions.find("NumberOfSprites") != enemyFireOptions.end(), "Number of Sprites not found");
	uint32_t numberOfSprites = std::strtoul(enemyFireOptions["NumberOfSprites"].c_str(), NULL, 0);

	ASSERT_DESCRIPTION(enemyFireOptions.find("IdleSprite") != enemyFireOptions.end(), "Idle Sprite not found");
	uint32_t idleSprite = std::strtoul(enemyFireOptions["IdleSprite"].c_str(), NULL, 0);

	std::vector<std::string> sprites;
	for (uint32_t i = 0; i < numberOfSprites; ++i)
	{
		//TODO Use a _TCHAR somehow... this is ugly
		std::stringstream sprite;
		sprite << "Sprite[" << i << "]";

		ASSERT_DESCRIPTION(enemyFireOptions.find(sprite.str()) != enemyFireOptions.end(), "Sprite not found for player fire");
		sprites.push_back(enemyFireOptions[sprite.str()]);
	}


	std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW AnimatedSpritesComponent(actorId, initialPositionX, initialPositionY,m_pRenderSystem.get(), sprites, idleSprite));

	m_pRenderSystem->AddRenderEntity(renderEntity);

	std::shared_ptr<ICollisionComponent> collisionEntity = std::shared_ptr<ICollisionComponent>(GCC_NEW ICollisionComponent(actorId, initialPositionX + collisionRadius, initialPositionY + collisionRadius, collisionRadius));
	m_pCollisionSystem->AddCollisionEntity(collisionEntity,AT_AlienFire);
	std::shared_ptr<Entity> enemyFire = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, AT_AlienFire, behaviour, renderEntity, collisionEntity));
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
	IniValuesMap explosionOptions = m_pGameOptions->GetValuesForAGivenKey("Explosion");
	uint32_t actorId = GetNewActorID();

	ASSERT_DESCRIPTION(explosionOptions.find("NumberOfSprites") != explosionOptions.end(), "Number of Sprites not found");
	uint32_t numberOfSprites = std::strtoul(explosionOptions["NumberOfSprites"].c_str(), NULL, 0);

	ASSERT_DESCRIPTION(explosionOptions.find("IdleSprite") != explosionOptions.end(), "Idle Sprite not found");
	uint32_t idleSprite = std::strtoul(explosionOptions["IdleSprite"].c_str(), NULL, 0);

	std::vector<std::string> sprites;
	for (uint32_t i = 0; i < numberOfSprites; ++i)
	{
		//TODO Use a _TCHAR somehow... this is ugly
		std::stringstream sprite;
		sprite << "Sprite[" << i << "]";

		ASSERT_DESCRIPTION(explosionOptions.find(sprite.str()) != explosionOptions.end(), "Sprite not found for player fire");
		sprites.push_back(explosionOptions[sprite.str()]);
	}


	std::shared_ptr<IGraphicsComponent> renderEntity = std::shared_ptr<IGraphicsComponent>(GCC_NEW StaticAnimatedSpritesComponent(actorId, initialPositionX, initialPositionY, m_pRenderSystem.get(),sprites, idleSprite));

	m_pRenderSystem->AddRenderEntity(renderEntity);

	std::shared_ptr<ICollisionComponent> nullCollision;
	std::shared_ptr<IBehaviourComponent> nullBehaviour;
	std::shared_ptr<Entity> explosion = std::shared_ptr<Entity>(GCC_NEW Entity(actorId, initialPositionX, initialPositionY, explosionType, nullBehaviour, renderEntity, nullCollision));
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

	if (eventType == EvtData_CreateEnemyFire::sk_EventType)
	{
		EvtData_CreateEnemyFire const & ed = static_cast< const EvtData_CreateEnemyFire & >(event);
		m_EntitySystem->CreateEnemyFire(ed.m_PosX, ed.m_PosY);
	}

	if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_CreatePlayerFire const & ed = static_cast< const EvtData_CreatePlayerFire & >(event);
		std::shared_ptr<Entity> entity=m_EntitySystem->GetEntity(ed.m_id);
		if (entity != nullptr)
		{
			if (entity->GetType() == AT_PlayerExplosion)
			{
				m_EntitySystem->InitGame();
			}
			else
			{
				m_EntitySystem->RemoveEntity(ed.m_id);
			}
		}
	}

	if (eventType == EvtData_CreateEnemyExplosion::sk_EventType)
	{
		EvtData_CreateEnemyExplosion const & ed = static_cast< const EvtData_CreateEnemyExplosion & >(event);
		m_EntitySystem->CreateExplosion(ed.m_PosX, ed.m_PosY, ed.m_id,AT_EnemyExplosion);
	}

	if (eventType == EvtData_CreatePlayerExplosion::sk_EventType)
	{
		EvtData_CreateEnemyExplosion const & ed = static_cast< const EvtData_CreateEnemyExplosion & >(event);
		m_EntitySystem->CreateExplosion(ed.m_PosX, ed.m_PosY, ed.m_id,AT_PlayerExplosion);
	}

	return true;
}
