#pragma once
#include <GameView\FontResource.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\StdLibraries.h>

///namespaces

using namespace GameSystem;

namespace Graphics
{

	// no need of using a list anyways we are not going to remove fonts by now.
	typedef std::vector<std::shared_ptr<FontResource> > FontsVector;
	/// <summary>
	/// Font System that is able to create Textures that contains the actual text that is intended to be rendered with the font and size chosen
	/// </summary>
	class FontSystem
	{
		public:

			FontsVector m_Fonts;
			/// <summary>
			/// Initializes a new instance of the <see cref="FontSystem"/> class.
			/// </summary>
			FontSystem() {}
			/// <summary>
			/// Finalizes an instance of the <see cref="FontSystem"/> class.
			/// </summary>
			virtual ~FontSystem() {}

			/// <summary>
			/// Adds the font.
			/// </summary>
			/// <param name="fontPath">The font path.</param>
			/// <param name="size">The size.</param>
			void AddFont(const std::string &fontPath, uint8_t size);

			/// <summary>
			/// Adds the font.
			/// </summary>
			/// <param name="fontPath">The font path.</param>
			/// <param name="size">The size.</param>
			void GetFont(const std::string &fontPath, uint8_t size);

			/// <summary>
			/// Renders the text.
			/// </summary>
			/// <param name="fontPath">The font path.</param>
			/// <param name="size">The size.</param>
			/// <param name="text">The text.</param>
			void RenderText(const std::string &fontPath, uint8_t size, const std::string &text);

	};


}