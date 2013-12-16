#include <System\EventManager.h>
#include <Collision\CollisionSystem.h>
#include <Collision\CollisionComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>


using namespace Collision;
CollisionSystem::CollisionSystem()
{
	m_pCollisionListener = EventListenerPtr(GCC_NEW CollisionSystemListener(this));
	IEventManager::Get()->VAddListener(m_pCollisionListener, EvtData_UpdatePosition::sk_EventType);
	IEventManager::Get()->VAddListener(m_pCollisionListener, EvtData_DestroyActor::sk_EventType);

}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void CollisionSystem::TogglePause(bool active)
{
	//TODO: Pending to implement
}

// Init
/// <summary>
/// Initializes this instance.
/// </summary>
void CollisionSystem::Reset()
{
	m_CollisionEntityList.clear();
}

/// <summary>
/// Adds the collision entity.
/// </summary>
/// <param name="renderEntity">The render entity.</param>
/// <param name="type">The type.</param>
void CollisionSystem::AddCollisionEntity(std::shared_ptr<ICollisionComponent> collisionEntity,ActorType type)
{
	ASSERT_DESCRIPTION(collisionEntity->GetOwner() > 0, "Attempted to add an actor with no valid ID")

	m_CollisionEntityList[collisionEntity->GetOwner()] = type;

	switch (type)
	{
		case AT_Player:
		case AT_PlayerFire:
			m_PlayerCollisionEntityList.push_back(collisionEntity);
			// We always sort the Player list according the X Element once we get a new entity added
			m_PlayerCollisionEntityList.sort([](std::shared_ptr<ICollisionComponent> a, std::shared_ptr<ICollisionComponent> b)
			{ return a->GetPosX() < b->GetPosX(); });
			break;
		case AT_Alien1:
		case AT_Alien2:
		case AT_Alien3:
		case AT_Alien4:
		case AT_AlienFire:
			m_EnemyCollisionEntityList.push_back(collisionEntity);
			// We always sort the Enemy list according the X Element once we get a new entity added
			m_EnemyCollisionEntityList.sort([](std::shared_ptr<ICollisionComponent> a, std::shared_ptr<ICollisionComponent> b)
			{ return a->GetPosX() < b->GetPosX(); });
			break;
		case AT_Unknown:
			ASSERT_DESCRIPTION(false, "AT_Unknown tried to be added");
	}

}
/// <summary>
/// Gets the collision entity.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<ICollisionComponent> CollisionSystem::GetCollisionEntity(const uint32_t id)
{
	auto it = m_CollisionEntityList.find(id);
	if (it == m_CollisionEntityList.end())
	{
		std::shared_ptr<ICollisionComponent> null;
		return nullptr;
	}
	else
	{
		if (it->second == AT_Player || it->second == AT_PlayerFire)
		{
			auto  entity = std::find_if(m_PlayerCollisionEntityList.begin(), m_PlayerCollisionEntityList.end(), [&](std::shared_ptr<ICollisionComponent> a){ return a->GetOwner() == id; });
			if (entity == m_PlayerCollisionEntityList.end())
			{
				return nullptr;
			}
			else
			{
				return *entity;
			}
		}
		else
		{
			auto  entity = std::find_if(m_EnemyCollisionEntityList.begin(), m_EnemyCollisionEntityList.end(), [&](std::shared_ptr<ICollisionComponent> a){ return a->GetOwner() == id; });
			if (entity == m_EnemyCollisionEntityList.end())
			{
				return nullptr;
			}
			else
			{
				return *entity;
			}

		}
	}
	//TODO : Pending to implement
	return nullptr;

}
/// <summary>
/// Vs the remove collision entity.
/// </summary>
/// <param name="id">The identifier.</param>
void CollisionSystem::RemoveCollisionEntity(uint32_t id)
{
	auto it = m_CollisionEntityList.find(id);
	if (it != m_CollisionEntityList.end())
	{
		switch (it->second)
		{
		case AT_Player:
		case AT_PlayerFire:
			m_PlayerCollisionEntityList.remove_if([&](std::shared_ptr<ICollisionComponent> a) { return a->GetOwner() == id; });
			break;
		case AT_Alien1:
		case AT_Alien2:
		case AT_Alien3:
		case AT_Alien4:
		case AT_AlienFire:
			m_EnemyCollisionEntityList.remove_if([&](std::shared_ptr<ICollisionComponent> a) { return a->GetOwner() == id; });
			break;
		case AT_Unknown:
			ASSERT_DESCRIPTION(false, "AT_Unknown tried to be removed");
		}
	}
	return;
}

/// <summary>
/// Called when an update happens. This function is the one in charge of detecting the collisions
/// By now we are having a brute force algorithm to detect the collisions. The only optimizations is that we do not waste time detecting collisions between enemies or between players.
/// because the game is currently single player.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
/// <returns></returns>
bool CollisionSystem::OnUpdate(uint32_t deltaMilliseconds)
{
	// I only check the collisions between the players entities and the enemy entities. I do not consider friendly fire.
	for (auto& playerEntity : m_PlayerCollisionEntityList)
	{
		for (auto& enemyEntity : m_EnemyCollisionEntityList)
		{
			
			if ((abs((int)(playerEntity->GetPosX() - enemyEntity->GetPosX())) <= (int)playerEntity->GetSize())
				||
				(abs((int)(playerEntity->GetPosX() - enemyEntity->GetPosX())) <= (int)enemyEntity->GetSize()))
			{
				if ((abs((int)(playerEntity->GetPosY() - enemyEntity->GetPosY())) <= (int)playerEntity->GetSize())
					||
					(abs((int)(playerEntity->GetPosY() - enemyEntity->GetPosY())) <= (int)enemyEntity->GetSize()))

					{
						auto it = m_CollisionEntityList.find(playerEntity->GetOwner());
						if (it->second == AT_PlayerFire)
						{
							IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_EnemyCollisionDetected(playerEntity->GetOwner(), enemyEntity->GetOwner())));
						}
						else
						{
							IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlayerCollisionDetected(playerEntity->GetOwner(), enemyEntity->GetOwner())));
						}

					}
			}

		}

	}
	return true;
}

/// <summary>
/// Handles the event to be received... This is the function that checks what should be done when an event is received.
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool CollisionSystemListener::HandleEvent(IEventData const & event) const
{

	EventType eventType = event.GetEventType();

	if (eventType == EvtData_UpdatePosition::sk_EventType)
	{
		EvtData_UpdatePosition const & ed = static_cast< const EvtData_UpdatePosition & >(event);
		std::shared_ptr<ICollisionComponent> entity = m_CollisionSystem->GetCollisionEntity(ed.m_ActorId);
		if (entity)
		{
			entity->UpdatePosition(ed.m_PosX, ed.m_PosY);
			return true;
		}
		else
		{
			return false;
		}
	}

	else if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_DestroyActor const & ed = static_cast< const EvtData_DestroyActor & >(event);
		m_CollisionSystem->RemoveCollisionEntity(ed.m_ActorId);
		return true;
	}
	return false;
}