#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Injected by the Makefile from config.mk; fallbacks keep other tools happy. */
#ifndef PROJECT_NAME
#define PROJECT_NAME "program"
#endif
#ifndef PROJECT_VERSION
#define PROJECT_VERSION "0.0.0"
#endif

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 350

#define CASE_WIDTH 35
#define CASE_HEIGHT 35

int main(int argc, char *argv[]) {
	if(argc > 1 && strcmp(argv[1], "--version") == 0) {
		printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
		return EXIT_SUCCESS;
	}

	srand((unsigned int)time(nullptr)); // Initialize random number generator

	SDL_Window *window = nullptr;	  // The window we'll be rendering to
	SDL_Renderer *renderer = nullptr; // The surface contained by the window
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

	// Create rectangle and surface to manage images
	SDL_Rect rectbutton = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	SDL_Surface *surface = nullptr;

	// Load images
	SDL_Texture *textureCaseButton = nullptr;
	SDL_Texture *textureOne = nullptr;
	SDL_Texture *textureTwo = nullptr;
	SDL_Texture *textureThree = nullptr;
	SDL_Texture *textureFour = nullptr;
	SDL_Texture *textureFive = nullptr;
	SDL_Texture *textureSix = nullptr;
	SDL_Texture *textureSeven = nullptr;
	SDL_Texture *textureEight = nullptr;
	SDL_Texture *textureBomb = nullptr;
	SDL_Texture *textureEmpty = nullptr;
	SDL_Texture *textureFlag = nullptr;
	SDL_Texture *textureRedBomb = nullptr;

	surface = SDL_LoadBMP("img\\case.bmp");
	textureCaseButton = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\one.bmp");
	textureOne = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\two.bmp");
	textureTwo = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\three.bmp");
	textureThree = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\four.bmp");
	textureFour = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\five.bmp");
	textureFive = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\six.bmp");
	textureSix = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\seven.bmp");
	textureSeven = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\eight.bmp");
	textureEight = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\bomb.bmp");
	textureBomb = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\empty.bmp");
	textureEmpty = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\flag.bmp");
	textureFlag = SDL_CreateTextureFromSurface(renderer, surface);

	surface = SDL_LoadBMP("img\\redbomb.bmp");
	textureRedBomb = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Waiting a mouse or keyboard event. See SDL_scancode.h to know the word for each keys.
	// Quit application when ESC was pressed or window is closed
	while((SDL_WaitEvent(&event) >= 0) &&
		  (event.type != SDL_QUIT) &&
		  !(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
		// Test on mouse event
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				printf("Mouse event in (x=%d, y=%d), left click\n", event.button.x, event.button.y);
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				printf("Mouse event in (x=%d, y=%d), right click\n", event.button.x, event.button.y);
			} else if(event.button.button == SDL_BUTTON_MIDDLE) {
				printf("Mouse event in (x=%d, y=%d), middle click\n", event.button.x, event.button.y);
			}
		}

		// Test on keyboard event
		if(event.type == SDL_KEYDOWN) {
			SDL_Texture *selectedTexture = nullptr;

			switch(event.key.keysym.scancode) {
				case SDL_SCANCODE_1:
				case SDL_SCANCODE_KP_1:
					printf("1 down event\n");
					selectedTexture = textureOne;
					break;

				case SDL_SCANCODE_2:
				case SDL_SCANCODE_KP_2:
					printf("2 down event\n");
					selectedTexture = textureTwo;
					break;

				case SDL_SCANCODE_3:
				case SDL_SCANCODE_KP_3:
					printf("3 down event\n");
					selectedTexture = textureThree;
					break;

				case SDL_SCANCODE_4:
				case SDL_SCANCODE_KP_4:
					printf("4 down event\n");
					selectedTexture = textureFour;
					break;

				case SDL_SCANCODE_5:
				case SDL_SCANCODE_KP_5:
					printf("5 down event\n");
					selectedTexture = textureFive;
					break;

				case SDL_SCANCODE_6:
				case SDL_SCANCODE_KP_6:
					printf("6 down event\n");
					selectedTexture = textureSix;
					break;

				case SDL_SCANCODE_7:
				case SDL_SCANCODE_KP_7:
					printf("7 down event\n");
					selectedTexture = textureSeven;
					break;

				case SDL_SCANCODE_8:
				case SDL_SCANCODE_KP_8:
					printf("8 down event\n");
					selectedTexture = textureEight;
					break;

				case SDL_SCANCODE_B:
					printf("B down event\n");
					selectedTexture = textureBomb;
					break;

				case SDL_SCANCODE_C:
					printf("C down event\n");
					selectedTexture = textureCaseButton;
					break;

				case SDL_SCANCODE_E:
					printf("E down event\n");
					selectedTexture = textureEmpty;
					break;

				case SDL_SCANCODE_F:
					printf("F down event\n");
					selectedTexture = textureFlag;
					break;

				case SDL_SCANCODE_N: {
					printf("N down event\n");
					int random_number = (rand() % 10) + 1;
					printf(" the random number is : %d\n", random_number);
					break;
				}

				case SDL_SCANCODE_R:
					printf("R down event\n");
					selectedTexture = textureRedBomb;
					break;

				default:
					break;
			}

			// Show image at the 0,0 position
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
	if(textureCaseButton != nullptr) SDL_DestroyTexture(textureCaseButton);
	if(textureOne != nullptr) SDL_DestroyTexture(textureOne);
	if(textureTwo != nullptr) SDL_DestroyTexture(textureTwo);
	if(textureThree != nullptr) SDL_DestroyTexture(textureThree);
	if(textureFour != nullptr) SDL_DestroyTexture(textureFour);
	if(textureFive != nullptr) SDL_DestroyTexture(textureFive);
	if(textureSix != nullptr) SDL_DestroyTexture(textureSix);
	if(textureSeven != nullptr) SDL_DestroyTexture(textureSeven);
	if(textureEight != nullptr) SDL_DestroyTexture(textureEight);
	if(textureBomb != nullptr) SDL_DestroyTexture(textureBomb);
	if(textureEmpty != nullptr) SDL_DestroyTexture(textureEmpty);
	if(textureFlag != nullptr) SDL_DestroyTexture(textureFlag);
	if(textureRedBomb != nullptr) SDL_DestroyTexture(textureRedBomb);

	// Destroy SDL objects
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}
