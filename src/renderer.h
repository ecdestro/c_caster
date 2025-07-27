#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "player.h"
#include "map.h"

/**
 * @brief Renders the 3D first-person view.
 * 
 * @param renderer The main SDL renderer target.
 * @param player The player object containing position and angle.
 * @param map The map object for collision checks.
 */
void render_3d_view(SDL_Renderer* renderer, const Player* player, const Map* map);

#endif // RENDERER_H
