#ifndef DEMINEUR_H
#define DEMINEUR_H

#include "definition.h"
#include <SDL2/SDL.h>

void creationGrid(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN]);
void showBomb(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int x, int y);
void checkCaseAround(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int x, int y);

#endif // DEMINEUR_H
