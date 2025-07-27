#define _USE_MATH_DEFINES // For M_PI, though not always reliable
#include "renderer.h"
#include <math.h>

// If M_PI is not defined by the compiler, define it ourselves.
// This is the most portable solution.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define the 3D projection plane and field of view
#define RENDER_WIDTH 640
#define RENDER_HEIGHT 640
#define FOV (M_PI / 3.0) // 60 degrees field of view

void render_3d_view(SDL_Renderer* renderer, const Player* player, const Map* map) {
    // Iterate through every vertical column of the rendering area
    for (int x = 0; x < RENDER_WIDTH; ++x) {
        // --- 1. Calculate Ray Angle ---
        float ray_angle = (player->angle - FOV / 2.0f) + ((float)x / (float)RENDER_WIDTH) * FOV;

        // --- 2. Cast the Ray (DDA Algorithm) ---
        float distance_to_wall = 0;
        int hit_wall = 0;
        
        float eye_x = cos(ray_angle);
        float eye_y = sin(ray_angle);

        while (!hit_wall && distance_to_wall < 20.0f) {
            distance_to_wall += 0.1f;
            int test_x = (int)(player->x + eye_x * distance_to_wall);
            int test_y = (int)(player->y + eye_y * distance_to_wall);

            if (test_x < 0 || test_x >= map->width || test_y < 0 || test_y >= map->height) {
                hit_wall = 1;
                distance_to_wall = 20.0f;
            } else {
                if (map->grid[test_y][test_x] == 'W') {
                    hit_wall = 1;
                }
            }
        }

        // --- 3. Correct Fisheye Distortion ---
        float ca = player->angle - ray_angle;
        if (ca < 0) ca += 2 * M_PI;
        if (ca > 2 * M_PI) ca -= 2 * M_PI;
        distance_to_wall *= cos(ca);

        // --- 4. Calculate Wall Slice Height ---
        int line_height = (int)(RENDER_HEIGHT / distance_to_wall);
        int draw_start = -line_height / 2 + RENDER_HEIGHT / 2;
        if (draw_start < 0) draw_start = 0;
        int draw_end = line_height / 2 + RENDER_HEIGHT / 2;
        if (draw_end >= RENDER_HEIGHT) draw_end = RENDER_HEIGHT - 1;

        // --- 5. Draw the Slice ---
        Uint8 shade = 255 - (Uint8)(distance_to_wall * 10);
        if (shade < 50) shade = 50;
        
        SDL_SetRenderDrawColor(renderer, shade, shade, shade, 255);
        int screen_x = x + RENDER_WIDTH;
        SDL_RenderDrawLine(renderer, screen_x, draw_start, screen_x, draw_end);
    }
}