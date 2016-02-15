#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define FIXED_TIMESTEP 0.0166666f

class Entity {
public:
	~Entity();
	Entity(float height, float width, float x, float y, float rotation);

	const float getX();
	const float getY();
	void setColor(int r, int g, int b);
	void setTexture(unsigned int textureID, float u, float v, float sprite_width, float sprite_height);
	virtual void Update(float elapsed);
	virtual void FixedUpdate();
	virtual void Render();
	float lerp(float v0, float v1, float t);

	float height;
	float width;
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

};