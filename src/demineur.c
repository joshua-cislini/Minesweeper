#include "demineur.h"
#include <stdlib.h>

void creationGrid(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN]) {
	// Reset grid to 0
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			grid[row][col] = 0;
		}
	}

	// Randomly place exactly NB_BOMBS bombs (value 1)
	int bombs_placed = 0;
	while(bombs_placed < NB_BOMBS) {
		int row = rand() % MAX_LINE;
		int col = rand() % MAX_COLUMN;
		if(grid[row][col] == 0) {
			grid[row][col] = 1;
			bombs_placed++;
		}
	}

	// Draw all cases of the Demineur
	SDL_Rect rect = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			rect.x = col * CASE_WIDTH;
			rect.y = row * CASE_HEIGHT;
			SDL_QueryTexture(texture[TEX_CASE], nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, texture[TEX_CASE], nullptr, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

void showBomb(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	int clicked_col = (x >= MAX_COLUMN) ? (x / CASE_WIDTH) : x;
	int clicked_line = (y >= MAX_LINE) ? (y / CASE_HEIGHT) : y;

	SDL_Rect rect = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			if(grid[row][col] == 1) {
				rect.x = col * CASE_WIDTH;
				rect.y = row * CASE_HEIGHT;
				if(row == clicked_line && col == clicked_col) {
					SDL_RenderCopy(renderer, texture[TEX_REDBOMB], nullptr, &rect);
				} else {
					SDL_RenderCopy(renderer, texture[TEX_BOMB], nullptr, &rect);
				}
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void checkCaseAround(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	(void)grid;
	int clicked_col = (x >= MAX_COLUMN) ? (x / CASE_WIDTH) : x;
	int clicked_line = (y >= MAX_LINE) ? (y / CASE_HEIGHT) : y;

	SDL_Rect rect = {clicked_col * CASE_WIDTH, clicked_line * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
	SDL_RenderCopy(renderer, texture[TEX_EMPTY], nullptr, &rect);
	SDL_RenderPresent(renderer);
}
