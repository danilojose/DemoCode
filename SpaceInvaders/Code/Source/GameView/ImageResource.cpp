#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Resource.h>
#include <GameView\ImageResource.h>
#include <GameView\RenderSystem.h>
#include <SDL_image.h>


using namespace GameSystem;
using namespace Graphics;
extern RenderSystem * g_pRenderSystem;
/// <summary>
/// Creates the handle to a ImageResource. Handles are the one that really consume memory
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <returns></returns>
ResHandle * ImageResource::CreateHandle(const std::string &filename, ResCache *pResCache)
{
	return GCC_NEW ImageResHandle(*this, filename, pResCache);

}
/// <summary>
/// Initializes a new instance of the <see cref="ImageResHandle"/> class. This sounds too much for the constructor... 
/// </summary>
/// <param name="r">The r.</param>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
ImageResHandle::ImageResHandle(Resource &r, const std::string &filename, ResCache *pResCache) :ResHandle(r, filename, pResCache)
{

	ASSERT_DESCRIPTION(!filename.empty(), "Cannot load image without a filename!");

}
/// <summary>
/// Loads the data from an ImageResourceHandle
/// </summary>
/// <returns></returns>
void ImageResHandle::Load()
{
	SDL_Surface *loadedSurface = IMG_Load(m_Filename.c_str());
	ASSERT_DESCRIPTION(loadedSurface == nullptr, "Create Texture from surface failed: " << IMG_GetError());

	ASSERT_RESULT_DECL(const int result, SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255)));

	ASSERT_DESCRIPTION(result == 0, "SDL_SetColorKey failed: " << SDL_GetError());

	m_Texture = SDL_CreateTextureFromSurface(g_pRenderSystem->GetRenderer(), loadedSurface);
	ASSERT_DESCRIPTION(m_Texture == nullptr, "Create Texture from surface failed: " << IMG_GetError());

	ASSERT_RESULT_DECL(const int error, SDL_QueryTexture(m_Texture, &m_Format, &m_Access, &m_Width, &m_Height));
	ASSERT_DESCRIPTION(error == 0, "Create Texture from surface failed: " << IMG_GetError());

	SDL_FreeSurface(loadedSurface);

}
/// <summary>
/// Finalizes an instance of the <see cref="ImageResHandle"/> class.
/// </summary>
ImageResHandle::~ImageResHandle()
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = nullptr;

}