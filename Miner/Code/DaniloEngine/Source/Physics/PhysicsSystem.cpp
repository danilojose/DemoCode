#include <System\EventManager.h>
#include <Physics\PhysicsSystem.h>
#include <Physics\CollisionComponent.h>
#include <System\Assert.h>
#include <System\Entity.h>


using namespace Physics;

PhysicsSystem::PhysicsSystem()
{
	m_pCollisionListener = EventListenerPtr(GCC_NEW PhysicsSystemListener(this));

}

PhysicsSystem::PhysicsSystem(PhysicsSystem&& other)
{
	m_pCollisionListener = other.m_pCollisionListener;
	m_CollisionEntityList = other.m_CollisionEntityList;
	m_pWorld = other.m_pWorld;
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
}

/// <summary>
/// Adds the collision entity.
/// </summary>
/// <param name="renderEntity">The render entity.</param>
/// <param name="type">The type.</param>
void PhysicsSystem::AddCollisionEntity(std::shared_ptr<ICollisionComponent> collisionEntity)
{
	ASSERT_DESCRIPTION(collisionEntity->GetEntity()!=nullptr&&collisionEntity->GetEntity()->GetID() > 0, "Attempted to add an actor with no valid ID")
	auto it = m_CollisionEntityList.find(collisionEntity->GetEntity()->GetID());
	ASSERT_DESCRIPTION(it == m_CollisionEntityList.end(), "Attempted to add a collision that already existed");
	m_CollisionEntityList[collisionEntity->GetEntity()->GetID()] = collisionEntity;
}
/// <summary>
/// Gets the collision entity.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<ICollisionComponent> PhysicsSystem::GetCollisionEntity(const uint32_t id)
{
	auto it = m_CollisionEntityList.find(id);
	if (it != m_CollisionEntityList.end())
	{
		return it->second;
	}
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
		m_CollisionEntityList.erase(id);
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
	//TODO: I have just removed the previous collisionSystem from the Alien Vaders Game I implemented
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
