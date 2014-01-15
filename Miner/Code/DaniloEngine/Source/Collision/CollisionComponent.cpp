#include <System\Assert.h>
#include <Collision\CollisionComponent.h>


using namespace Collision;
const std::string ICollisionComponent::COMPONENT_NAME = "ICollisionComponent";

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void ICollisionComponent::Build(JSONNode *descriptor)
{

	ASSERT_DESCRIPTION(descriptor, "Collision Radius Not Found");
	uint32_t collisionRadius = descriptor->GetUInteger("CollisionRadius");
	ASSERT_DESCRIPTION(collisionRadius>0, "Collision Radius should be greater than 0");
	m_Size = collisionRadius;
}


std::shared_ptr<IComponent> ICollisionComponent::Clone()
{
	return std::shared_ptr<IComponent>(GCC_NEW ICollisionComponent());
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void ICollisionComponent::OnEntityEvent(IEventData const & event)
{

}