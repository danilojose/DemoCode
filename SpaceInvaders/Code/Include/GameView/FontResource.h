#pragma once
#include <System\Resource.h>
#include <SDL_ttf.h>

using namespace GameSystem;

namespace Graphics
{


	/// <summary>
	/// FontResource that contains the name of the resource and has the ability of creating a handle to the resource
	/// </summary>
	class FontResource : public Resource
	{
	public:
		
		uint8_t m_FontSize;
		/// <summary>
		/// Initializes a new instance of the <see cref="FontResource"/> class.
		/// </summary>
		/// <param name="filename">The filename.</param>
		FontResource(const std::string &filename,uint8_t fontSize) : Resource(filename), m_FontSize(fontSize) { }
		/// <summary>
		/// Creates the handle to a FontResource. Handles are the one that really consume memory
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <param name="pResCache">The p resource cache.</param>
		/// <returns></returns>
		virtual ResHandle *CreateHandle(const std::string &filename, ResCache *pResCache) override;
	};
	/// <summary>
	/// Class that contains the memory for a given image
	/// </summary>
	class FontResHandle : public ResHandle
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FontResHandle"/> class.
		/// </summary>
		/// <param name="r">The r.</param>
		/// <param name="filename">The filename.</param>
		/// <param name="pResCache">The p resource cache.</param>
		/// <param name="size">The font size</param>
		FontResHandle(Resource &r, const std::string &filename, ResCache *pResCache,uint8_t size);
		/// <summary>
		/// Finalizes an instance of the <see cref="SoundFxResHandle"/> class.
		/// </summary>
		virtual ~FontResHandle();

		/// <summary>
		/// Gets the SDL Font. This is specific to SDL (sic)
		/// </summary>
		/// <returns></returns>
		TTF_Font *GetFont(){ return m_Font; }

		/// <summary>
		/// Loads the data from an ImageResourceHandle
		/// </summary>
		/// <returns></returns>
		void Load() override;

	private:
		TTF_Font *m_Font = nullptr;
		uint8_t m_Size;
	};

}