#ifndef DEMINEUR_H
#define DEMINEUR_H

#include "definition.h"
#include <SDL2/SDL.h>

void creationGrid(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN]);
void showBomb(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y);
void checkCaseAround(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y);
void toggleFlag(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y);
bool checkWin(case_t grid[MAX_LINE][MAX_COLUMN]);
void winAnimation(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN]);

#endif // DEMINEUR_H
