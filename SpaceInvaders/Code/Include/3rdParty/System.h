#ifndef System_h
#define System_h

#include <SDL.h>

class SDL_System
{
public:
	SDL_System();
	~SDL_System();

	static void Init(unsigned int screenWidth,unsigned int screenHeight);
	static void Clear();
	static void Render();
	static SDL_Renderer* GetRenderer();

private:
	static SDL_Renderer* s_renderer;
	static SDL_Window* s_window;

	
};

#endif
