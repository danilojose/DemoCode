#include <System\EventManager.h>
#include <UI\FontSystem.h>
#include <UI\FontResource.h>
#include <System\Assert.h>
#include <System\Application.h>
#include <3rdParty\System.h>
#include <GameView\RenderSystem.h>

extern RenderSystem * g_pRenderSystem;
extern GameCodeApp * g_pApp;

/// <summary>
/// Inits the font System
/// </summary>
/// <param name="descriptor">The font system descriptor.</param>
void FontSystem::Init(JSONNode *descriptor)
{
	JSONNode * fontsDescriptor = descriptor->GetChild("Fonts");

	const std::vector<JSONNode*> fontsVector=fontsDescriptor->GetArrayValues();

	for (const auto &fontsDescriptor: fontsVector)
	{
		const char *fontStyle = fontsDescriptor->GetString("Name");
		const char *fontPath = fontsDescriptor->GetString("Font");
		uint8_t fontSize = static_cast<uint8_t>(fontsDescriptor->GetUInteger("FontSize"));

		ASSERT_DESCRIPTION(fontStyle, "FontStyle not found when reading descriptor");
		ASSERT_DESCRIPTION(fontPath, "Font not found when reading descriptor");
		ASSERT_DESCRIPTION(fontSize>0 && fontSize<100, "Font Size not found or not greater than zero or greater than max Value (100)");
		AddFont(fontStyle, fontPath, fontSize);
	}
}

/// <summary>
/// Adds the font.
/// </summary>
/// <param name="fontPath">The font path.</param>
/// <param name="size">The size.</param>
void FontSystem::AddFont(const std::string &fontStyle, const std::string &fontPath, uint8_t size)
{
	HashedString hashedFontStyleString = HashedString(fontStyle.c_str());
	auto it = m_Fonts.find(hashedFontStyleString.getHashValue());
	ASSERT_DESCRIPTION(it == m_Fonts.end(), fontStyle + " Already exists at the font system. It cannot be added")
		m_Fonts[hashedFontStyleString.getHashValue()] = std::shared_ptr<FontResource>(GCC_NEW FontResource(fontPath, size));

}

/// <summary>
/// Gets the font Resource
/// </summary>
/// <param name="fontStyle">The font style.</param>
/// <returns></returns>
std::shared_ptr<FontResource> FontSystem::GetFont(const std::string &fontStyle)
{

	HashedString hashedFontStyleString = HashedString(fontStyle.c_str());
	auto it = m_Fonts.find(hashedFontStyleString.getHashValue());
	ASSERT_DESCRIPTION(it != m_Fonts.end(), fontStyle + " Does Not exists at the font system. It cannot be added")
		return m_Fonts[hashedFontStyleString.getHashValue()];

}
/// <summary>
/// Renders the text.
/// </summary>
/// <param name="fontStyle">The font style.</param>
/// <param name="text">The text.</param>
/// <param name="positionX">The position x.</param>
/// <param name="positionY">The position y.</param>
void FontSystem::RenderText(const std::string &fontStyle, const std::string &text, uint16_t positionX, uint16_t positionY, uint16_t width, uint16_t height, float r, float g, float b)
{
	std::shared_ptr<FontResource> fontResource=GetFont(fontStyle);

	ASSERT_DESCRIPTION(fontResource, fontStyle+"Font Resource has not been found in the system");

	std::shared_ptr<FontResHandle> fontResHandle = std::static_pointer_cast<FontResHandle>(g_pApp->GetCache()->GetHandle(fontResource.get()));

	ASSERT_DESCRIPTION(fontResHandle, "the font is not found at the resource cache");
	ASSERT_DESCRIPTION(fontResHandle->GetFont(), "the font is not loaded at the resource cache");
	ASSERT_DESCRIPTION(!text.empty(), "Trying to render empty string of text!");
	ASSERT_DESCRIPTION(r >= 0.0f && r <= 1.0f, "r must be a value between 0.0 and 1.0! r = " << r);
	ASSERT_DESCRIPTION(g >= 0.0f && g <= 1.0f, "g must be a value between 0.0 and 1.0! g = " << g);
	ASSERT_DESCRIPTION(b >= 0.0f && b <= 1.0f, "b must be a value between 0.0 and 1.0! b = " << b);



	const SDL_Color forgroundColor = { static_cast<Uint8>(r * 255.0f), static_cast<Uint8>(g * 255.0f), static_cast<Uint8>(b * 255.0f), 0 };

	//Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
	SDL_Surface *textSurface = TTF_RenderText_Blended(fontResHandle->GetFont(), text.c_str(), forgroundColor);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(g_pRenderSystem->GetRenderer(), textSurface);

	SDL_Rect rect = {positionX, positionY, width, height };

	ASSERT_RESULT_DECL(const int result, SDL_RenderCopy(g_pRenderSystem->GetRenderer(), texture, NULL, &rect));
	ASSERT_DESCRIPTION(result == 0, "SDL_RenderCopy for text failed: " << SDL_GetError());

	SDL_FreeSurface(textSurface);
}

