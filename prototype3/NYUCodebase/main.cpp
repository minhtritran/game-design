
#include "Game.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	Game app;
	while (!app.UpdateAndRender()) {}
	return 0;
}