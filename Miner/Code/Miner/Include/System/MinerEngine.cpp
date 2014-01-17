#include<System\MinerEngine.h>
#include<System\MinerEntityEvents.h>
#include<Logic\MinerUserControlledBehaviour.h>
#include<Logic\GameBoardBehaviour.h>
#include<Logic\StoneBehaviour.h>
#include <time.h>

using namespace GameSystem;

/// <summary>
/// Initializes the instance.
/// </summary>
/// <param name="screenWidth">Width of the screen.</param>
/// <param name="screenHeight">Height of the screen.</param>
/// <returns></returns>
bool MinerEngine::InitInstance(int screenWidth , int screenHeight )
{
	
	GameCodeApp::InitInstance(screenWidth, screenHeight);
	return true;
}
void MinerEngine::RegisterBaseGameEvents()
{
	GameCodeApp::RegisterBaseGameEvents();

	// AI events....
	m_pEventManager->RegisterEvent(EvtData_SwapEntitiesRequested::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_StoneMovementRequested::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_ReleasePosition::sk_EventType);

}
/// <summary>
/// Removes the entity.
/// </summary>
/// <param name="id">The identifier.</param>
void MinerEntitySystem::RemoveEntity(uint32_t id)
{
	std::shared_ptr<Entity> actor = GetEntity(id);
	if (actor)
	{
		m_EntityMap.erase(id);
		if (actor->GetEntityName().getHashValue() == HashedString("Explosion").getHashValue())
		{
			IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_ReleasePosition(std::pair<uint16_t, uint16_t>(actor->GetPosX(), actor->GetPosY()))));
		}
	}
	else
	{
		ASSERT_DESCRIPTION(false, _T("UNKNOWN ACTOR") + id);
	}
}

void MinerComponentManager::Init()
{
	ComponentManager::Init();
	REGISTER_COMPONENT(MinerUserControlledBehaviour);
	REGISTER_COMPONENT(GameBoardBehaviour);
	REGISTER_COMPONENT(StoneBehaviour);
}

