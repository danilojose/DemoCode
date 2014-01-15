#include <System\Entity.h>

using namespace GameSystem;
/// <summary>
/// Initializes the specified in.
/// </summary>
/// <param name="in">The in.</param>
/// <returns></returns>
bool Entity::Init(std::istrstream &in)
{
	int hasActorId = 0;

	in >> hasActorId;
	if (hasActorId)
	{
		in >> hasActorId;
		m_EntityId = hasActorId;
	}
	in >> m_PosX >> m_PosY;

	return true;
}

/// <summary>
/// Serializes the specified out.
/// </summary>
/// <param name="out">The out.</param>
void Entity::Serialize(std::ostrstream &out) const
{
	out << m_EntityId;
	out << m_PosX << m_PosY;
}
/// <summary>
/// Adds the component.
/// </summary>
/// <param name="component">The component.</param>
void Entity::AddComponent(std::shared_ptr<IComponent> component)
{
	m_Components.push_back(component);
	component->SetEntity(this);
}
/// <summary>
/// Trigger event inside the Entity. Later to be moved to a deque to be updated every single tick
/// </summary>
/// <param name="event">The event.</param>
void Entity::TriggerInternalEvent(IEventDataPtr const & event) 
{
	for (const auto &component : m_Components)
	{
		component->OnEntityEvent(*event);
	}
}