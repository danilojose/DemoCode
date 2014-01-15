#include <Logic\EmptyBehaviour.h>

using namespace AI;

const std::string EmptyBehaviour::COMPONENT_NAME = "EmptyBehaviour";

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void EmptyBehaviour::Build(JSONNode *descriptor)
{

}


/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> EmptyBehaviour::Clone()
{
	std::shared_ptr<EmptyBehaviour> cloned = std::shared_ptr<EmptyBehaviour>(GCC_NEW EmptyBehaviour());

	return cloned;
}


/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void EmptyBehaviour::OnEntityEvent(IEventData const & event)
{

}