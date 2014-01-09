#include <System\Application.h>
#include <System\StdLibraries.h>
#include <3rdParty\Font.h>
#include <3rdParty\Input.h>
#include <3rdParty\System.h>
#include <System\Assert.h>
#include <time.h>

using namespace GameSystem;

const uint8_t MAX_LOADSTRING=100;
GameCodeApp *g_pApp = NULL;


/// <summary>
/// Initializes a new instance of the <see cref="GameCodeApp"/> class.
/// </summary>
GameCodeApp::GameCodeApp()
{

	//m_pEventManager = nullptr;
	m_LastActorId = 0;
	m_bContinueUpdating = true;
	m_HasModalDialog = 0;
	m_pResCache = nullptr;
	g_pApp = this;
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
	bool bCollisionUpdate=m_pCollisionSystem->OnUpdate(deltaTime);
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

	srand((unsigned int)time(NULL));

	//TODO: Check return of constructors.
	m_pGameOptions = std::unique_ptr<GameOptions>(GCC_NEW GameOptions("options.ini"));
	m_pGameOptions->Init();
	IniValuesMap prueba = m_pGameOptions->GetValuesForAGivenKey("Game");
	SDL_System::Init(std::strtoul(prueba["Width"].c_str(), NULL, 0), std::strtoul(prueba["Height"].c_str(), NULL, 0));

	// event manager should be created first so that subsystems
	// can hook in as desired.

	m_pEventManager = GCC_NEW EventManager( true);
	ASSERT_DESCRIPTION(m_pEventManager, "The Event Manager was not properly initialized");
	RegisterBaseGameEvents();	//Register all base event types.

	m_pGame = std::unique_ptr<SimBinGameLogic>(GCC_NEW SimBinGameLogic(std::strtoul(prueba["Lives"].c_str(), NULL, 0)));
	if (!m_pGame)
	{
		ASSERT_DESCRIPTION(false, "The Game Logic Manager was not properly initialized");
	}


	m_pRenderSystem = std::shared_ptr<RenderSystem>(GCC_NEW RenderSystem(SDL_System::GetWindow()));
	ASSERT_DESCRIPTION(m_pRenderSystem, "The Event Manager was not properly initialized");

	m_pSoundSystem = std::unique_ptr<SoundSystem>(GCC_NEW SoundSystem());
	ASSERT_DESCRIPTION(m_pSoundSystem, "The Sound System was not properly initialized");

	m_pUISystem = std::unique_ptr<UISystem>(GCC_NEW UISystem(m_pGameOptions,std::shared_ptr<FontSystem> (GCC_NEW FontSystem())));
	ASSERT_DESCRIPTION(m_pUISystem, "The UI System was not properly initialized");

	m_pResCache = GCC_NEW ResCache();
	ASSERT_DESCRIPTION(m_pResCache, "The UI System was not properly initialized");

	m_pCollisionSystem = std::unique_ptr<CollisionSystem>(GCC_NEW CollisionSystem());
	ASSERT_DESCRIPTION(m_pCollisionSystem, "The UI System was not properly initialized");

	m_pEntitySystem = std::unique_ptr<EntitySystem>(GCC_NEW EntitySystem(m_pGame,m_pRenderSystem,m_pCollisionSystem,m_pGameOptions));
	ASSERT_DESCRIPTION(m_pEntitySystem, "The UI System was not properly initialized");


	m_pUISystem->Init();
	m_pEntitySystem->InitGame();
	m_pGame->ChangeState(SB_Ingame);
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
	Uint32 oldTicks = SDL_GetTicks();

	bool leftKey = false;
	bool rightKey = false;
	bool upKey = false;
	bool downKey = false;
	bool fireKey = false;	
	bool continueUpdating = true;
	do
	{
		SDL_Delay(40);
		const uint32_t ticksNow = SDL_GetTicks();
		const uint32_t deltaTime = (ticksNow - oldTicks);
		oldTicks = ticksNow;
		continueUpdating = Update(deltaTime);
		if (!continueUpdating)
			break;
		Render();
		Input::setMouseMotion(0, 0);
		Input::setMouseDown(false);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			//case SDL_MOUSEMOTION:
			//	Input::setMouseMotion(event.motion.xrel, event.motion.yrel);
			//	Input::setMousePosition(event.motion.x, event.motion.y);
			//	break;
			//case SDL_MOUSEBUTTONDOWN:
			//	Input::setMouseDown((event.button.button != 0));
			//	break;
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






