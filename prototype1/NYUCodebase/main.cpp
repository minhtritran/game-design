
#include "EightQueens.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	EightQueens app;
	while (!app.UpdateAndRender()) {}
	return 0;
}