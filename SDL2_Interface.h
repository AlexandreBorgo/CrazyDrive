#ifndef SDL2INTERFACE_H
#define SDL2INTERFACE_H

#include <SDL2/SDL_image.h>

SDL_Texture* loadTexture(SDL_Renderer *renderer, char* img_path) {
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, img_path);

	if(!texture) {
		printf("Unable to load texture: %s\n", SDL_GetError());
		exit(-1);
	}

	return texture;
}

SDL_Surface* loadSurface(char* img_path) {	
	SDL_Surface *surface = NULL;
	surface = IMG_Load(img_path);

	if(!surface) {
		printf("Unable to load surface: %s\n", SDL_GetError());
		exit(-1);
	}

	return surface;
}

SDL_Texture* getTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface) {
	SDL_Texture *texture;
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if(!texture) {
		printf("Unable to load texture: %s\n", SDL_GetError());
		exit(-1);
	}
	return texture;
}

#endif
