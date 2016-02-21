#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <tuple>
#include <iostream>
#include <vector>
#include <math.h>

#define FIXED_TIMESTEP 0.0166666f
#define M_PI 3.14159265359

#define NUM_CELLS 40
#define BACKGROUND_SHADE 0.3f
#define PULSE_SPEED 30
#define WAVE_WIDTH 0.03f
#define WAVE_SPEED 0.06f
#define CELL_SIZE 0.05f
#define CELL_STARTING_SHADE_DIVERSITY 0.3f 
#define CELL_DEATH_SENSITIVITY 0.1f
#define CELL_CHANGE_STEP 0.4f
#define CELL_COLOR_LOCK_TOLERANCE 0.2f


class Entity {
public:
	~Entity();
	Entity(float height, float width, float x, float y, float rotation);
	Entity(float x, float y, float radius);
	const float getX();
	const float getY();
	void setColor(int r, int g, int b);
	void setColor(std::tuple<float,float,float> color);
	void setTexture(unsigned int textureID, float u, float v, float sprite_width, float sprite_height);
	virtual void Update(float elapsed);
	virtual void FixedUpdate(float elapsed);
	virtual void Render();
	std::tuple<float, float, float> getCurrentColor();
	float lerp(float v0, float v1, float t);
	float linear(float t, float b, float c, float d);
	float ease(float v0, float v1, float t);
	float easeExpo(float t, float b, float c, float d);

	float height;
	float width;
	float radius = -1.0f;
	float x;
	float y;
	float rotation;
	
	float r = -1;
	float g;
	float b;

	bool usingTexture = false;
	int sprite_width;
	int sprite_height;
	int u;
	int v;
	unsigned int textureID;

	bool draggable;
	Entity* collision;
	bool killed;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	float friction_x;
	float friction_y;

	bool isFollowupWave;

};