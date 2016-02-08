#include "Tile.h"

Tile::Tile(float height, float width, float x, float y, float rotation)
	: Entity(height, width, x, y, rotation)	 {
	this->draggable = false;
	this->touchingPiece = false;
}

void Tile::Update(float elapsed) {
	Entity::Update(elapsed);
	
	if (collision != nullptr) {
		this->touchingPiece = true;
	}
	//default (dark grey)
	if (status == 0) {
		this->setColor(70.0f, 70.0f, 70.0f);
	}
	//goal (light grey)
	else if (status == 1) {
		this->setColor(120.0f, 120.0f, 120.0f);
	}
	//set (white)
	else if (status == 2) {
		this->setColor(240.0f, 240.0f, 240.0f);
	}
	//suggest positive (green)
	else if (status == 3) {
		this->setColor(120.0f, 150.0f, 120.0f);
	}
	//suggest negative (red)
	else if (status == 4) {
		this->setColor(85.0f, 70.0f, 70.0f);
	}
}

void Tile::Render() {
	Entity::Render();
}