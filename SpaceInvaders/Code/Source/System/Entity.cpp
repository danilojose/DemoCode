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

void Entity::AddComponent(std::shared_ptr<IComponent> component)
{
	m_Components.push_back(component);
	component->SetEntity(this);
}