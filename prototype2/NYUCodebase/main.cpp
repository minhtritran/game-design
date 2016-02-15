
#include "PoopGame.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	PoopGame app;
	while (!app.UpdateAndRender()) {}
	return 0;
}