#pragma once

#include <System\StdLibraries.h>
#include <System\World.h>
#include <Logic\SimBinVadersLogic.h>
#include <GameView\RenderSystem.h>
#include <Sound\SoundSystem.h>
#include <Collision\PhysicsSystem.h>
#include <System\EventManager.h>
#include <System\FileSystemReader.h>
#include <System\GameOptions.h>
#include <System\EntitySystem.h>
#include <System\Resource.h>
#include <UI\UISystem.h>

/// NameSpaces
using namespace Sound;

namespace GameSystem
{
	/// <summary>
	/// Class that is mainly our engine as it is the one that contains all the systems that are in charge of implementing our game
	/// </summary>
	class GameCodeApp
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameCodeApp"/> class.
		/// </summary>
		GameCodeApp(std::unique_ptr<EntitySystem> entitySystem);
		/// <summary>
		/// Finalizes an instance of the <see cref="GameCodeApp"/> class.
		/// </summary>
		virtual ~GameCodeApp();
		/// <summary>
		/// Updates every system that is updatable
		/// </summary>
		/// <param name="deltaTime">The delta time.</param>
		/// <returns></returns>
		bool Update(uint32_t deltaTime);
		/// <summary>
		/// Renders every system that is pontentially renderable
		/// </summary>
		void Render() const;

	private:
		// TODO: No inicializaciones
		int m_iIncX = 0, m_iIncY = 0;

	protected:
		bool m_bWindowedMode;					// true if the app is windowed, false if fullscreen
		bool m_bIsRunning;						// true if everything is initialized and the game is in the main loop
		bool m_bQuitRequested;					// true if the app should run the exit sequence
		bool m_bQuitting;						// true if the app is running the exit sequence
		uint32_t m_LastActorId;

		//TODO todo debería ser o std o raw. Singletons?
		std::shared_ptr<World> m_pWorld; //The AI GameLogic
		std::shared_ptr<SimBinGameLogic> m_pGame; //The AI GameLogic
		std::shared_ptr<RenderSystem> m_pRenderSystem; //The RenderSystem
		std::unique_ptr<SoundSystem> m_pSoundSystem; //The SoundSystem
		std::unique_ptr<UISystem> m_pUISystem; //The SoundSystem
		std::shared_ptr<PhysicsSystem> m_pPhysicsSystem; //The SoundSystem
		std::unique_ptr<JSONFileSystemParser> m_pJSONGameOptions; //The JSONGameOptions
		std::unique_ptr<EntitySystem> m_pEntitySystem; //The EntitySystem
		EventManager *m_pEventManager; // The Event Manager
		ResCache *m_pResCache; //The ResourceCache

		int m_HasModalDialog;					// determines if a modal dialog is up

		bool m_bContinueUpdating;


	public:

		/// <summary>
		/// Initializes the instance.
		/// </summary>
		/// <param name="screenWidth">Width of the screen.</param>
		/// <param name="screenHeight">Height of the screen.</param>
		/// <returns></returns>
		virtual bool InitInstance(int screenWidth = 1024, int screenHeight = 768);
		/// <summary>
		/// Registers the base game events. This is the single place where the Events are registered. To be called when initialized
		/// </summary>
		virtual void RegisterBaseGameEvents();
		/// <summary>
		/// This is the main loop!
		/// </summary>
		/// <returns></returns>
		bool MainLoop();

		/// <summary>
		/// Aborts the game.
		/// </summary>
		void AbortGame() { m_bQuitting = true; }
		/// <summary>
		/// Determines whether this instance is running.
		/// </summary>
		/// <returns></returns>
		bool IsRunning() { return m_bIsRunning; }
		/// <summary>
		/// Sets the quitting.
		/// </summary>
		/// <param name="quitting">The quitting.</param>
		void SetQuitting(bool quitting) { m_bQuitting = quitting; }

		/// <summary>
		/// Gets the cache.
		/// </summary>
		/// <returns></returns>
		ResCache *const  GetCache() { return m_pResCache; }

		/// <summary>
		/// Loads Level
		/// </summary>
		/// <returns></returns>
		void LoadLevel(const char *levelFile);

	};

}