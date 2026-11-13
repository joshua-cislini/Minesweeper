#define SDL_MAIN_HANDLED

#include "definition.h"
#include "demineur.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Injected by the Makefile from config.mk; fallbacks keep other tools happy. */
#ifndef PROJECT_NAME
#define PROJECT_NAME "Minesweeper"
#endif
#ifndef PROJECT_VERSION
#define PROJECT_VERSION "0.1.0"
#endif

static const char *const texture_files[eNberOfTexture] = {
		[eEmpty] = "img\\empty.bmp",
		[eOne] = "img\\one.bmp",
		[eTwo] = "img\\two.bmp",
		[eThree] = "img\\three.bmp",
		[eFour] = "img\\four.bmp",
		[eFive] = "img\\five.bmp",
		[eSix] = "img\\six.bmp",
		[eSeven] = "img\\seven.bmp",
		[eEight] = "img\\eight.bmp",
		[eCase] = "img\\case.bmp",
		[eFlag] = "img\\flag.bmp",
		[eBomb] = "img\\bomb.bmp",
		[eRedbomb] = "img\\redbomb.bmp",
};

int main(int argc, char *argv[]) {
	if(argc > 1 && strcmp(argv[1], "--version") == 0) {
		printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
		return EXIT_SUCCESS;
	}

	srand((unsigned int)time(nullptr)); // Initialize random number generator

	case_t grid[MAX_LINE][MAX_COLUMN] = {}; // 2D grid initialized to 0 / false
	int state = STATE_IDLE;					// Current game state

	SDL_Window *window = nullptr;	  // The window we'll be rendering to
	SDL_Renderer *renderer = nullptr; // The renderer contained by the window
	SDL_Event event;				  // To wait on event

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Create window
	window = SDL_CreateWindow(PROJECT_NAME " " PROJECT_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == nullptr) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Rect rectbutton = {0, 0, CASE_WIDTH, CASE_HEIGHT}; // Create rectangle to manage images
	SDL_Texture *textures[eNberOfTexture] = {nullptr};	   // Unique array of texture pointers

	for(int i = 0; i < eNberOfTexture; i++) {
		SDL_Surface *surface = SDL_LoadBMP(texture_files[i]);
		if(surface == nullptr) {
			printf("Unable to load image %s! SDL_Error: %s\n", texture_files[i], SDL_GetError());
		} else {
			textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
	}

	// ********** START OF THE GAME LOGIC *********
	// Waiting a mouse or keyboard event.
	// Quit application when ESC was pressed or window is closed
	while((SDL_WaitEvent(&event) >= 0) && (event.type != SDL_QUIT) && !(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
		// Test on mouse event
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			int col = x / CASE_WIDTH;
			int line = y / CASE_HEIGHT;

			printf("Mouse event button down\n");
			printf(" mouse x position : %d\n", x);
			printf(" mouse y position : %d\n", y);
			printf(" case index of column : %d\n", col);
			printf(" case index of line   : %d\n", line);

			if(event.button.button == SDL_BUTTON_LEFT) {
				printf(" left click\n");
				if(state == STATE_RUN && col >= 0 && col < MAX_COLUMN && line >= 0 && line < MAX_LINE) {
					if(!grid[line][col].flag && !grid[line][col].open) {
						if(grid[line][col].bomb) {
							showBomb(renderer, textures, grid, col, line);
							state = STATE_LOST;
							printf("GAME OVER!!!\n");
							printf("Press 's' to restart the game\n");
						} else {
							checkCaseAround(renderer, textures, grid, col, line);
						}
					}
				}
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				printf(" right click\n");
				if(state == STATE_RUN && col >= 0 && col < MAX_COLUMN && line >= 0 && line < MAX_LINE) {
					toggleFlag(renderer, textures, grid, col, line);
				}
			} else if(event.button.button == SDL_BUTTON_MIDDLE) {
				printf(" middle click\n");
			}
		}

		// Test on keyboard event
		if(event.type == SDL_KEYDOWN) {
			SDL_Texture *selectedTexture = nullptr;

			switch(event.key.keysym.scancode) {
				case SDL_SCANCODE_1:
				case SDL_SCANCODE_KP_1:
					printf("1 down event\n");
					selectedTexture = textures[eOne];
					break;

				case SDL_SCANCODE_2:
				case SDL_SCANCODE_KP_2:
					printf("2 down event\n");
					selectedTexture = textures[eTwo];
					break;

				case SDL_SCANCODE_3:
				case SDL_SCANCODE_KP_3:
					printf("3 down event\n");
					selectedTexture = textures[eThree];
					break;

				case SDL_SCANCODE_4:
				case SDL_SCANCODE_KP_4:
					printf("4 down event\n");
					selectedTexture = textures[eFour];
					break;

				case SDL_SCANCODE_5:
				case SDL_SCANCODE_KP_5:
					printf("5 down event\n");
					selectedTexture = textures[eFive];
					break;

				case SDL_SCANCODE_6:
				case SDL_SCANCODE_KP_6:
					printf("6 down event\n");
					selectedTexture = textures[eSix];
					break;

				case SDL_SCANCODE_7:
				case SDL_SCANCODE_KP_7:
					printf("7 down event\n");
					selectedTexture = textures[eSeven];
					break;

				case SDL_SCANCODE_8:
				case SDL_SCANCODE_KP_8:
					printf("8 down event\n");
					selectedTexture = textures[eEight];
					break;

				case SDL_SCANCODE_B:
					printf("B down event\n");
					selectedTexture = textures[eBomb];
					break;

				case SDL_SCANCODE_C:
					printf("C down event\n");
					selectedTexture = textures[eCase];
					break;

				case SDL_SCANCODE_E:
					printf("E down event\n");
					selectedTexture = textures[eEmpty];
					break;

				case SDL_SCANCODE_F:
					printf("F down event\n");
					selectedTexture = textures[eFlag];
					break;

				case SDL_SCANCODE_N: {
					printf("N down event\n");
					int random_number = (rand() % 10) + 1;
					printf(" the random number is : %d\n", random_number);
					break;
				}

				case SDL_SCANCODE_R:
					printf("R down event\n");
					selectedTexture = textures[eRedbomb];
					break;

				case SDL_SCANCODE_S: {
					if(state == STATE_IDLE || state == STATE_LOST || state == STATE_WIN) {
						printf("Start\n");
						creationGrid(renderer, textures, grid);
						state = STATE_RUN;
					}
					break;
				}

				default:
					break;
			}

			// Show single image at the 0,0 position if a single texture was selected
			if(selectedTexture != nullptr) {
				rectbutton.x = 0;
				rectbutton.y = 0;
				SDL_QueryTexture(selectedTexture, nullptr, nullptr, &rectbutton.w, &rectbutton.h);
				SDL_RenderCopy(renderer, selectedTexture, nullptr, &rectbutton);
				SDL_RenderPresent(renderer);
			}
		}
	}

	// Destroy textures
	for(int i = 0; i < eNberOfTexture; i++) {
		if(textures[i] != nullptr) {
			SDL_DestroyTexture(textures[i]);
		}
	}

	// Destroy SDL objects
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}
