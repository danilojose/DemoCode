#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Resource.h>
#include <GameView\FontResource.h>
#include <GameView\RenderSystem.h>
#include <SDL_ttf.h>


using namespace GameSystem;
using namespace Graphics;
extern RenderSystem * g_pRenderSystem;
/// <summary>
/// Creates the handle to a FontResource. Handles are the one that really consume memory
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <returns></returns>
ResHandle * FontResource::CreateHandle(const std::string &filename, ResCache *pResCache)
{
	return GCC_NEW FontResHandle(*this, filename, pResCache,m_FontSize);

}
/// <summary>
/// Initializes a new instance of the <see cref="FontResHandle"/> class. This sounds too much for the constructor... 
/// </summary>
/// <param name="r">The r.</param>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <param name="size">The font size</param>
FontResHandle::FontResHandle(Resource &r, const std::string &filename, ResCache *pResCache, uint8_t size) :ResHandle(r, filename, pResCache), m_Size(size)
{

	ASSERT_DESCRIPTION(!filename.empty(), "Cannot load image without a filename!");

}
/// <summary>
/// Loads the data from an FontResHandle
/// </summary>
/// <returns></returns>
void FontResHandle::Load()
{
	ASSERT_DESCRIPTION(!filename.empty(), "Cannot load font without a filename!");
	m_Font = TTF_OpenFont(m_Filename.c_str(), m_Size);
	ASSERT_DESCRIPTION(m_font != nullptr, "TTF_OpenFont failed: " << TTF_GetError());

}
/// <summary>
/// Finalizes an instance of the <see cref="FontResHandle"/> class.
/// </summary>
FontResHandle::~FontResHandle()
{
	TTF_CloseFont(m_Font);
	m_Font = nullptr;
}