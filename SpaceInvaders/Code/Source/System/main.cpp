#include <System\Application.h>
#include <SDL.h>

int main(int argc, char** argv)
{
	argc = argc;
	argv = argv;



	GameCodeApp app;

	if (!app.InitInstance())
	{
		return -1;
	}

	app.MainLoop();

	return 0;
}
