#include <System\Assert.h>
#include <System\ComponentManager.h>
#include <Collision\CollisionComponent.h>
#include <Logic\EmptyBehaviour.h>
#include <Logic\FireBehaviour.h>
#include <Logic\EnemyBehaviour.h>
#include <Logic\UserControlledBehaviour.h>
#include <GameView\AnimatedSpritesComponent.h>
#include <GameView\StaticAnimatedSpritesComponent.h>

using namespace Collision;
using namespace AI;
using namespace Graphics;



void ComponentManager::Init()
{
	//...
	REGISTER_COMPONENT(ICollisionComponent);

	REGISTER_COMPONENT(AnimatedSpritesComponent);
	REGISTER_COMPONENT(StaticAnimatedSpritesComponent);

	REGISTER_COMPONENT(EmptyBehaviour);
	REGISTER_COMPONENT(EnemyBehaviour);
	REGISTER_COMPONENT(FireBehaviour);
	REGISTER_COMPONENT(UserControlledBehaviour);
	//...	
}
std::shared_ptr<IComponent> ComponentManager::GetNewComponent(const std::string &componentName, const IniValuesMap &descriptor)
{
	ASSERT_DESCRIPTION(m_builderTable[componentName], componentName + " was not found at ComponentManager");
	std::shared_ptr<IComponent> component=m_builderTable[componentName]();
	component->Build(descriptor);
	return component;
}