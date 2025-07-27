#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Player* player_load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open player file %s\n", filename);
        return NULL;
    }

    Player* player = (Player*)malloc(sizeof(Player));
    if (!player) {
        fclose(file);
        return NULL;
    }

    char name_buffer[32];
    if (fscanf(file, "%s %f %f %f", name_buffer, &player->x, &player->y, &player->angle) != 4) {
        fprintf(stderr, "Error: Invalid player file format in %s\n", filename);
        free(player);
        fclose(file);
        return NULL;
    }

    // Initialize movement parameters
    player->move_speed = 0.05f;
    player->rotation_speed = 0.03f;

    fclose(file);
    return player;
}

void player_handle_input(Player* player, const Uint8* keystate, const Map* map) {
    if (!player || !keystate || !map) return;

    // Rotation
    if (keystate[SDL_SCANCODE_LEFT]) {
        player->angle -= player->rotation_speed;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        player->angle += player->rotation_speed;
    }

    // Forward/Backward movement
    float dx = cos(player->angle) * player->move_speed;
    float dy = sin(player->angle) * player->move_speed;
    float new_x = player->x;
    float new_y = player->y;

    if (keystate[SDL_SCANCODE_W]) {
        new_x += dx;
        new_y += dy;
    }
    if (keystate[SDL_SCANCODE_S]) {
        new_x -= dx;
        new_y -= dy;
    }

    // Strafing (Left/Right)
    if (keystate[SDL_SCANCODE_A]) {
        new_x += dy;
        new_y -= dx;
    }
    if (keystate[SDL_SCANCODE_D]) {
        new_x -= dy;
        new_y += dx;
    }

    // Collision detection
    // Check the new position's map cell. If it's not a wall, update the player's position.
    // This is a simple check; more advanced checks could be done separately for X and Y.
    int map_x = (int)new_x;
    int map_y = (int)new_y;

    if (map_x >= 0 && map_x < map->width && map_y >= 0 && map_y < map->height &&
        map->grid[map_y][map_x] != 'W') {
        player->x = new_x;
        player->y = new_y;
    }
}


void player_free(Player* player) {
    if (player) {
        free(player);
    }
}