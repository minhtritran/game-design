#pragma once
#include "Entity.h"
#include <string>

class Piece : public Entity {
public:
	Piece(float height, float width, float x, float y, float rotation);
	void Update(float elapsed);
	void Render();
	
	std::string type;
	
};