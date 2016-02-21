#include "Entity.h"

Entity::~Entity() {}
Entity::Entity(float height, float width, float x, float y, float rotation)
	: height(height), width(width), x(x), y(y), rotation(rotation) {}

Entity::Entity(float x, float y, float radius)
	: x(x), y(y), radius(radius) {}

const float Entity::getX() { return x; }
const float Entity::getY() { return y; }

void Entity::setColor(int r, int g, int b) {
	this->r = r / 255.0f;
	this->g = g / 255.0f;
	this->b = b / 255.0f;
}

void Entity::setColor(std::tuple<float, float, float> color) {
	this->r = std::get<0>(color);
	this->g = std::get<1>(color);
	this->b = std::get<2>(color);
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

void Entity::FixedUpdate(float elapsed) {
	x += velocity_x * FIXED_TIMESTEP;
	y += velocity_y * FIXED_TIMESTEP;

	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);

	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
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


	int vertexCount = 200;
	if (radius != -1.0f) {

		//GLfloat* buffer = new GLfloat[vertexCount * 2](); // (x,y) for each vertex
		//std::vector<GLfloat> buffer(vertexCount * 2);
		GLfloat buffer[400];

		int idx = 0;

		// Center vertex for triangle fan
		buffer[idx++] = x;
		buffer[idx++] = y;

		// Outer vertices of the circle
		int outerVertexCount = vertexCount - 1;

		for (int i = 0; i < outerVertexCount; ++i) {
			float percent = (i / (float)(outerVertexCount - 1));
			float rad = percent * 2 * M_PI;

			//Vertex position
			float outer_x = x + radius * cos(rad);
			float outer_y = y + radius * sin(rad);

			buffer[idx++] = outer_x;
			buffer[idx++] = outer_y;
		}

		glVertexPointer(2, GL_FLOAT, 0, buffer);
	}
	else {
		GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };
		
		glVertexPointer(2, GL_FLOAT, 0, quad);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	

	if (r != -1 && radius != -1.0f) {
		GLfloat colors[600];
		for (int i = 0; i < 600; i += 3) {
			colors[i] = r;
			colors[i + 1] = g;
			colors[i + 2] = b;
		}
		glColorPointer(3, GL_FLOAT, 0, colors);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else {
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

	if (radius != -1.0f) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	}
	else {
		glDrawArrays(GL_QUADS, 0, 4);
	}
	

	glDisableClientState(GL_VERTEX_ARRAY);
	if (r != -1) glDisableClientState(GL_COLOR_ARRAY);
	if (usingTexture) glDisable(GL_TEXTURE_2D);
}

std::tuple<float, float, float> Entity::getCurrentColor() {
	return std::make_tuple(r, g, b);
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float Entity::linear(float t, float b, float c, float d) {
	c = c - b;
	return c*t / d + b;
}

float Entity::ease(float v0, float v1, float t) {
	float y;
	int difficulty = 13;
	float difficulty2 = 0.3;
	if (t <= 0.5f) {
		y = pow(t, difficulty) * (0.5f / pow(0.5f, difficulty));
	}
	else {
		t -= 0.5f;
		y = pow(t, difficulty2) * (0.5f / pow(0.5f, difficulty2));
		//y = pow(t, 0.2);
		y += 0.5f;
	}

	return (1.0f - y)*v0 + y*v1;
}

float Entity::easeExpo(float t, float b, float c, float d) {
	c = c - b;
	t /= d / 2;
	if (t < 1) return c / 2 * pow(2, PULSE_SPEED * (t - 1)) + b;
	t--;
	return c / 2 * (-pow(2, -PULSE_SPEED * t) + 2) + b;
};