#pragma once
#include <UI\FontResource.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\HashedString.h>
#include <System\StdLibraries.h>
#include <System\FileSystemReader.h>

///namespaces

using namespace GameSystem;

namespace UI
{

	// no need of using a list anyways we are not going to remove fonts by now.
	typedef std::unordered_map<uint32_t, std::shared_ptr<FontResource> > FontsMap;
	/// <summary>
	/// Font System that is able to create Textures that contains the actual text that is intended to be rendered with the font and size chosen
	/// </summary>
	class FontSystem
	{
		public:

			/// <summary>
			/// Initializes a new instance of the <see cref="FontSystem"/> class.
			/// </summary>
			FontSystem() {}
			/// <summary>
			/// Finalizes an instance of the <see cref="FontSystem"/> class.
			/// </summary>
			virtual ~FontSystem() {}

			/// <summary>
			/// Inits the font System
			/// </summary>
			/// <param name="descriptor">The font system descriptor.</param>
			void Init(JSONNode *descriptor);

			/// <summary>
			/// Adds the font.
			/// </summary>
			/// <param name="fontStyle">The font family name.</param>
			/// <param name="fontPath">The font path.</param>
			/// <param name="size">The size.</param>
			void AddFont(const std::string &fontStyle, const std::string &fontPath, uint8_t size);

			/// <summary>
			/// Renders the text.
			/// </summary>
			/// <param name="fontStyle">The font style.</param>
			/// <param name="text">The text.</param>
			/// <param name="positionX">The position x.</param>
			/// <param name="positionY">The position y.</param>
			void RenderText(const std::string &fontStyle, const std::string &text, uint16_t positionX, uint16_t positionY, uint16_t width, uint16_t height, float r, float g, float b);

	private:
			FontsMap m_Fonts;
			/// <summary>
			/// Gets the font Resource
			/// </summary>
			/// <param name="fontStyle">The font style.</param>
			/// <returns></returns>
			std::shared_ptr<FontResource> GetFont(const std::string &fontStyle);

	};


}