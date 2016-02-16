#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#include "Poop.h"
#include "People.h"
#include <math.h>
#include <string>

#define MAX_TIMESTEPS 6

using namespace std;

class PoopGame {
public:
	PoopGame();
	~PoopGame();
	void Init();
	bool isColliding(Entity* e1, Entity* e2);
	bool isCollidingWithMouse(const Entity* e, int x, int y);
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
	bool UpdateAndRender();
	float randFloat(float LO, float HI);
	GLuint LoadTexture(const char *image_path);
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	int resolution_x = 800;
	int resolution_y = 600;
	
	GLuint fontSheetTexture;

	vector<Entity*> entities;
	vector<Poop*> poops;
	vector<People*> peoples;
	Entity* dragEntity;

	bool dragging;

	int score;
	int gameState;
};