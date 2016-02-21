#include "PoopGame.h"
#include <iostream>
#include <time.h>
#include <algorithm>

PoopGame::PoopGame() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Brown!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, resolution_x, resolution_y);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	Init();
}

void PoopGame::Init() {
	if (fontSheetTexture == 0)
		fontSheetTexture = LoadTexture("pixel_font.png");
	srand(time(NULL));
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
	poops.clear();
	peoples.clear();

	score = STARTING_SCORE;
	gameState = 0;
	dragging = false;

	Poop* bigPoop = new Poop(INITIAL_POOP_SIZE, INITIAL_POOP_SIZE, 0.0f, 0.0f, 0.0f);
	bigPoop->setColor(130, 100, 80);
	bigPoop->draggable = true;
	entities.push_back(bigPoop);
	poops.push_back(bigPoop);

	for (int i = 0; i <50; i++) {
		float xDist = randFloat(0.0f, 1.0f);
		float yDist = 1.0f - xDist;
		float randXDistance = randFloat(0.0f, 0.3f);
		if (rand() % 2 == 0) {
			xDist *= -1.0f;
			randXDistance *= -1.0f;
		}
		float randYDistance = randFloat(0.0f, 0.3f);
		if (rand() % 2 == 0) {
			yDist *= -1.0f;
			randYDistance *= -1.0f;
		}
		People* peep = new People(PEOPLE_SIZE, PEOPLE_SIZE, xDist + randXDistance, yDist + randYDistance, 0.0f);
		if (rand() % 2 == 0) {
			peep->velocity_x = randFloat(PEOPLE_MIN_SPEED, PEOPLE_MAX_SPEED);
		}
		else {
			peep->velocity_y = randFloat(PEOPLE_MIN_SPEED, PEOPLE_MAX_SPEED);
		}
		if (rand() % 2 == 0) {
			peep->velocity_x *= -1;
			peep->velocity_y *= -1;
		}

		peep->setColor(200, 200, 200);
		entities.push_back(peep);
		peoples.push_back(peep);
	}
	

}

PoopGame::~PoopGame() {
	SDL_Quit();
}

bool PoopGame::isColliding(Entity* e1, Entity* e2) {
	float e1_y_max = e1->y + e1->height / 2;
	float e1_y_min = e1->y - e1->height / 2;
	float e1_x_max = e1->x + e1->width / 2;
	float e1_x_min = e1->x - e1->width / 2;

	float e2_y_max = e2->y + e2->height / 2;
	float e2_y_min = e2->y - e2->height / 2;
	float e2_x_max = e2->x + e2->width / 2;
	float e2_x_min = e2->x - e2->width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;

	return true;
}

bool PoopGame::isCollidingWithMouse(const Entity* e, int x, int y) {
	float mouse_x = (x - resolution_x / 2) / (float)(resolution_y / 2);
	float mouse_y = ((y - resolution_y / 2) * -1) / (float)(resolution_y / 2);
	if (mouse_x <= e->x + e->width / 2 && mouse_x >= e->x - e->width / 2) {
		if (mouse_y <= e->y + e->height / 2 && mouse_y >= e->y - e->height / 2) {
			return true;
		}
	}

	return false;
}

void PoopGame::Update(float elapsed) {
	for (size_t i = 0; i < entities.size(); i++) {
		for (size_t j = 0; j < entities.size(); j++) {
			if (i != j && isColliding(entities[i], entities[j])) {
				entities[i]->collision = entities[j];
			}
		}
	}
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_R) {
				Init();
				return;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			dragEntity = nullptr;
			for (size_t i = 0; i < entities.size(); i++) {
				if (entities[i]->draggable && isCollidingWithMouse(entities[i], event.button.x, event.button.y)) {
					dragEntity = entities[i];
					
				}
			}
			dragging = true;
			
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			dragEntity = nullptr;
			dragging = false;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			if (dragEntity != nullptr) {
				if (dragEntity->width > 0.0f && dragEntity->height > 0.0f) {
					dragEntity->x = (event.motion.x - resolution_x / 2) / (float)(resolution_y / 2);
					dragEntity->y = ((event.motion.y - resolution_y / 2) * -1) / (float)(resolution_y / 2);

					float randX = randFloat(dragEntity->x - dragEntity->width / 2.0f, dragEntity->x + dragEntity->width / 2.0f);
					float randY = randFloat(dragEntity->y - dragEntity->width / 2.0f, dragEntity->y + dragEntity->width / 2.0f);
					float randSize = randFloat(dragEntity->width / 4.0f - 0.02f, dragEntity->width / 4.0f + 0.02f);
					float colorR = rand() % 20 + 120;
					float colorG = rand() % 10 + 90;
					float colorB = rand() % 10 + 70;
					Poop* poop = new Poop(randSize, randSize, randX, randY, 0.0f);
					poop->setColor(colorR, colorG, colorB);
					entities.push_back(poop);
					poops.push_back(poop);

					dragEntity->width -= 0.002f;
					dragEntity->height -= 0.002f;
				}
			}
			if (dragging) {
				float mouse_x = (event.button.x - resolution_x / 2) / (float)(resolution_y / 2);
				float mouse_y = ((event.button.y - resolution_y / 2) * -1) / (float)(resolution_y / 2);
				
				for (auto poop : poops) {
					if (poop != dragEntity) {
						float dist = pow(pow(poop->x - mouse_x, 2) + pow(poop->y - mouse_y, 2), 0.5);

						float vector_x = (poop->x - mouse_x) / dist;
						float vector_y = (poop->y - mouse_y) / dist;

						if (dist < MOUSE_SMEAR_RADIUS) {
							poop->velocity_x = vector_x * POOP_PUSH_FACTOR;
							poop->velocity_y = vector_y * POOP_PUSH_FACTOR;
						}
					}
					
				}
			}
			
			
			
		}
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
		entities[i]->collision = nullptr;
	}


	
	entities.erase(std::remove_if(
		entities.begin(), entities.end(),
		[](Entity* ent) {
			return ent->killed;
		}),
		entities.end());

	poops.erase(std::remove_if(
		poops.begin(), poops.end(),
		[](Poop* poop) {
		return poop->killed;
	}),
		poops.end());

	if (score <= 0) gameState = 1;
	
	if (gameState == 0) {
		bool all_infected = true;
		for (auto peep : peoples) {
			if (!peep->infected) all_infected = false;
		}
		if (all_infected) {
			gameState = 2;
		}
	}

	if (gameState == 0) {
		if (poops.size() == 0) gameState = 1;
	}
	
}

void PoopGame::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();
	}
	if (gameState == 0)
		score--;
}

void PoopGame::Render() {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	glLoadIdentity();
	glTranslatef(-1.3f, 0.95f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.05, 0.0, 0.0, 0.0, 0.0, 1.0);

	if (gameState == 1) {
		glLoadIdentity();
		glTranslatef(-0.6f, -0.90f, 0.0f);
		DrawText(fontSheetTexture, "You lose! Press R to reset.", 0.05, 0.0, 1.0, 0.0, 0.0, 1.0);
	}
	else if (gameState == 2) {
		glLoadIdentity();
		glTranslatef(-0.6f, -0.90f, 0.0f);
		DrawText(fontSheetTexture, "You won! Press R to reset.", 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
	}
	
	

	SDL_GL_SwapWindow(displayWindow);
}

bool PoopGame::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

float PoopGame::randFloat(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

GLuint PoopGame::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

	return textureID;
}

void PoopGame::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), 0.5f* size, ((size + spacing) * i) + (-0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
}