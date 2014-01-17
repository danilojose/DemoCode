#include <System\Assert.h>
#include <System\ComponentManager.h>
#include <Physics\CollisionComponent.h>
#include <Logic\EmptyBehaviour.h>
#include <Logic\FireBehaviour.h>
#include <Logic\EnemyBehaviour.h>
#include <Logic\UserControlledBehaviour.h>
#include <GameView\AnimatedSpritesComponent.h>
#include <GameView\StaticAnimatedSpritesComponent.h>
#include <GameView\SelectableSpritesComponent.h>

using namespace Physics;
using namespace AI;
using namespace Graphics;



/// <summary>
/// Initializes this instance.
/// </summary>
void ComponentManager::Init()
{
	//...
	REGISTER_COMPONENT(ICollisionComponent);

	REGISTER_COMPONENT(SelectableSpritesComponent);
	REGISTER_COMPONENT(AnimatedSpritesComponent);
	REGISTER_COMPONENT(StaticAnimatedSpritesComponent);

	REGISTER_COMPONENT(UserControlledBehaviour);

	//...	
}
/// <summary>
/// Gets the new component.
/// </summary>
/// <param name="">The .</param>
/// <param name="descriptor">The descriptor.</param>
/// <returns></returns>
std::shared_ptr<IComponent> ComponentManager::GetNewComponent(const std::string &componentName, JSONNode *descriptor)
{
	ASSERT_DESCRIPTION(m_builderTable[componentName], componentName + " was not found at ComponentManager");
	std::shared_ptr<IComponent> component=m_builderTable[componentName]();
	component->Build(descriptor);
	return component;
}