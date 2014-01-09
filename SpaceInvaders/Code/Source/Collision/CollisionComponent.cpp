#include <System\Assert.h>
#include <Collision\CollisionComponent.h>


using namespace Collision;
const std::string ICollisionComponent::COMPONENT_NAME = "ICollisionComponent";

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void ICollisionComponent::Build(const IniValuesMap &descriptor)
{
	ASSERT_DESCRIPTION(descriptor.find("CollisionRadius") != descriptor.end(), "Collision Radius Not Found");
	uint32_t collisionRadius = std::strtoul(descriptor.at("CollisionRadius").c_str(), NULL, 0);
	ASSERT_DESCRIPTION(collisionRadius>0, "Collision Radius should be greater than 0");
	m_Size = collisionRadius;
}