#pragma once
#include "Entity.h"

class Poop : public Entity {
public:
	Poop(float height, float width, float x, float y, float rotation);
	void Update(float elapsed);
	void FixedUpdate();
	void Render();

	void reduceSize(float size);
	
};