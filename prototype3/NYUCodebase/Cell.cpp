#include "Cell.h"
using namespace std;
Cell::Cell(float x, float y, float radius)
	: Entity(x, y, radius) {

}

void Cell::Update(float elapsed) {
	Entity::Update(elapsed);
	if (g <= 0.0f + CELL_DEATH_SENSITIVITY || g >= 1.0f - CELL_DEATH_SENSITIVITY) {
		dead = true;
	}
}

void Cell::FixedUpdate(float elapsed) {
	Entity::FixedUpdate(elapsed);
	
}

void Cell::Render() {
	Entity::Render();
}