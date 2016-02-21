#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#include "Pulser.h"
#include "Cell.h"

#include <string>

#define MAX_TIMESTEPS 6

using namespace std;

class Game {
public:
	Game();
	~Game();
	void Init();
	
	void Update(float elapsed);
	void FixedUpdate(float elapsed);
	void Render();
	bool UpdateAndRender();

	bool isColliding(Entity* e1, Entity* e2);
	bool isCollidingCircle(Entity* e1, Entity* e2);
	bool isCollidingWithMouse(const Entity* e, int x, int y);
	float randFloat(float LO, float HI);
	GLuint LoadTexture(const char *image_path);
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);

	bool Game::goodSpawn(float x, float y);

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	int resolution_x = 800;
	int resolution_y = 600;
	
	GLuint fontSheetTexture;

	vector<Entity*> entities;
	Pulser* pulser;
	vector<Entity*>  pulses;
	vector<Entity*> cells;

	bool sendFollowupWave;
	float followupWaveTimer;

	int score;
	int gameState;
	float wave_front;
	float wave_shade;
};