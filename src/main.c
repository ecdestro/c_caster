#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "map.h"
#include "player.h"

// Screen dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

// Minimap settings
#define TILE_SIZE 10
#define PLAYER_SIZE 4

void draw_map(SDL_Renderer* renderer, const Map* map) {
    if (!map) return;

    for (int y = 0; y < map->height; ++y) {
        for (int x = 0; x < map->width; ++x) {
            SDL_Rect tile_rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (map->grid[y][x]) {
                case 'W': SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
                default:  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); break;
            }
            SDL_RenderFillRect(renderer, &tile_rect);
        }
    }
}

void draw_player(SDL_Renderer* renderer, const Player* player) {
    if (!player) return;

    // Draw player body
    SDL_Rect player_rect = {
        (int)(player->x * TILE_SIZE) - (PLAYER_SIZE / 2),
        (int)(player->y * TILE_SIZE) - (PLAYER_SIZE / 2),
        PLAYER_SIZE,
        PLAYER_SIZE
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderFillRect(renderer, &player_rect);

    // Draw direction line
    int line_length = 15;
    int start_x = (int)(player->x * TILE_SIZE);
    int start_y = (int)(player->y * TILE_SIZE);
    int end_x = start_x + (int)(cos(player->angle) * line_length);
    int end_y = start_y + (int)(sin(player->angle) * line_length);
    SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
}

int main(int argc, char* argv[]) {
    // --- Data Loading ---
    Map* map = map_load("data/level1.map");
    if (!map) return 1;
    printf("Map loaded: %d x %d\n", map->width, map->height);

    Player* player = player_load("data/level1.plr");
    if (!player) {
        map_free(map);
        return 1;
    }
    printf("Player loaded at: (%.2f, %.2f)\n", player->x, player->y);

    // --- SDL Initialization ---
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* ... error handling ... */ return 1; }
    window = SDL_CreateWindow("C Caster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) { /* ... error handling ... */ return 1; }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) { /* ... error handling ... */ return 1; }

    // --- Main Loop ---
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = 1;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_map(renderer, map);
        draw_player(renderer, player);

        SDL_RenderPresent(renderer);
    }

    // --- Cleanup ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    player_free(player);
    map_free(map);
    printf("Cleanup complete. Exiting.\n");

    return 0;
}