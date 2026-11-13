#include "demineur.h"
#include <stdlib.h>

void creationGrid(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN]) {
	// Reset grid to 0
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			grid[row][col] = 0;
		}
	}

	// Ensure NB_BOMBS doesn't exceed grid capacity
	int max_cells = MAX_LINE * MAX_COLUMN;
	int target_bombs = (NB_BOMBS > max_cells) ? max_cells : NB_BOMBS; // Still impossible to win but prevents errors

	// Randomly place exactly NB_BOMBS bombs (value 1)
	for(int bombs_placed = 0; bombs_placed < target_bombs; /*do nothing here*/) {
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

void showBomb(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int col, int line) {
	SDL_Rect rect = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	for(int row = 0; row < MAX_LINE; row++) {
		for(int c = 0; c < MAX_COLUMN; c++) {
			if(grid[row][c] == 1) {
				rect.x = c * CASE_WIDTH;
				rect.y = row * CASE_HEIGHT;
				if(row == line && c == col) {
					SDL_RenderCopy(renderer, texture[TEX_REDBOMB], nullptr, &rect);
				} else {
					SDL_RenderCopy(renderer, texture[TEX_BOMB], nullptr, &rect);
				}
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void checkCaseAround(SDL_Renderer *renderer, SDL_Texture **texture, unsigned char grid[MAX_LINE][MAX_COLUMN], int col, int line) {
	if(col < 0 || col >= MAX_COLUMN || line < 0 || line >= MAX_LINE) {
		return;
	}

	// Count neighboring bombs with boundary check (effets de bords)
	int bomb_count = 0;
	for(int dl = -1; dl <= 1; dl++) {
		for(int dc = -1; dc <= 1; dc++) {
			if(dl == 0 && dc == 0) {
				continue;
			}
			int nl = line + dl;
			int nc = col + dc;
			if(nl >= 0 && nl < MAX_LINE && nc >= 0 && nc < MAX_COLUMN) {
				if(grid[nl][nc] == 1) {
					bomb_count++;
				}
			}
		}
	}

	// Select the corresponding texture
	SDL_Texture *tex = nullptr;
	switch(bomb_count) {
		case 0:
			tex = texture[TEX_EMPTY];
			break;
		case 1:
			tex = texture[TEX_ONE];
			break;
		case 2: tex = texture[TEX_TWO]; break;
		case 3: tex = texture[TEX_THREE]; break;
		case 4: tex = texture[TEX_FOUR]; break;
		case 5: tex = texture[TEX_FIVE]; break;
		case 6: tex = texture[TEX_SIX]; break;
		case 7: tex = texture[TEX_SEVEN]; break;
		case 8: tex = texture[TEX_EIGHT]; break;
		default: tex = texture[TEX_EMPTY]; break;
	}

	SDL_Rect rect = {col * CASE_WIDTH, line * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
	SDL_RenderPresent(renderer);
}
