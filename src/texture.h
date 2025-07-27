#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @brief Loads an image file into an SDL_Texture.
 * 
 * @param renderer The renderer to create the texture with.
 * @param filename The path to the image file.
 * @return A pointer to the loaded SDL_Texture, or NULL on failure.
 */
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename);

#endif // TEXTURE_H
