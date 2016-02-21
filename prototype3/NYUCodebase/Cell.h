#pragma once
#include "Entity.h"

class Cell : public Entity {
public:
	Cell(float x, float y, float radius);
	void Update(float elapsed);
	void FixedUpdate(float elapsed);
	void Render();

	bool start_timer;
	float timer;
	float goal_shade;
	float old_shade;

	bool touched;
	bool dead;
	
};