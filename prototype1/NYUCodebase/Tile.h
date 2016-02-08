#include "Entity.h"

class Tile : public Entity {
public:
	Tile(float height, float width, float x, float y, float rotation);
	void Update(float elapsed);
	void Render();

	bool touchingPiece;
	int xIndex;
	int yIndex;
	int status;
	int oldStatus;
};