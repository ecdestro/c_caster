#include "texture.h"
#include <stdio.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename) {
    printf("Loading texture: %s\n", filename);
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL) {
        fprintf(stderr, "Failed to load texture %s! SDL_image Error: %s\n", filename, IMG_GetError());
    }
    return texture;
}

