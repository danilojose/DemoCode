#include<System\MinerEngine.h>
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
	srand((unsigned int)time(NULL));

	FileSystemReader fileReader("settings\\options.json");
	m_pJSONGameOptions = std::unique_ptr<JSONFileSystemParser>(GCC_NEW JSONFileSystemParser(fileReader.GetContents()));
	ASSERT_DESCRIPTION(m_pJSONGameOptions, "The JSON Game Options where not properly initialized");

	m_pJSONGameOptions->Init();

	SDL_System::Init(
		m_pJSONGameOptions->GetNode("options")->GetChild("Game")->GetUInteger("Width"),
		m_pJSONGameOptions->GetNode("options")->GetChild("Game")->GetUInteger("Height"));

	RegisterBaseGameEvents();	//Register all base event types.
	m_pRenderSystem = std::shared_ptr<RenderSystem>(GCC_NEW RenderSystem(SDL_System::GetWindow()));
	ASSERT_DESCRIPTION(m_pRenderSystem, "The Event Manager was not properly initialized");

	m_pUISystem->Init(m_pJSONGameOptions->GetNode("options")->GetChild("UI"));
	m_pEntitySystem->InitGame("settings\\entities.json");
	m_pUISystem->LoadFrontEnd("settings\\hud.json");

	m_pPhysicsSystem->Reset();
	m_pRenderSystem->Reset();
	m_pSoundSystem->Init();

	m_pWorld = std::shared_ptr<World>(GCC_NEW World());
	m_pGame = std::unique_ptr<SimBinGameLogic>(GCC_NEW SimBinGameLogic(m_pJSONGameOptions->GetNode("options")->GetChild("Game")->GetUInteger("Lives"), m_pWorld));
	ASSERT_DESCRIPTION(m_pGame, "The Game Logic Manager was not properly initialized");
	m_pGame->Reset();
	m_pGame->ChangeState(SB_Ingame);

	LoadLevel("settings\\level1.json");

	IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_LivesRemaining(m_pGame->GetCurrentLives())));
	return true;
}
void MinerEngine::RegisterBaseGameEvents()
{
	GameCodeApp::RegisterBaseGameEvents();

	// AI events....
	m_pEventManager->RegisterEvent(EvtData_SwapEntitiesRequested::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_StoneMovementRequested::sk_EventType);

}
void MinerComponentManager::Init()
{
	ComponentManager::Init();
	REGISTER_COMPONENT(MinerUserControlledBehaviour);
	REGISTER_COMPONENT(GameBoardBehaviour);
	REGISTER_COMPONENT(StoneBehaviour);
}

