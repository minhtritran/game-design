#include "Entity.h"
#include <iostream>
Entity::~Entity() {}
Entity::Entity(float height, float width, float x, float y, float rotation)
	: height(height), width(width), x(x), y(y), rotation(rotation) {}

const float Entity::getX() { return x; }
const float Entity::getY() { return y; }

void Entity::setColor(int r, int g, int b) {
	this->r = r / 255.0f;
	this->g = g / 255.0f;
	this->b = b / 255.0f;
}

void Entity::setTexture(unsigned int textureID, float u, float v, float sprite_width, float sprite_height) {
	usingTexture = true;
	this->textureID = textureID;
	this->u = u;
	this->v = v;
	this->sprite_width = sprite_width;
	this->sprite_height = sprite_height;
}

void Entity::Update(float elapsed) {
	
}

void Entity::Render() {
	if (usingTexture) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	if (r != -1) {
		GLfloat colors[12];
		for (int i = 0; i < 12; i += 3) {
			colors[i] = r;
			colors[i + 1] = g;
			colors[i + 2] = b;
		}
		glColorPointer(3, GL_FLOAT, 0, colors);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	
	if (usingTexture) {
		GLfloat quadUVs[] = { u, v, u, v + sprite_height, u + sprite_height, v + sprite_height, u + sprite_height, v };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	if (r != -1) glDisableClientState(GL_COLOR_ARRAY);
	if (usingTexture) glDisable(GL_TEXTURE_2D);
}