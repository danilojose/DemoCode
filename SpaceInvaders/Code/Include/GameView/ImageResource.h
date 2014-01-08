#pragma once
#include <System\Resource.h>
#include <SDL_image.h>

using namespace GameSystem;

namespace Graphics
{

	/// TODO: Crear método IsValid().

	/// <summary>
	/// ImageResource that contains the name of the resource and has the ability of creating a handle to the resource
	/// </summary>
	class ImageResource : public Resource
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ImageResource"/> class.
		/// </summary>
		/// <param name="filename">The filename.</param>
		ImageResource(const std::string &filename) : Resource(filename) { }
		/// <summary>
		/// Creates the handle to a ImageResource. Handles are the one that really consume memory
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <param name="pResCache">The p resource cache.</param>
		/// <returns></returns>
		virtual ResHandle *CreateHandle(const std::string &filename, ResCache *pResCache) override;
	};
	/// <summary>
	/// Class that contains the memory for a given image
	/// </summary>
	class ImageResHandle : public ResHandle
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ImageResHandle"/> class.
		/// </summary>
		/// <param name="r">The r.</param>
		/// <param name="filename">The filename.</param>
		/// <param name="pResCache">The p resource cache.</param>
		ImageResHandle(Resource &r, const std::string &filename, ResCache *pResCache);
		/// <summary>
		/// Finalizes an instance of the <see cref="SoundFxResHandle"/> class.
		/// </summary>
		virtual ~ImageResHandle();

		/// <summary>
		/// Gets the SDL Texture. This is specific to SDL (sic)
		/// </summary>
		/// <returns></returns>
		SDL_Texture *GetTexture(){ return m_Texture; }

		/// <summary>
		/// Gets the Width of the Texture
		/// </summary>
		/// <returns></returns>
		int GetWidth(){ return m_Width; }

		/// <summary>
		/// Gets Height of the Texture
		/// </summary>
		/// <returns></returns>
		int GetHeight(){ return m_Height; }

		/// <summary>
		/// Loads the data from an ImageResourceHandle
		/// </summary>
		/// <returns></returns>
		void Load() override;

	private:
		SDL_Texture *m_Texture = NULL;
		uint32_t m_Format;
		int m_Access;
		int m_Width;
		int m_Height;
	};

}