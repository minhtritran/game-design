#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#include "Tile.h"
#include "Piece.h"
#include <math.h>
#include <string>

using namespace std;

class EightQueens {
public:
	EightQueens();
	~EightQueens();
	void Init();
	bool isColliding(Entity* e1, Entity* e2);
	bool isCollidingWithMouse(const Entity* e, int x, int y);
	void Update(float elapsed);
	void Render();
	bool UpdateAndRender();
	GLuint LoadTexture(const char *image_path);
	void playPieceOnTile(Piece* curPiece, Tile* curTile, Tile* tiles[8][8], int status, bool suggest);
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	int resolution_x = 800;
	int resolution_y = 600;
	
	GLuint fontSheetTexture;

	GLuint queenTexture;
	GLuint kingTexture;
	GLuint pawnTexture;
	GLuint rookTexture;
	GLuint knightTexture;
	GLuint bishopTexture;

	vector<Entity*> entities;
	Tile* tiles[8][8];
	Entity* dragEntity;

	Tile* curTile;
	Piece* curPiece;
	bool suggest;
	int gameState;
};