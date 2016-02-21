#pragma once
#include "Entity.h"

class Pulser : public Entity {
public:
	Pulser(float x, float y, float radius);
	void Update(float elapsed);
	void FixedUpdate(float elapsed);
	void Render();
	std::tuple<float, float, float> color1;
	std::tuple<float, float, float> color2;
	
	float timer;
	int increasing;
};