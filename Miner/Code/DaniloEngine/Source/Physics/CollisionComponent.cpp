#include <System\Assert.h>
#include <Physics\CollisionComponent.h>


using namespace Physics;
const std::string ICollisionComponent::COMPONENT_NAME = "ICollisionComponent";
/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void ICollisionComponent::Build(JSONNode *descriptor)
{

	ASSERT_DESCRIPTION(descriptor, "Physics Radius Not Found");
	uint32_t collisionRadius = descriptor->GetUInteger("CollisionRadius");
	ASSERT_DESCRIPTION(collisionRadius>0, "Physics Radius should be greater than 0");
	m_Size = collisionRadius;
}


std::shared_ptr<IComponent> ICollisionComponent::Clone(Entity *entity)
{
	std::shared_ptr<IComponent> cloned=std::shared_ptr<IComponent>(GCC_NEW ICollisionComponent());
	return cloned;
	
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void ICollisionComponent::OnEntityEvent(IEventData const & event)
{

}