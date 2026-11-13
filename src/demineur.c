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

	// Randomly place exactly NB_BOMBS bombs
	for(int bombs_placed = 0; bombs_placed < NB_BOMBS; /*do nothing here*/) {
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

	// If no neighboring bombs, recursively reveal all valid unopened neighbors
	if(bomb_count == 0) {
		for(int dl = -1; dl <= 1; dl++) {
			for(int dc = -1; dc <= 1; dc++) {
				if(dl == 0 && dc == 0) {
					continue;
				}
				int nl = y + dl;
				int nc = x + dc;
				if(nl >= 0 && nl < MAX_LINE && nc >= 0 && nc < MAX_COLUMN) {
					if(!grid[nl][nc].open && !grid[nl][nc].flag) {
						checkCaseAround(renderer, texture, grid, nc, nl);
					}
				}
			}
		}
	}

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

bool checkWin(case_t grid[MAX_LINE][MAX_COLUMN]) {
	bool all_non_bombs_open = true;
	bool all_bombs_flagged = true;
	int flag_count = 0;
	int bomb_count = 0;

	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			if(grid[row][col].flag) {
				flag_count++;
			}
			if(grid[row][col].bomb) {
				bomb_count++;
				if(!grid[row][col].flag) {
					all_bombs_flagged = false;
				}
			} else {
				if(!grid[row][col].open) {
					all_non_bombs_open = false;
				}
			}
		}
	}

	bool flagged_win = (bool)((all_bombs_flagged && (flag_count == bomb_count)) != 0);
	return (bool)((flagged_win || all_non_bombs_open) != 0);
}

static void renderCell(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN], int x, int y) {
	if(x < 0 || x >= MAX_COLUMN || y < 0 || y >= MAX_LINE) {
		return;
	}

	SDL_Texture *tex = nullptr;
	if(grid[y][x].open) {
		if(grid[y][x].bomb) {
			tex = texture[eBomb];
		} else {
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
		}
	} else if(grid[y][x].flag) {
		tex = texture[eFlag];
	} else {
		tex = texture[eCase];
	}

	SDL_Rect rect = {x * CASE_WIDTH, y * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
}

void winAnimation(SDL_Renderer *renderer, SDL_Texture **texture, case_t grid[MAX_LINE][MAX_COLUMN]) {
	// 1. Auto-flag any unflagged bombs with a sequential reveal
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			if(grid[row][col].bomb && !grid[row][col].flag) {
				grid[row][col].flag = true;
				renderCell(renderer, texture, grid, col, row);
				SDL_RenderPresent(renderer);
				SDL_Delay(40);
			}
		}
	}

	// Make sure the entire board is freshly drawn
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			renderCell(renderer, texture, grid, col, row);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_Delay(50);

	// 2. Celebratory diagonal gold wave across the board
	for(int diag = 0; diag < MAX_LINE + MAX_COLUMN - 1; diag++) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 215, 0, 140);
		for(int row = 0; row < MAX_LINE; row++) {
			int col = diag - row;
			if(col >= 0 && col < MAX_COLUMN) {
				SDL_Rect rect = {col * CASE_WIDTH, row * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(35);

		// Restore tiles of this diagonal
		for(int row = 0; row < MAX_LINE; row++) {
			int col = diag - row;
			if(col >= 0 && col < MAX_COLUMN) {
				renderCell(renderer, texture, grid, col, row);
			}
		}
	}

	// 3. Celebratory emerald pulse on all flags
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 50, 205, 50, 160);
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			if(grid[row][col].flag) {
				SDL_Rect rect = {col * CASE_WIDTH, row * CASE_HEIGHT, CASE_WIDTH, CASE_HEIGHT};
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	SDL_RenderPresent(renderer);
	SDL_Delay(180);

	// 4. Final clean render of the board
	for(int row = 0; row < MAX_LINE; row++) {
		for(int col = 0; col < MAX_COLUMN; col++) {
			renderCell(renderer, texture, grid, col, row);
		}
	}
	SDL_RenderPresent(renderer);
}
