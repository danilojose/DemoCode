#include <System\Application.h>
#include <System\MinerEngine.h>
#include <SDL.h>
#include <ostream>
#include <iostream>

int main(int argc, char** argv)
{
	argc = argc;
	argv = argv;

#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MinerEngine app;
	if (!app.InitInstance())
	{
		return -1;
	}

	app.MainLoop();

	return 0;
}
