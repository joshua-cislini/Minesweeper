#include "demineur.h"
#include <stdlib.h>

void creationGrid(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN]) {
	// Reset grid to default values
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			grid[row][col].bomb = false;
			grid[row][col].open = false;
			grid[row][col].flag = false;
		}
	}

	// Ensure NB_BOMBS doesn't exceed grid capacity
	int max_cells = MAX_LINE * MAX_COLUMN;
	int target_bombs = (NB_BOMBS > max_cells) ? max_cells : NB_BOMBS; // Still impossible to win but prevents errors

	// Randomly place exactly NB_BOMBS bombs
	for(int bombs_placed = 0; bombs_placed < target_bombs; /*do nothing here*/) {
		int row = rand() % MAX_LINE;
		int col = rand() % MAX_COLUMN;

		if(!grid[row][col].bomb) {
			grid[row][col].bomb = true;
			bombs_placed++;
		}
	}

	// Draw all cases of the Demineur
	SDL_Rect rect = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			rect.x = col * CASE_WIDTH;
			rect.y = row * CASE_HEIGHT;
			SDL_QueryTexture(texture[eCase], nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, texture[eCase], nullptr, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

void showBomb(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	SDL_Rect rect = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			if(grid[row][col].bomb) {
				rect.x = col * CASE_WIDTH;
				rect.y = row * CASE_HEIGHT;
				SDL_Texture *tex = (row == y && col == x) ? texture[eRedbomb] : texture[eBomb];
				SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
				SDL_RenderCopy(renderer, tex, nullptr, &rect);
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void checkCaseAround(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	if(x < 0 || x >= MAX_COLUMN || y < 0 || y >= MAX_LINE) {
		return;
	}

	// Do not open if flagged or already opened
	if(grid[y][x].flag || grid[y][x].open) {
		return;
	}

	grid[y][x].open = true;

	// Count neighboring bombs with boundary check (effets de bords)
	int bomb_count = 0;
	for(int dl = -1; dl <= 1; dl++) {
		for(int dc = -1; dc <= 1; dc++) {
			if(dl == 0 && dc == 0) {
				continue;
			}
			int nl = y + dl;
			int nc = x + dc;
			if(nl >= 0 && nl < MAX_LINE && nc >= 0 && nc < MAX_COLUMN) {
				if(grid[nl][nc].bomb) {
					bomb_count++;
				}
			}
		}
	}

	// Select the corresponding texture
	SDL_Texture *tex = nullptr;
	switch(bomb_count) {
		case 0: tex = texture[eEmpty]; break;
		case 1: tex = texture[eOne]; break;
		case 2: tex = texture[eTwo]; break;
		case 3: tex = texture[eThree]; break;
		case 4: tex = texture[eFour]; break;
		case 5: tex = texture[eFive]; break;
		case 6: tex = texture[eSix]; break;
		case 7: tex = texture[eSeven]; break;
		case 8: tex = texture[eEight]; break;
		default: tex = texture[eEmpty]; break;
	}

	SDL_Rect rect = {x * CASE_WIDTH, y * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
	SDL_RenderPresent(renderer);
}

void toggleFlag(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	if(x < 0 || x >= MAX_COLUMN || y < 0 || y >= MAX_LINE) {
		return;
	}

	// Cannot place or remove flag on an already open case
	if(grid[y][x].open) {
		return;
	}

	// Toggle flag state
	grid[y][x].flag = (bool)!grid[y][x].flag;

	// Display either the flag or the standard case
	SDL_Texture *tex = nullptr;
	if(grid[y][x].flag) {
		tex = texture[eFlag];
	} else {
		tex = texture[eCase];
	}
	SDL_Rect rect = {x * CASE_WIDTH, y * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
	SDL_RenderPresent(renderer);
}
