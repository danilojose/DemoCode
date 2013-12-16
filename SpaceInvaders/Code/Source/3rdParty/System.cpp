#include <3rdParty\System.h>
#include <System\Assert.h>
#include <SDL_ttf.h>
#include <string>



SDL_Window* SDL_System::s_window = nullptr;


SDL_System::SDL_System()
{
}

SDL_System::~SDL_System()
{

	TTF_Quit();
	SDL_Quit();
}

void SDL_System::Init(unsigned int screenWidth, unsigned int screenHeight)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	s_window = SDL_CreateWindow("Danilo Vaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	ASSERT_DESCRIPTION((win == nullptr), "SDL_Init windows failed: " << SDL_GetError());


	SDL_ShowCursor(0);
	//
	ASSERT_RESULT_DECL(const int ttfInitResult, TTF_Init());
	ASSERT_DESCRIPTION(ttfInitResult == 0, "TTF_Init failed: " << TTF_GetError());

}

