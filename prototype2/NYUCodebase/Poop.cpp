#include "Poop.h"

Poop::Poop(float height, float width, float x, float y, float rotation)
	: Entity(height, width, x, y, rotation) {
	friction_x = POOP_FRICTION;
	friction_y = POOP_FRICTION;
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