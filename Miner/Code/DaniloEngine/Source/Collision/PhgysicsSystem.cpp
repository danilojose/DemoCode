#include <System\EventManager.h>
#include <Collision\PhysicsSystem.h>
#include <Collision\CollisionComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>


using namespace Collision;

PhysicsSystem::PhysicsSystem()
{
	m_pCollisionListener = EventListenerPtr(GCC_NEW PhysicsSystemListener(this));

}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void PhysicsSystem::TogglePause(bool active)
{
	//TODO: Pending to implement
}

// Init
/// <summary>
/// Initializes this instance.
/// </summary>
void PhysicsSystem::Reset()
{
	IEventManager::Get()->VAddListener(m_pCollisionListener, EvtData_DestroyActor::sk_EventType);
	m_CollisionEntityList.clear();
	m_PlayerCollisionEntityList.clear();
	m_EnemyCollisionEntityList.clear();
}

/// <summary>
/// Adds the collision entity.
/// </summary>
/// <param name="renderEntity">The render entity.</param>
/// <param name="type">The type.</param>
void PhysicsSystem::AddCollisionEntity(std::shared_ptr<ICollisionComponent> collisionEntity,ActorType type)
{
	ASSERT_DESCRIPTION(collisionEntity->GetEntity()!=nullptr&&collisionEntity->GetEntity()->GetID() > 0, "Attempted to add an actor with no valid ID")

	m_CollisionEntityList[collisionEntity->GetEntity()->GetID()] = type;

	switch (type)
	{
		case ActorType::AT_PLAYER:
		case ActorType::AT_PLAYERFIRE:
			m_PlayerCollisionEntityList.push_back(collisionEntity);
			// We always sort the Player list according the X Element once we get a new entity added
			m_PlayerCollisionEntityList.sort([](std::shared_ptr<ICollisionComponent> a, std::shared_ptr<ICollisionComponent> b)
			{ return a->GetEntity()->GetPosX() < b->GetEntity()->GetPosX(); });
			break;
		case ActorType::AT_ALIEN1:
		case ActorType::AT_ALIEN2:
		case ActorType::AT_ALIEN3:
		case ActorType::AT_ALIEN4:
		case ActorType::AT_ALIENFIRE:
			m_EnemyCollisionEntityList.push_back(collisionEntity);
			// We always sort the Enemy list according the X Element once we get a new entity added
			m_EnemyCollisionEntityList.sort([](std::shared_ptr<ICollisionComponent> a, std::shared_ptr<ICollisionComponent> b)
			{ return a->GetEntity()->GetPosX() < b->GetEntity()->GetPosX(); });
			break;
		case ActorType::AT_UNKNOWN:
			ASSERT_DESCRIPTION(false, "AT_Unknown tried to be added");
	}

}
/// <summary>
/// Gets the collision entity.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<ICollisionComponent> PhysicsSystem::GetCollisionEntity(const uint32_t id)
{
	auto it = m_CollisionEntityList.find(id);
	if (it == m_CollisionEntityList.end())
	{
		std::shared_ptr<ICollisionComponent> null;
		return nullptr;
	}
	else
	{
		if (it->second == ActorType::AT_PLAYER || it->second == ActorType::AT_PLAYERFIRE)
		{
			auto  entity = std::find_if(m_PlayerCollisionEntityList.begin(), m_PlayerCollisionEntityList.end(), [&](std::shared_ptr<ICollisionComponent> a){ return a->GetEntity()->GetID() == id; });
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
			auto  entity = std::find_if(m_EnemyCollisionEntityList.begin(), m_EnemyCollisionEntityList.end(), [&](std::shared_ptr<ICollisionComponent> a){ return a->GetEntity()->GetID() == id; });
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
void PhysicsSystem::RemoveCollisionEntity(uint32_t id)
{
	auto it = m_CollisionEntityList.find(id);
	if (it != m_CollisionEntityList.end())
	{
		switch (it->second)
		{
		case ActorType::AT_PLAYER:
		case ActorType::AT_PLAYERFIRE:
			m_PlayerCollisionEntityList.remove_if([&](std::shared_ptr<ICollisionComponent> a) { return a->GetEntity()->GetID() == id; });
			break;
		case ActorType::AT_ALIEN1:
		case ActorType::AT_ALIEN2:
		case ActorType::AT_ALIEN3:
		case ActorType::AT_ALIEN4:
		case ActorType::AT_ALIENFIRE:
			m_EnemyCollisionEntityList.remove_if([&](std::shared_ptr<ICollisionComponent> a) { return a->GetEntity()->GetID() == id; });
			break;
		case ActorType::AT_UNKNOWN:
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
bool PhysicsSystem::OnUpdate(uint32_t deltaMilliseconds)
{
	// I only check the collisions between the players entities and the enemy entities. I do not consider friendly fire.
	for (auto& playerEntity : m_PlayerCollisionEntityList)
	{
		for (auto& enemyEntity : m_EnemyCollisionEntityList)
		{
			
			if ((abs((int)(playerEntity->GetEntity()->GetPosX() - enemyEntity->GetEntity()->GetPosX())) <= (int)playerEntity->GetSize())
				||
				(abs((int)(playerEntity->GetEntity()->GetPosX() - enemyEntity->GetEntity()->GetPosX())) <= (int)enemyEntity->GetSize()))
			{
				if ((abs((int)(playerEntity->GetEntity()->GetPosY() - enemyEntity->GetEntity()->GetPosY())) <= (int)playerEntity->GetSize())
					||
					(abs((int)(playerEntity->GetEntity()->GetPosY() - enemyEntity->GetEntity()->GetPosY())) <= (int)enemyEntity->GetSize()))

					{
					auto it = m_CollisionEntityList.find(playerEntity->GetEntity()->GetID());
						ASSERT_DESCRIPTION(it != m_CollisionEntityList.end(), "There is an error and the collision Entity List has got desynched with the player and enemy collision entity lists");
						if (it->second == ActorType::AT_PLAYERFIRE)
						{
							IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_EnemyCollisionDetected(playerEntity->GetEntity()->GetID(), enemyEntity->GetEntity()->GetID())));
						}
						else
						{
							IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_PlayerCollisionDetected(playerEntity->GetEntity()->GetID(), enemyEntity->GetEntity()->GetID())));
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
bool PhysicsSystemListener::HandleEvent(IEventData const & event) const
{

	EventType eventType = event.GetEventType();

	if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_DestroyActor const & ed = static_cast< const EvtData_DestroyActor & >(event);
		m_PhysicsSystem->RemoveCollisionEntity(ed.m_ActorId);
		return true;
	}
	return false;
}
