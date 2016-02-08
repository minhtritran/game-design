#include "Piece.h"

Piece::Piece(float height, float width, float x, float y, float rotation)
	: Entity(height, width, x, y, rotation) {
	this->draggable = true;
}

void Piece::Update(float elapsed) {
	Entity::Update(elapsed);

}

void Piece::Render() {
	Entity::Render();
}