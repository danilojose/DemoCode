#pragma once
#include <System\EventManager.h>
#include <System\Events.h>
#include <System\StdLibraries.h>
#include <GameView\ImageResource.h>
#include <GameView\RenderSystem.h>
#include <UI\FontSystem.h>
#include <UI\UIElements.h>
#include <System\GameOptions.h>


//namespaces

using namespace Graphics;
using namespace UI;
namespace GameSystem
{

	/// <summary>
	/// Class in charge of displaying the Screen Elements. Currently hardcoded but will implement a more datadriven structure
	/// </summary>
	class UISystem
	{

	protected:

		std::shared_ptr<RenderSystem> m_pRendersystem;
		std::shared_ptr<FontSystem> m_pFontSystem;

		std::vector<std::unique_ptr<UIWidget>> m_Widgets;
		std::unique_ptr<UIWidgetFactory> m_WidgetFactory;

		uint32_t m_LastWidgetId;
		/// <summary>
		/// Gets the new actor identifier.
		/// </summary>
		/// <returns></returns>
		uint32_t GetNewWidgetId(void)
		{
			return ++m_LastWidgetId;
		}

		EventListenerPtr m_pUISystemListener;			// Collision event listener
		bool m_RenderDiagnostics;						// Are we rendering diagnostics?

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="UISystem"/> class.
		/// </summary>
		/// <param name="gameOptions">The game options.</param>
		/// <param name="gameOptions">The game options.</param>
		explicit UISystem(std::shared_ptr<FontSystem> fontSystem);

		/// <summary>
		/// Finalizes an instance of the <see cref="UISystem"/> class.
		/// </summary>
		virtual ~UISystem()
		{
			m_Widgets.clear();
		}

		/// <summary>
		/// Initializes this instance.
		/// </summary>
		void Init(JSONNode* descriptor);

		/// <summary>
		/// Loads a FrontEnd
		/// </summary>
		void LoadFrontEnd(const std::string &filename);

		/// <summary>
		/// Gets Font System so that UIElements can use it
		/// </summary>
		FontSystem* GetFontSystem() { return m_pFontSystem.get(); }

		// Pausing
		/// <summary>
		/// Toggles the pause.
		/// </summary>
		/// <param name="active">The active.</param>
		void TogglePause(bool active);

		/// <summary>
		/// On Render function
		/// </summary>

		virtual void OnRender();
		/// <summary>
		/// Updates the score.
		/// </summary>
		/// <param name="additionalScore">The additional score.</param>
		virtual void UpdateScore(uint16_t additionalScore);

		/// <summary>
		/// Updates the remaining lives.
		/// </summary>
		/// <param name="lives">The lives.</param>
		virtual void UpdateRemainingLives(uint16_t lives);
	};


	/// <summary>
	/// Implements Listener for UI System
	/// </summary>
	class UISystemListener : public IEventListener
	{
		UISystem *m_UISystem;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="UISystemListener"/> class.
		/// </summary>
		/// <param name="uiSystem">The UI system.</param>
		explicit UISystemListener(UISystem * uiSystem) : IEventListener() { m_UISystem = uiSystem; }
		/// <summary>
		/// Gets the name.
		/// </summary>
		/// <returns></returns>
		virtual char const* GetName(void) const override { return "UISystemListener"; }
		/// <summary>
		/// Handles the event that could affect the UISystem
		/// </summary>
		/// <param name="event">The event.</param>
		/// <returns></returns>
		virtual bool HandleEvent(IEventData const & event) const override;
	};
}
