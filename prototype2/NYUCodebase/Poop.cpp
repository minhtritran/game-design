#include "Poop.h"

Poop::Poop(float height, float width, float x, float y, float rotation)
	: Entity(height, width, x, y, rotation) {
	friction_x = 10.0f;
	friction_y = 10.0f;
	draggable = true;
}

void Poop::Update(float elapsed) {
	Entity::Update(elapsed);
	if (width < 0 || height < 0) {
		killed = true;
	}

}

void Poop::FixedUpdate() {
	Entity::FixedUpdate();
}

void Poop::Render() {
	Entity::Render();
}

void Poop::reduceSize(float size) {
	width -= size;
	height -= size;
}