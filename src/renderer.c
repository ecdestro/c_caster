#define _USE_MATH_DEFINES
#include "renderer.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RENDER_WIDTH 640
#define RENDER_HEIGHT 640
#define FOV (M_PI / 3.0)
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

void render_3d_view(SDL_Renderer* renderer, const Player* player, const Map* map, SDL_Texture* wall_texture) {
    for (int x = 0; x < RENDER_WIDTH; ++x) {
        float ray_angle = (player->angle - FOV / 2.0f) + ((float)x / (float)RENDER_WIDTH) * FOV;
        float ray_dir_x = cos(ray_angle);
        float ray_dir_y = sin(ray_angle);

        int map_x = (int)player->x;
        int map_y = (int)player->y;

        float side_dist_x, side_dist_y;
        
        float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        float perp_wall_dist;

        int step_x, step_y;
        int hit = 0;
        int side;

        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (player->x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - player->x) * delta_dist_x;
        }
        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (player->y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - player->y) * delta_dist_y;
        }

        while (hit == 0) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0; // Wall was hit on an X-side (vertical)
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1; // Wall was hit on a Y-side (horizontal)
            }
            if (map->grid[map_y][map_x] == 'W') hit = 1;
        }

        if (side == 0) {
            perp_wall_dist = (side_dist_x - delta_dist_x);
        } else {
            perp_wall_dist = (side_dist_y - delta_dist_y);
        }

        int line_height = (int)(RENDER_HEIGHT / perp_wall_dist);
        int draw_start = -line_height / 2 + RENDER_HEIGHT / 2;
        if (draw_start < 0) draw_start = 0;
        int draw_end = line_height / 2 + RENDER_HEIGHT / 2;
        if (draw_end >= RENDER_HEIGHT) draw_end = RENDER_HEIGHT - 1;

        // --- Texture Calculation ---
        double wall_x; // Where exactly the wall was hit
        if (side == 0) {
            wall_x = player->y + perp_wall_dist * ray_dir_y;
        } else {
            wall_x = player->x + perp_wall_dist * ray_dir_x;
        }
        wall_x -= floor(wall_x);

        // X coordinate on the texture
        int tex_x = (int)(wall_x * (double)TEXTURE_WIDTH);
        if (side == 0 && ray_dir_x > 0) tex_x = TEXTURE_WIDTH - tex_x - 1;
        if (side == 1 && ray_dir_y < 0) tex_x = TEXTURE_WIDTH - tex_x - 1;

        // --- Draw the Texture Slice ---
        SDL_Rect src_rect = { tex_x, 0, 1, TEXTURE_HEIGHT };
        SDL_Rect dest_rect = { x + RENDER_WIDTH, draw_start, 1, draw_end - draw_start };
        
        // Apply shading for distance
        Uint8 shade = 255 - (Uint8)(perp_wall_dist * 20);
        if (shade < 60) shade = 60;
        if (side == 1) shade = (shade * 2) / 3; // Make Y-sides darker
        SDL_SetTextureColorMod(wall_texture, shade, shade, shade);

        SDL_RenderCopy(renderer, wall_texture, &src_rect, &dest_rect);
    }
}
