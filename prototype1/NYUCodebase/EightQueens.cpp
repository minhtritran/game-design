#include "EightQueens.h"
#include <iostream>
#include <time.h>

EightQueens::EightQueens() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Eight Queens", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, resolution_x, resolution_y);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	
	done = false;
	lastFrameTicks = 0.0f;
	Init();
}

void EightQueens::Init() {
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
	gameState = 0;
	
	srand(time(NULL));
	queenTexture = LoadTexture("0_queen.png");
	kingTexture = LoadTexture("0_king.png");
	pawnTexture = LoadTexture("0_pawn.png");
	rookTexture = LoadTexture("0_rook.png");
	knightTexture = LoadTexture("0_knight.png");
	bishopTexture = LoadTexture("0_bishop.png");
	fontSheetTexture = LoadTexture("pixel_font.png");
	//spawn tiles
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Tile* tile = new Tile(0.2, 0.2, -1.1f + i / 4.7f, 0.8f - j / 4.7f, 0.0f);
			tile->setColor(70.0f, 70.0f, 70.0f);
			tile->xIndex = i;
			tile->yIndex = j;
			entities.push_back(tile);
			tiles[i][j] = tile;
		}
	}

	for (int i = 0; i < 4; i++) {
		float choice = rand() % 6;
		int randomX = rand() % 8;
		int randomY = rand() % 8;
		Piece* piece = new Piece(0.15f, 0.15f, 0.9f, 0.5f - i / 4.0f, 0.0f);
		if (choice == 0) {
			piece->type = "queen";
			piece->setTexture(queenTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (choice == 1) {
			piece->type = "king";
			piece->setTexture(kingTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (choice == 2) {
			piece->type = "pawn";
			piece->setTexture(pawnTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (choice == 3) {
			piece->type = "rook";
			piece->setTexture(rookTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (choice == 4) {
			piece->type = "knight";
			piece->setTexture(knightTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (choice == 5) {
			piece->type = "bishop";
			piece->setTexture(bishopTexture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		entities.push_back(piece);
		playPieceOnTile(piece, tiles[randomX][randomY], tiles, 1, false);
	}
	
	


}

EightQueens::~EightQueens() {
	SDL_Quit();
}

bool EightQueens::isColliding(Entity* e1, Entity* e2) {
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

bool EightQueens::isCollidingWithMouse(const Entity* e, int x, int y) {
	float mouse_x = (x - resolution_x / 2) / (float)(resolution_y / 2);
	float mouse_y = ((y - resolution_y / 2) * -1) / (float)(resolution_y / 2);
	if (mouse_x <= e->x + e->width / 2 && mouse_x >= e->x - e->width / 2) {
		if (mouse_y <= e->y + e->height / 2 && mouse_y >= e->y - e->height / 2) {
			return true;
		}
	}

	return false;
}

void EightQueens::Update(float elapsed) {
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
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			if (dragEntity != nullptr && dragEntity->collision != nullptr) {
				dragEntity->x = dragEntity->collision->x;
				dragEntity->y = dragEntity->collision->y;

				curTile = dynamic_cast<Tile*>(dragEntity->collision);
				curPiece = (Piece*)dragEntity;
				suggest = false;

			}

			dragEntity = nullptr;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			if (dragEntity != nullptr) {
				dragEntity->x = (event.motion.x - resolution_x / 2) / (float)(resolution_y / 2);
				dragEntity->y = ((event.motion.y - resolution_y / 2) * -1) / (float)(resolution_y / 2);
			}
			
		}
	}

	if (dragEntity != nullptr && dragEntity->collision != nullptr) {
		//curTile = (Tile*)dragEntity->collision;
		curTile = dynamic_cast<Tile*>(dragEntity->collision);
		curPiece = (Piece*)dragEntity;
		suggest = true;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles[i][j]->status == 3 || tiles[i][j]->status == 4) {
				tiles[i][j]->status = tiles[i][j]->oldStatus;
			}
		}
	}

	playPieceOnTile(curPiece, curTile, tiles, 2, suggest);
	
	bool nonDraggable = true;
	bool won = true;
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
		entities[i]->collision = nullptr;
		if (entities[i]->draggable) nonDraggable = false;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles[i][j]->status == 1) won = false;
		}
	}

	if (nonDraggable && won) gameState = 1;
	if (nonDraggable && !won) gameState = 2;


	curTile = nullptr;
	curPiece = nullptr;


}


void EightQueens::playPieceOnTile(Piece* curPiece, Tile* curTile, Tile* tiles[8][8], int status, bool suggest) {
	if (curTile != nullptr) {
		if (!suggest && status != 1) curPiece->draggable = false;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int xdiff = abs(curTile->xIndex - i);
				int ydiff = abs(curTile->yIndex - j);

				if (curPiece->type == "queen") {
					if (i == curTile->xIndex || j == curTile->yIndex || xdiff == ydiff) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

				if (curPiece->type == "king") {
					if (xdiff <= 1 && ydiff <= 1) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

				if (curPiece->type == "pawn") {
					if ((xdiff <= 1 || ydiff <= 1) && xdiff == ydiff) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

				if (curPiece->type == "rook") {
					if (i == curTile->xIndex || j == curTile->yIndex) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

				if (curPiece->type == "bishop") {
					if (xdiff == ydiff) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

				if (curPiece->type == "knight") {
					if ((xdiff + ydiff == 3 && xdiff != 0 && ydiff != 0) || xdiff == 0 && ydiff == 0) {
						tiles[i][j]->oldStatus = tiles[i][j]->status;
						if (suggest) {
							if (tiles[i][j]->status == 1) tiles[i][j]->status = 3;
							if (tiles[i][j]->status == 0) tiles[i][j]->status = 4;
						}
						else if (tiles[i][j]->status == 1 || status == 1) tiles[i][j]->status = status;
					}
				}

			}
		}
	}
}

void EightQueens::Render() {

	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	glLoadIdentity();
	glTranslatef(-0.6f, -0.90f, 0.0f);
	if (gameState == 1) {	
		DrawText(fontSheetTexture, "You win! Press R to reset.", 0.05, 0.0, 0.7, 1.0, 0.7, 1.0);
	}
	if (gameState == 2) {
		DrawText(fontSheetTexture, "You LOST! Press R to reset.", 0.05, 0.0, 1.0, 0.7, 0.7, 1.0);
	}
	

	SDL_GL_SwapWindow(displayWindow);
}
bool EightQueens::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	Update(elapsed);
	Render();
	return done;
}

GLuint EightQueens::LoadTexture(const char *image_path) {
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

void EightQueens::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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