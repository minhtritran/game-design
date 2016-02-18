#pragma once
#include "Entity.h"

class People : public Entity {
public:
	People(float height, float width, float x, float y, float rotation);
	void Update(float elapsed);
	void FixedUpdate();
	void Render();

	float infected_amount;
	float infected_threshold;
	bool infected;

};