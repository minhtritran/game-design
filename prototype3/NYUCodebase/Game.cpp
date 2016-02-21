#include "Game.h"
#include <iostream>
#include <time.h>
#include <algorithm>

Game::Game() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("HotOrCold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

void Game::Init() {
	if (fontSheetTexture == 0)
		fontSheetTexture = LoadTexture("pixel_font.png");
	srand(time(NULL));

	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
	for (int i = 0; i < pulses.size(); i++) {
		delete pulses[i];
	}
	pulses.clear();
	pulser = nullptr;
	cells.clear();
	wave_front = 0.0f;
	wave_shade = 0.0f;


	score = 2000;
	gameState = 0;

	Pulser* pulser = new Pulser(0.0f, 0.0f, 0.2f);
	pulser->setColor(255, 255, 255);
	entities.push_back(pulser);

	for (int i = 0; i < NUM_CELLS; i++) {
		float radius, rad, x = 0, y = 0;
		
		while (!goodSpawn(x, y)) {
			radius = randFloat(0.25f, 0.7f);
			rad = randFloat(0.0f, 2 * M_PI);
			x = radius * cos(rad);
			y = radius * sin(rad);
		}

		float shade = randFloat(0.5f - CELL_STARTING_SHADE_DIVERSITY, 0.5f + CELL_STARTING_SHADE_DIVERSITY);
		
		Cell* cell = new Cell(x, y, CELL_SIZE);
		cell->setColor(std::make_tuple(1.0f, shade, shade));
		entities.push_back(cell);
		cells.push_back(cell);
	}

}

Game::~Game() {
	SDL_Quit();
}

void Game::Update(float elapsed) {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_R) {
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				
				
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (gameState != 0) {
				Init();
				return;
			}
			if (pulses.size() <= 0) {
				Entity* pulse = new Entity(0.0f, 0.0f, 0.2f);
				pulse->setColor(entities[0]->getCurrentColor());
				pulses.push_back(pulse);
				sendFollowupWave = true;
				wave_shade = get<1>(entities[0]->getCurrentColor());
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {

		}
		else if (event.type == SDL_MOUSEMOTION) {

		}
	}

	if (sendFollowupWave && followupWaveTimer > WAVE_WIDTH) {
		Entity* pulse = new Entity(0.0f, 0.0f, 0.2f);
		pulse->setColor(std::make_tuple(BACKGROUND_SHADE, BACKGROUND_SHADE, BACKGROUND_SHADE));
		pulse->isFollowupWave = true;
		pulses.push_back(pulse);
		followupWaveTimer = 0.0f;
		sendFollowupWave = false;
	}

	for (auto pulse : pulses) {
		pulse->Update(elapsed);
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	
	for (auto pulse : pulses) {
		if (!pulse->isFollowupWave) {
			if (pulse->radius > 2.5f) {
				wave_front = 0.0f;
				wave_shade = 0.0f;

				for (auto cell : cells) {
					Cell* cellptr = dynamic_cast<Cell*>(cell);
					cellptr->touched = false;
				}
			}
		}

	}

	float shade = -1.0f;
	bool wonGame = true;
	bool lostGame = true;
	for (auto cell : cells) {
		Cell* cellptr = dynamic_cast<Cell*>(cell);
		if (cellptr->dead) {
			cell->setColor(0,0,0);
		}
		if (!cellptr->dead) {
			lostGame = false;
			if (shade == -1.0f) shade = cellptr->g;
			else {
				if (cellptr->g != shade) {
					
					wonGame = false;
				}
			}
		}
	}

	if (wonGame && !lostGame) {
		score = 0.0f;
		gameState = 1;
		for (auto cell : cells) {
			Cell* cellptr = dynamic_cast<Cell*>(cell);
			if (!cellptr->dead) score++;
		}
	}
	else if (lostGame) {
		gameState = 2;
	}

	pulses.erase(std::remove_if(
		pulses.begin(), pulses.end(),
		[](Entity* ent) {
		return ent->radius > 2.5f;
	}),
		pulses.end());

}

void Game::FixedUpdate(float elapsed) {

	if (sendFollowupWave) {
		followupWaveTimer += FIXED_TIMESTEP;
	}
	
	for (auto pulse : pulses) {
		pulse->FixedUpdate(elapsed);
		pulse->radius += WAVE_SPEED;
		if (!pulse->isFollowupWave) wave_front = pulse->radius;	
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate(elapsed);
	}

	for (auto cell : cells) {
		Cell* cellptr = dynamic_cast<Cell*>(cell);
		if (!cellptr->touched) {
			float dist = pow(pow(cell->x, 2) + pow(cell->y, 2), 0.5);
			if (dist < CELL_SIZE + wave_front / 2) {
				float diff = wave_shade - cellptr->g;
				if (diff <= -CELL_COLOR_LOCK_TOLERANCE) {
					cell->setColor(make_tuple(1.0f, cellptr->g - CELL_CHANGE_STEP, cellptr->g - CELL_CHANGE_STEP));
				}
				else if (diff >= CELL_COLOR_LOCK_TOLERANCE) {
					cell->setColor(make_tuple(1.0f, cellptr->g + CELL_CHANGE_STEP, cellptr->g + CELL_CHANGE_STEP));
				}
				else if (abs(diff) < CELL_COLOR_LOCK_TOLERANCE) {
					cell->setColor(make_tuple(1.0f, wave_shade, wave_shade));
				}
				
				cellptr->touched = true;
			}
		}
		
	}
}

void Game::Render() {

	glClearColor(BACKGROUND_SHADE, BACKGROUND_SHADE, BACKGROUND_SHADE, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (auto pulse : pulses) {
		pulse->Render();
	}
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	if (gameState == 1) {
		glLoadIdentity();
		glTranslatef(-1.15f, -0.75f, 0.0f);
		DrawText(fontSheetTexture, to_string(score) + "/" + to_string(NUM_CELLS), 0.15f, 0.0, 1.0, 1.0, 1.0, 1.0);

		glLoadIdentity();
		glTranslatef(-1.2f, -0.9f, 0.0f);
		DrawText(fontSheetTexture, "You survived. Click to reset.", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	}
	else if (gameState == 2) {
		glLoadIdentity();
		glTranslatef(-1.2f, -0.8f, 0.0f);
		DrawText(fontSheetTexture, "All your cells died.", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);

		glLoadIdentity();
		glTranslatef(-1.2f, -0.9f, 0.0f);
		DrawText(fontSheetTexture, "Click to reset.", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	}


	SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate(elapsed);
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}
bool Game::isColliding(Entity* e1, Entity* e2) {
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
bool Game::isCollidingCircle(Entity* e1, Entity* e2) {
	float dist = pow(pow(e1->x - e2->x, 2) + pow(e2->y - e2->y, 2), 0.5);
	if (dist < e1->radius + e2->radius)
		return true;

	return false;
}
bool Game::isCollidingWithMouse(const Entity* e, int x, int y) {
	float mouse_x = (x - resolution_x / 2) / (float)(resolution_y / 2);
	float mouse_y = ((y - resolution_y / 2) * -1) / (float)(resolution_y / 2);
	if (mouse_x <= e->x + e->width / 2 && mouse_x >= e->x - e->width / 2) {
		if (mouse_y <= e->y + e->height / 2 && mouse_y >= e->y - e->height / 2) {
			return true;
		}
	}

	return false;
}
bool Game::goodSpawn(float x, float y) {
	if (x == 0 || y == 0 || x < -1.33f / 2 + CELL_SIZE || x > 1.3f / 2 - CELL_SIZE || y < -1.0f / 2 + CELL_SIZE || y > 1.0f / 2 - CELL_SIZE) {
		return false;
	}

	for (auto cell : cells) {
		float dist = pow(pow(x - cell->x, 2) + pow(y - cell->y, 2), 0.5);
		if (dist < CELL_SIZE + cell->radius)
			return false;
	}

	return true;

}
float Game::randFloat(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}
GLuint Game::LoadTexture(const char *image_path) {
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
void Game::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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