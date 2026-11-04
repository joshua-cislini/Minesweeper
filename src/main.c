#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 350
#define CASE_WIDTH 35
#define CASE_HEIGHT 35
/**
 * Fonction utilitaire pour charger un fichier image BMP sous forme de SDL_Texture.
 */
static SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path) {
	SDL_Surface *surface = SDL_LoadBMP(path);
	if(surface == NULL) {
		printf("Erreur de chargement de l'image %s : %s\n", path, SDL_GetError());
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(texture == NULL) {
		printf("Erreur lors de la creation de la texture %s : %s\n", path, SDL_GetError());
	}
	return texture;
}

int main(void) {
	// Initialisation du generateur de nombres
	// pseudo-aleatoires
	srand((unsigned int)time(NULL));
	// The window we'll be rendering to
	SDL_Window *window = NULL;
	// The renderer contained by the window
	SDL_Renderer *renderer = NULL;
	// To wait on event
	SDL_Event event;
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	// Create window
	window = SDL_CreateWindow("Demineur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}
	// Chargement des textures des images
	SDL_Texture *textureCaseButton = load_texture(renderer, "img\\case.bmp");
	SDL_Texture *textureOne = load_texture(renderer, "img\\one.bmp");
	SDL_Texture *textureTwo = load_texture(renderer, "img\\two.bmp");
	SDL_Texture *textureThree = load_texture(renderer, "img\\three.bmp");
	SDL_Texture *textureFour = load_texture(renderer, "img\\four.bmp");
	SDL_Texture *textureFive = load_texture(renderer, "img\\five.bmp");
	SDL_Texture *textureSix = load_texture(renderer, "img\\six.bmp");
	SDL_Texture *textureSeven = load_texture(renderer, "img\\seven.bmp");
	SDL_Texture *textureEight = load_texture(renderer, "img\\eight.bmp");
	SDL_Texture *textureBomb = load_texture(renderer, "img\\bomb.bmp");
	SDL_Texture *textureEmpty = load_texture(renderer, "img\\empty.bmp");
	SDL_Texture *textureFlag = load_texture(renderer, "img\\flag.bmp");
	SDL_Texture *textureRedBomb = load_texture(renderer, "img\\redbomb.bmp");
	// Rectangle pour gerer l'affichage de l'image a la position (0, 0)
	SDL_Rect rectbutton = {0, 0, CASE_WIDTH, CASE_HEIGHT};
	// Affichage initial de la case par defaut
	if(textureCaseButton != NULL) {
		SDL_RenderClear(renderer);
		rectbutton.x = 0;
		rectbutton.y = 0;
		SDL_QueryTexture(textureCaseButton, NULL, NULL, &rectbutton.w, &rectbutton.h);
		SDL_RenderCopy(renderer, textureCaseButton, NULL, &rectbutton);
		SDL_RenderPresent(renderer);
	}

	// Waiting a mouse or keyboard event.
	// Quit application when ESC is pressed or when the window close button is clicked.
	int running = 1;
	while(running && (SDL_WaitEvent(&event) >= 0)) {
		// Fermeture de la fenetre (croix)
		if(event.type == SDL_QUIT) {
			running = 0;
			break;
		}
		// Gestion des clics de souris
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				printf("Clic souris : bouton GAUCHE (x=%d, y=%d)\n", event.button.x, event.button.y);
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				printf("Clic souris : bouton DROIT (x=%d, y=%d)\n", event.button.x, event.button.y);
			} else if(event.button.button == SDL_BUTTON_MIDDLE) {
				printf("Clic souris : bouton MILIEU (x=%d, y=%d)\n", event.button.x, event.button.y);
			}
		}

		// Gestion des touches du clavier
		if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				running = 0;
				break;
			}
			SDL_Texture *textureToDisplay = NULL;
			switch(event.key.keysym.scancode) {
				case SDL_SCANCODE_1:
				case SDL_SCANCODE_KP_1:
					printf("Touche 1 pressee\n");
					textureToDisplay = textureOne;
					break;
				case SDL_SCANCODE_2:
				case SDL_SCANCODE_KP_2:
					printf("Touche 2 pressee\n");
					textureToDisplay = textureTwo;
					break;
				case SDL_SCANCODE_3:
				case SDL_SCANCODE_KP_3:
					printf("Touche 3 pressee\n");
					textureToDisplay = textureThree;
					break;
				case SDL_SCANCODE_4:
				case SDL_SCANCODE_KP_4:
					printf("Touche 4 pressee\n");
					textureToDisplay = textureFour;
					break;
				case SDL_SCANCODE_5:
				case SDL_SCANCODE_KP_5:
					printf("Touche 5 pressee\n");
					textureToDisplay = textureFive;
					break;
				case SDL_SCANCODE_6:
				case SDL_SCANCODE_KP_6:
					printf("Touche 6 pressee\n");
					textureToDisplay = textureSix;
					break;
				case SDL_SCANCODE_7:
				case SDL_SCANCODE_KP_7:
					printf("Touche 7 pressee\n");
					textureToDisplay = textureSeven;
					break;
				case SDL_SCANCODE_8:
				case SDL_SCANCODE_KP_8:
					printf("Touche 8 pressee\n");
					textureToDisplay = textureEight;
					break;
				case SDL_SCANCODE_B:
					printf("Touche B pressee\n");
					textureToDisplay = textureBomb;
					break;
				case SDL_SCANCODE_C:
					printf("Touche C pressee\n");
					textureToDisplay = textureCaseButton;
					break;
				case SDL_SCANCODE_E:
					printf("Touche E pressee\n");
					textureToDisplay = textureEmpty;
					break;
				case SDL_SCANCODE_F:
					printf("Touche F pressee\n");
					textureToDisplay = textureFlag;
					break;
				case SDL_SCANCODE_N: {
					printf("Touche N pressee\n");
					int random_number = (rand() % 10) + 1;
					printf("Nombre aleatoire genere (1 a 10) : %d\n", random_number);
					break;
				}
				case SDL_SCANCODE_R:
					printf("Touche R pressee\n");
					textureToDisplay = textureRedBomb;
					break;
				default:
					break;
			}
			// Si une texture correspond a la touche pressee, on l'affiche a la position (0, 0)
			if(textureToDisplay != NULL) {
				SDL_RenderClear(renderer);
				rectbutton.x = 0;
				rectbutton.y = 0;
				SDL_QueryTexture(textureToDisplay, NULL, NULL, &rectbutton.w, &rectbutton.h);
				SDL_RenderCopy(renderer, textureToDisplay, NULL, &rectbutton);
				SDL_RenderPresent(renderer);
			}
		}
	}

	// Liberation des textures
	if(textureCaseButton) SDL_DestroyTexture(textureCaseButton);
	if(textureOne) SDL_DestroyTexture(textureOne);
	if(textureTwo) SDL_DestroyTexture(textureTwo);
	if(textureThree) SDL_DestroyTexture(textureThree);
	if(textureFour) SDL_DestroyTexture(textureFour);
	if(textureFive) SDL_DestroyTexture(textureFive);
	if(textureSix) SDL_DestroyTexture(textureSix);
	if(textureSeven) SDL_DestroyTexture(textureSeven);
	if(textureEight) SDL_DestroyTexture(textureEight);
	if(textureBomb) SDL_DestroyTexture(textureBomb);
	if(textureEmpty) SDL_DestroyTexture(textureEmpty);
	if(textureFlag) SDL_DestroyTexture(textureFlag);
	if(textureRedBomb) SDL_DestroyTexture(textureRedBomb);
	// Destroy SDL objects
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}