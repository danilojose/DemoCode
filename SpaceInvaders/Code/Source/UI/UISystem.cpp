#include <UI\UISystem.h>
#include <System\GameOptions.h>

using namespace GameSystem;

extern RenderSystem* g_pRenderSystem;
/// <summary>
/// Initializes a new instance of the <see cref="ImageData"/> struct.
/// </summary>
/// <param name="fileImage">The file image.</param>
/// <param name="positionX">The position x.</param>
/// <param name="positionY">The position y.</param>
ImageData::ImageData(const std::string &fileImage, uint32_t positionX, uint32_t positionY) :m_PosX(positionX), m_PosY(positionY)
{
	m_Image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(fileImage));
}

/// <summary>
/// Initializes a new instance of the <see cref="LabelData"/> struct.
/// </summary>
/// <param name="fontFile">The font file.</param>
/// <param name="positionX">The position x.</param>
/// <param name="positionY">The position y.</param>
/// <param name="fontSize">Size of the font.</param>
/// <param name="binding">The binding.</param>
/// <param name="text">The text.</param>
LabelData::LabelData(const std::string &fontFile, uint32_t positionX, uint32_t positionY, uint32_t fontSize, const std::string &binding,const std::string &text)
{
	m_Font = std::unique_ptr<Font>(GCC_NEW Font());
	m_Font->loadTtf(fontFile,fontSize);
	m_PosX = positionX;
	m_PosY = positionY;
	m_Binding = std::unique_ptr<HashedString>(GCC_NEW HashedString(binding.c_str()));
	m_Text = text;
}
/// <summary>
/// Initializes a new instance of the <see cref="UISystem"/> class.
/// </summary>
/// <param name="gameOptions">The game options.</param>
UISystem::UISystem(std::shared_ptr<GameOptions> gameOptions, std::shared_ptr<FontSystem> fontSystem)
{
	m_GameOptions = gameOptions;
	m_pFontSystem = fontSystem;
	m_pUISystemListener = EventListenerPtr(GCC_NEW UISystemListener(this));
	IEventManager::Get()->VAddListener(m_pUISystemListener, EvtData_PointsObtained::sk_EventType);
	IEventManager::Get()->VAddListener(m_pUISystemListener, EvtData_LivesRemaining::sk_EventType);


}

/// <summary>
/// Creates the label.
/// </summary>
/// <param name="key">The key.</param>
/// <returns></returns>
std::unique_ptr<LabelData> UISystem::CreateLabel(const std::string &key)
{
	IniValuesMap scoreOptions = m_GameOptions->GetValuesForAGivenKey(key);

	uint32_t labelX, labelY;

	if (scoreOptions.find("X") != scoreOptions.end())
	{
		labelX = std::strtoul(scoreOptions["X"].c_str(), NULL, 0);
	}

	if (scoreOptions.find("Y") != scoreOptions.end())
	{
		labelY = std::strtol(scoreOptions["Y"].c_str(), NULL, 0);
	}
	std::string labelFont;
	if (scoreOptions.find("Font") != scoreOptions.end())
	{
		labelFont = scoreOptions["Font"];
	}

	unsigned int labelFontSize;
	if (scoreOptions.find("FontSize") != scoreOptions.end())
	{
		labelFontSize = std::strtol(scoreOptions["FontSize"].c_str(), NULL, 0);
	}

	std::string labelBinding;
	if (scoreOptions.find("Binding") != scoreOptions.end())
	{
		labelBinding = scoreOptions["Binding"];
	}

	std::string labelText;
	if (scoreOptions.find("Text") != scoreOptions.end())
	{
		labelText = scoreOptions["Text"];
	}

	return std::unique_ptr<LabelData>(GCC_NEW LabelData(labelFont, labelX, labelY, labelFontSize, labelBinding, labelText));

}

/// <summary>
/// Creates the image.
/// </summary>
/// <param name="key">The key.</param>
/// <returns></returns>
std::unique_ptr<ImageData> UISystem::CreateImage(const std::string &key)
{
	IniValuesMap imageOptions = m_GameOptions->GetValuesForAGivenKey(key);

	uint32_t imageX, imageY;

	if (imageOptions.find("X") != imageOptions.end())
	{
		imageX = std::strtoul(imageOptions["X"].c_str(), NULL, 0);
	}

	if (imageOptions.find("Y") != imageOptions.end())
	{
		imageY = std::strtoul(imageOptions["Y"].c_str(), NULL, 0);
	}
	std::string imagefile;
	if (imageOptions.find("BackGroundImage") != imageOptions.end())
	{
		imagefile = imageOptions["BackGroundImage"];
	}

	return std::unique_ptr<ImageData>(GCC_NEW ImageData(imagefile, imageX, imageY));

}
/// <summary>
/// Initializes this instance.
/// </summary>
void UISystem::Init()
{
	m_Background = CreateImage("Background");
	m_BackgroundScore = CreateImage("BackgroundScore");
	m_ScoreText = CreateLabel("Score");
	m_ScoreValue = CreateLabel("ScoreValue");
	m_ScoreValue->m_Text = "0";
	m_LivesText = CreateLabel("Lives");
	m_LivesValue = CreateLabel("LivesValue");
	m_LivesValue->m_Text = "0";
}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void UISystem::TogglePause(bool active)
{
}



/// <summary>
/// On render function.
/// </summary>
void UISystem::OnRender()
{
	g_pRenderSystem->RenderImage(m_Background->m_Image, m_Background->m_PosX, m_Background->m_PosY);
	g_pRenderSystem->RenderImage(m_BackgroundScore->m_Image, m_BackgroundScore->m_PosX, m_BackgroundScore->m_PosY);

	m_ScoreText->m_Font->render(m_ScoreText->m_Text, m_ScoreText->m_PosX, m_ScoreText->m_PosY,1.0,1.0,0);
	m_ScoreValue->m_Font->render(m_ScoreValue->m_Text, m_ScoreValue->m_PosX, m_ScoreValue->m_PosY, 1.0, 1.0, 0);
	m_LivesText->m_Font->render(m_LivesText->m_Text, m_LivesText->m_PosX, m_LivesText->m_PosY, 1.0, 1.0, 0.0);
	m_LivesValue->m_Font->render(m_LivesValue->m_Text, m_LivesValue->m_PosX, m_LivesValue->m_PosY, 1.0, 1.0, 0.0);
}

/// <summary>
/// Updates the score.
/// </summary>
/// <param name="additionalScore">The additional score.</param>
void UISystem::UpdateScore(uint16_t additionalScore)
{
	std::stringstream s;
	s << additionalScore;
	m_ScoreValue->m_Text = s.str();
}

/// <summary>
/// Updates the remaining lives.
/// </summary>
/// <param name="lives">The lives.</param>
void UISystem::UpdateRemainingLives(uint16_t lives)
{
	std::stringstream s;
	s << lives;
	m_LivesValue->m_Text = s.str();

}
/// <summary>
/// Handles the event that could affect the UISystem
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool UISystemListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_PointsObtained::sk_EventType)
	{
		EvtData_PointsObtained const & ed = static_cast< const EvtData_PointsObtained & >(event);
		m_UISystem->UpdateScore(ed.m_Points);
		return true;
	}

	if (eventType == EvtData_LivesRemaining::sk_EventType)
	{
		EvtData_LivesRemaining const & ed = static_cast< const EvtData_LivesRemaining & >(event);
		m_UISystem->UpdateRemainingLives(ed.m_Lives);
		return true;
	}

	return false;
}
