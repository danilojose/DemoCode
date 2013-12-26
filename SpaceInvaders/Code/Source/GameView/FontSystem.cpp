#include <System\EventManager.h>
#include <GameView\FontSystem.h>
#include <GameView\FontResource.h>
#include <System\Assert.h>
#include <System\Application.h>
#include <3rdParty\System.h>

/// <summary>
/// Adds the font.
/// </summary>
/// <param name="fontPath">The font path.</param>
/// <param name="size">The size.</param>
void FontSystem::AddFont(const std::string &fontPath, uint8_t size)
{
	auto it = std::find_if(m_Fonts.begin(), m_Fonts.end(), [&] (std::shared_ptr<FontResource> r) 
	{
		if ((r->m_Filename.compare(fontPath) == 0) && (r->m_FontSize == size))
		{
			return true;
		}
		else
		{
			return false;
		}
	} 
	);
	if (it == m_Fonts.end())
	{
		m_Fonts.push_back(std::shared_ptr<FontResource>(GCC_NEW FontResource(fontPath, size)));
	}
}

/// <summary>
/// Renders the text.
/// </summary>
/// <param name="fontPath">The font path.</param>
/// <param name="size">The size.</param>
/// <param name="text">The text.</param>
void FontSystem::RenderText(const std::string &fontPath, uint8_t size, const std::string &text)
{

}