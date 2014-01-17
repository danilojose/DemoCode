
#pragma once

#include <System\IComponent.h>
#include <System\FileSystemReader.h>
using namespace GameSystem;
namespace Physics
{
/// <summary>
///  Component Class storing the Physics properties from a given actor.
///  We will use square collision boxes taking into account that the position x and y is the left top corner
/// </summary>
class ICollisionComponent :public IComponent
{
protected:
	uint32_t									m_Size;

public:
	static const std::string COMPONENT_NAME;

	ICollisionComponent() :IComponent(ICollisionComponent::COMPONENT_NAME,nullptr),m_Size(0)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="ICollisionComponent"/> struct.
	/// </summary>
	/// <param name="owner">The owner.</param>
	/// <param name="size">The size.</param>
	ICollisionComponent(Entity * owner, uint32_t size) :IComponent(ICollisionComponent::COMPONENT_NAME,owner),m_Size(size)
	{
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="ICollisionComponent"/> class.
	/// </summary>
	virtual ~ICollisionComponent(){}

	/// <summary>
	/// Gets the size.
	/// </summary>
	/// <returns></returns>
	const uint32_t GetSize() const { return m_Size; }
	/// <summary>
	/// Checks the collision.
	/// </summary>
	virtual void CheckCollision() {}

	/// <summary>
	/// Builds the specified descriptor.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void Build(JSONNode *descriptor) override;

	/// <summary>
	/// Clones the current Component
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual std::shared_ptr<IComponent> Clone(Entity *entity) override;

	/// <summary>
	/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
	/// </summary>
	/// <param name="descriptor">The descriptor.</param>
	virtual void OnEntityEvent(IEventData const & event)  override;
};
}