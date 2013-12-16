#pragma once
#include <System\EventManager.h>
#include <System\Events.h>
#include <System\StdLibraries.h>
#include <GameView\ImageResource.h>
#include <GameView\RenderSystem.h>
#include <GameView\FontSystem.h>
#include <3rdParty\Font.h>
#include <System\GameOptions.h>


//namespaces

using namespace Graphics;
namespace GameSystem
{

	/// <summary>
	/// ImageData struct that contains the data that is read to display the FrontEnd
	/// </summary>
	struct ImageData
	{
		std::shared_ptr<ImageResource> m_Image;
		uint16_t m_PosX;
		uint16_t m_PosY;
		/// <summary>
		/// Initializes a new instance of the <see cref="ImageData"/> struct.
		/// </summary>
		/// <param name="fileImage">The file image.</param>
		/// <param name="positionX">The position x.</param>
		/// <param name="positionY">The position y.</param>
		ImageData(const std::string &fileImage, uint32_t positionX, uint32_t positionY);
		/// <summary>
		/// Finalizes an instance of the <see cref="ImageData"/> class.
		/// </summary>
		virtual  ~ImageData(){}

	};
	/// <summary>
	/// LabelData struct that contains the data that is read to display the FrontEnd
	/// </summary>

	struct LabelData
	{
		std::unique_ptr<Font> m_Font;
		std::unique_ptr<HashedString> m_Binding;
		std::string m_Text;
		uint16_t m_PosX;
		uint16_t m_PosY;
		/// <summary>
		/// Initializes a new instance of the <see cref="LabelData"/> struct.
		/// </summary>
		/// <param name="fontFile">The font file.</param>
		/// <param name="positionX">The position x.</param>
		/// <param name="positionY">The position y.</param>
		/// <param name="fontSize">Size of the font.</param>
		/// <param name="binding">The binding.</param>
		/// <param name="text">The text.</param>
		LabelData(const std::string &fontFile, uint32_t positionX, uint32_t positionY,uint32_t fontSize,const std::string &binding,const std::string &text);
		/// <summary>
		/// Finalizes an instance of the <see cref="LabelData"/> class.
		/// </summary>
		virtual  ~LabelData(){}

	};
	/// <summary>
	/// Class in charge of displaying the Screen Elements. Currently hardcoded but will implement a more datadriven structure
	/// </summary>
	class UISystem
	{

	protected:

		std::shared_ptr<GameOptions> m_GameOptions;
		std::shared_ptr<ImageData> m_Background;
		std::shared_ptr<ImageData> m_BackgroundScore;
		std::unique_ptr<LabelData> m_ScoreText;
		std::unique_ptr<LabelData> m_ScoreValue;
		std::unique_ptr<LabelData> m_LivesText;
		std::unique_ptr<LabelData> m_LivesValue;

		std::shared_ptr<RenderSystem> m_pRendersystem;
		std::shared_ptr<FontSystem> m_pFontSystem;

		EventListenerPtr m_pUISystemListener;			// Collision event listener
		bool m_RenderDiagnostics;						// Are we rendering diagnostics?
		/// <summary>
		/// Creates the label.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <returns></returns>
		std::unique_ptr<LabelData> UISystem::CreateLabel(const std::string &key);
		/// <summary>
		/// Creates the image.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <returns></returns>
		std::unique_ptr<ImageData> UISystem::CreateImage(const std::string &key);

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="UISystem"/> class.
		/// </summary>
		/// <param name="gameOptions">The game options.</param>
		/// <param name="gameOptions">The game options.</param>
		explicit UISystem(std::shared_ptr<GameOptions> gameOptions, std::shared_ptr<FontSystem> fontSystem);

		/// <summary>
		/// Finalizes an instance of the <see cref="UISystem"/> class.
		/// </summary>
		virtual ~UISystem()
		{}

		/// <summary>
		/// Initializes this instance.
		/// </summary>
		void Init();
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
