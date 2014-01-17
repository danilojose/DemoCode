#include <System\EntityEvents.h>
#include <System\Application.h>
#include <System\FileSystemReader.h>
#include <System\StdLibraries.h>
#include <3rdParty\Input.h>
#include <3rdParty\System.h>
#include <System\Assert.h>
#include <time.h>

using namespace GameSystem;

const uint8_t MAX_LOADSTRING=100;
GameCodeApp *g_pApp = NULL;


GameCodeApp::GameCodeApp(std::unique_ptr<EntitySystem> entitySystem,std::shared_ptr<GameLogic> gameLogic,std::unique_ptr<PhysicsSystem> physicsSystem) 
						:m_LastActorId(0), m_bContinueUpdating(true), m_HasModalDialog(0)
{
	g_pApp = this;
	m_pEventManager = GCC_NEW EventManager(true);
	ASSERT_DESCRIPTION(m_pEventManager, "The Event Manager was not properly initialized");
	m_pSoundSystem = std::unique_ptr<SoundSystem>(GCC_NEW SoundSystem());
	ASSERT_DESCRIPTION(m_pSoundSystem, "The Sound System was not properly initialized");

	m_pUISystem = std::unique_ptr<UISystem>(GCC_NEW UISystem(std::shared_ptr<FontSystem>(GCC_NEW FontSystem())));
	ASSERT_DESCRIPTION(m_pUISystem, "The UI System was not properly initialized");

	m_pResCache = GCC_NEW ResCache();
	ASSERT_DESCRIPTION(m_pResCache, "The Resolution Cache System was not properly initialized");

	ASSERT_DESCRIPTION(physicsSystem, "The Collision System was not properly initialized");
	m_pPhysicsSystem = std::move(physicsSystem);

	ASSERT_DESCRIPTION(entitySystem, "The Entity System was not properly initialized");
	m_pEntitySystem = std::move(entitySystem);

	m_pWorld = std::shared_ptr<World>(GCC_NEW World());
	ASSERT_DESCRIPTION(gameLogic, "The Entity System was not properly initialized");
	m_pGame = gameLogic;
	// sets world into gameLogic and PhysicsSytem
	m_pGame->SetWorld(m_pWorld);
	m_pPhysicsSystem->SetWorld(m_pWorld);

}

/// <summary>
/// Finalizes an instance of the <see cref="GameCodeApp"/> class.
/// </summary>
GameCodeApp::~GameCodeApp()
{

	// this is the only and single pointer we are not using an autopointer since it will be accessed from multiple places but will always be read accessed and never copied
	SAFE_DELETE(m_pResCache);
	SAFE_DELETE(m_pEventManager);
}

/// <summary>
/// Updates every system that is updatable
/// </summary>
/// <param name="deltaTime">The delta time.</param>
/// <returns></returns>
bool GameCodeApp::Update(uint32_t deltaTime)
{
	//TODO Add Asserts on NULL MNptrs
	m_pEventManager->VTick((unsigned long)deltaTime);
	bool bEntityUpdate=m_pEntitySystem->OnUpdate(deltaTime);
	bool bLogicUpdate = m_pGame->OnUpdate(deltaTime);
	bool bCollisionUpdate=m_pPhysicsSystem->OnUpdate(deltaTime);
	return bEntityUpdate&&bLogicUpdate&&bCollisionUpdate;
}

/// <summary>
/// Renders every system that is pontentially renderable
/// </summary>
void GameCodeApp::Render() const
{
	m_pRenderSystem->OnPreRender();
	m_pUISystem->OnRender();
	m_pRenderSystem->OnRender();
	m_pRenderSystem->OnPostRender();
}

/// <summary>
/// Initializes the instance.
/// </summary>
/// <param name="screenWidth">Width of the screen.</param>
/// <param name="screenHeight">Height of the screen.</param>
/// <returns></returns>
bool GameCodeApp::InitInstance(int screenWidth, int screenHeight)
{


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

	m_pGame->Reset(m_pJSONGameOptions->GetNode("options")->GetChild("Game")->GetUInteger("Lives"));
	m_pGame->ChangeState(SB_Ingame);

	LoadLevel("settings\\level1.json");

	IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_LivesRemaining(m_pGame->GetCurrentLives())));
	return true;
}

/// <summary>
/// This is the main loop!
/// </summary>
/// <returns></returns>
bool GameCodeApp::MainLoop()
{
	SDL_Event event;
	Uint32 lastRenderedTicks = SDL_GetTicks();
	Uint32 lastUpdatedTicks = lastRenderedTicks;

	bool leftKey = false;
	bool rightKey = false;
	bool upKey = false;
	bool downKey = false;
	bool fireKey = false;	
	bool continueUpdating = true;
	do
	{
		SDL_Delay(10);
		const uint32_t ticksNow = SDL_GetTicks();
		// 120Hz is one update every 8.3 ms
		if ((ticksNow - lastUpdatedTicks)>8)
		{
			//ASSERT_DESCRIPTION((ticksNow - lastUpdatedTicks) < 16, "We are suffering a reduction in the updates per second of the system");
			continueUpdating = Update(ticksNow - lastUpdatedTicks);
			if (!continueUpdating)
				break;
			lastUpdatedTicks = ticksNow;
		}

		// 60Hz is one update every 16.67 ms
		if ((ticksNow - lastRenderedTicks)>16)
		{
			//ASSERT_DESCRIPTION((ticksNow - lastRenderedTicks) < 33, "We are suffering a reduction in the frames per second");
			Render();
			lastRenderedTicks = ticksNow;
		}

		Input::setMouseMotion(0, 0);
		Input::setMouseDown(false);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				Input::setMouseMotion(event.motion.xrel, event.motion.yrel);
				Input::setMousePosition(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				Input::setMouseDown((event.button.button != 0));
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					leftKey = (event.type == SDL_KEYDOWN);
					break;
				case SDLK_RIGHT:
					rightKey = (event.type == SDL_KEYDOWN);
					break;
				case SDLK_UP:
					upKey = (event.type == SDL_KEYDOWN);
					break;
				case SDLK_DOWN:
					downKey = (event.type == SDL_KEYDOWN);
					break;
				case SDLK_LCTRL:
					fireKey = (event.type == SDL_KEYDOWN);
					break;
				case SDLK_ESCAPE:
					if (event.type == SDL_KEYDOWN)
					{
						continueUpdating = false;
					}
				}
				Input::setKeys(leftKey, rightKey, upKey, downKey, fireKey);
				break;
			case SDL_QUIT:
				continueUpdating = false;
				break;
			}
		}

	} while (continueUpdating);

	return true;
}

/// <summary>
/// Registers the base game events. This is the single place where the Events are registered. To be called when initialized
/// </summary>
void GameCodeApp::RegisterBaseGameEvents()
{
	ASSERT_DESCRIPTION(m_pEventManager!=nullptr,"Cannot register base game events until the event manager exists!");

	//Graphic Events

	m_pEventManager->RegisterEvent(EvtData_AnimationFinished::sk_EventType);
	//Physics events...
	m_pEventManager->RegisterEvent(EvtData_EnemyCollisionDetected::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_PlayerCollisionDetected::sk_EventType);

	//Actor events...
	m_pEventManager->RegisterEvent(EvtData_UpdatePosition::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_DestroyActor::sk_EventType);
	// AI events....
	m_pEventManager->RegisterEvent(EvtData_PointsObtained::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_LivesRemaining::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_CreateEnemyExplosion::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_CreatePlayerExplosion::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_CreatePlayerFire::sk_EventType);
	m_pEventManager->RegisterEvent(EvtData_CreateEnemyFire::sk_EventType);
	// Sound events....
	m_pEventManager->RegisterEvent(EvtData_PlaySound::sk_EventType);
	//General game events...


}

void GameCodeApp::LoadLevel(const char * levelFile)
{
	FileSystemReader f(levelFile);
	JSONFileSystemParser parser(f.GetContents());
	parser.Init();
	m_pWorld->Build(parser.GetNode("GameBoard"));
	

	//for (int i = 0; i < m_pWorld->GetNumberOfStonesByRow(); ++i)
	//{
	//	for (int j = 0; j < m_pWorld->GetNumberOfRows(); ++j)
	//	{
	//		std::shared_ptr<Entity> stone = m_pEntitySystem->CreateEntity((i%2==0)?
	//			((j % 2 == 0) ? "BlueStone" : "YellowStone"):
	//			((j % 2 == 0) ? "GreenStone" : "PurpleStone"));
	//		m_pWorld->AddEntity(i, j, stone.get());
	//	}
	//}

	std::shared_ptr<Entity> player = m_pEntitySystem->CreateEntity("Player");

	std::shared_ptr<Entity> board = m_pEntitySystem->CreateEntity("Board");



}







