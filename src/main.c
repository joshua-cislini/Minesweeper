#define SDL_MAIN_HANDLED

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

#define GRID_SIZE 20
#define CASE_WIDTH 35
#define CASE_HEIGHT 35

#define WINDOW_WIDTH (GRID_SIZE * CASE_WIDTH)
#define WINDOW_HEIGHT (GRID_SIZE * CASE_HEIGHT)

#define NB_BOMBS 10

enum TextureIndex {
	TEX_CASE = 0,
	TEX_ONE,
	TEX_TWO,
	TEX_THREE,
	TEX_FOUR,
	TEX_FIVE,
	TEX_SIX,
	TEX_SEVEN,
	TEX_EIGHT,
	TEX_BOMB,
	TEX_EMPTY,
	TEX_FLAG,
	TEX_REDBOMB,
	NB_TEXTURES,
};

static const char *const texture_files[NB_TEXTURES] = {
		"img\\case.bmp",
		"img\\one.bmp",
		"img\\two.bmp",
		"img\\three.bmp",
		"img\\four.bmp",
		"img\\five.bmp",
		"img\\six.bmp",
		"img\\seven.bmp",
		"img\\eight.bmp",
		"img\\bomb.bmp",
		"img\\empty.bmp",
		"img\\flag.bmp",
		"img\\redbomb.bmp",
};

int main(int argc, char *argv[]) {
	if(argc > 1 && strcmp(argv[1], "--version") == 0) {
		printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
		return EXIT_SUCCESS;
	}

	srand((unsigned int)time(nullptr)); // Initialize random number generator

	int grid[GRID_SIZE][GRID_SIZE] = {0}; // 2D grid initialized to 0

	SDL_Window *window = nullptr;	  // The window we'll be rendering to
	SDL_Renderer *renderer = nullptr; // The renderer contained by the window
	SDL_Event event;				  // To wait on event

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Create window
	window = SDL_CreateWindow("Demineur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
	SDL_Texture *textures[NB_TEXTURES] = {nullptr};		   // Unique array of texture pointers

	for(int i = 0; i < NB_TEXTURES; i++) {
		SDL_Surface *surface = SDL_LoadBMP(texture_files[i]);
		if(surface == nullptr) {
			printf("Unable to load image %s! SDL_Error: %s\n", texture_files[i], SDL_GetError());
		} else {
			textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
	}

	// Waiting a mouse or keyboard event.
	// Quit application when ESC was pressed or window is closed
	while((SDL_WaitEvent(&event) >= 0) &&
		  (event.type != SDL_QUIT) &&
		  !(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
		// Test on mouse event
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			int x = 0;
			int y = 0;
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
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				printf(" right click\n");
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
					selectedTexture = textures[TEX_ONE];
					break;

				case SDL_SCANCODE_2:
				case SDL_SCANCODE_KP_2:
					printf("2 down event\n");
					selectedTexture = textures[TEX_TWO];
					break;

				case SDL_SCANCODE_3:
				case SDL_SCANCODE_KP_3:
					printf("3 down event\n");
					selectedTexture = textures[TEX_THREE];
					break;

				case SDL_SCANCODE_4:
				case SDL_SCANCODE_KP_4:
					printf("4 down event\n");
					selectedTexture = textures[TEX_FOUR];
					break;

				case SDL_SCANCODE_5:
				case SDL_SCANCODE_KP_5:
					printf("5 down event\n");
					selectedTexture = textures[TEX_FIVE];
					break;

				case SDL_SCANCODE_6:
				case SDL_SCANCODE_KP_6:
					printf("6 down event\n");
					selectedTexture = textures[TEX_SIX];
					break;

				case SDL_SCANCODE_7:
				case SDL_SCANCODE_KP_7:
					printf("7 down event\n");
					selectedTexture = textures[TEX_SEVEN];
					break;

				case SDL_SCANCODE_8:
				case SDL_SCANCODE_KP_8:
					printf("8 down event\n");
					selectedTexture = textures[TEX_EIGHT];
					break;

				case SDL_SCANCODE_B:
					printf("B down event\n");
					selectedTexture = textures[TEX_BOMB];
					break;

				case SDL_SCANCODE_C:
					printf("C down event\n");
					selectedTexture = textures[TEX_CASE];
					break;

				case SDL_SCANCODE_E:
					printf("E down event\n");
					selectedTexture = textures[TEX_EMPTY];
					break;

				case SDL_SCANCODE_F:
					printf("F down event\n");
					selectedTexture = textures[TEX_FLAG];
					break;

				case SDL_SCANCODE_N: {
					printf("N down event\n");
					int random_number = (rand() % 10) + 1;
					printf(" the random number is : %d\n", random_number);
					break;
				}

				case SDL_SCANCODE_R:
					printf("R down event\n");
					selectedTexture = textures[TEX_REDBOMB];
					break;

				case SDL_SCANCODE_S: {
					printf("Start\n");

					// Reset 2D grid to 0
					for(int row = 0; row < GRID_SIZE; row++) {
						for(int col = 0; col < GRID_SIZE; col++) {
							grid[row][col] = 0;
						}
					}

					// Randomly place exactly 10 bombs (value 1)
					int bombs_placed = 0;
					while(bombs_placed < NB_BOMBS) {
						int row = rand() % GRID_SIZE;
						int col = rand() % GRID_SIZE;
						if(grid[row][col] == 0) {
							grid[row][col] = 1;
							bombs_placed++;
						}
					}

					// Draw all 10x10 cases of the Demineur
					for(int row = 0; row < GRID_SIZE; row++) {
						for(int col = 0; col < GRID_SIZE; col++) {
							rectbutton.x = col * CASE_WIDTH;
							rectbutton.y = row * CASE_HEIGHT;
							SDL_QueryTexture(textures[TEX_CASE], nullptr, nullptr, &rectbutton.w, &rectbutton.h);
							SDL_RenderCopy(renderer, textures[TEX_CASE], nullptr, &rectbutton);
						}
					}
					SDL_RenderPresent(renderer);
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
	for(int i = 0; i < NB_TEXTURES; i++) {
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
