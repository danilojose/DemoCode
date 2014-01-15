#ifndef System_h
#define System_h

#include <SDL.h>

class SDL_System
{
public:
	SDL_System();
	~SDL_System();

	static void Init(unsigned int screenWidth,unsigned int screenHeight);

	static SDL_Window* GetWindow() {
		return s_window;
	}

private:
	static SDL_Window* s_window;

	
};

#endif
