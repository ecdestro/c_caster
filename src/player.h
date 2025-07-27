#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h> // Include SDL for Uint8 and other definitions
#include "map.h"

// Represents the player's state.
typedef struct {
    float x;      // X position in map coordinates
    float y;      // Y position in map coordinates
    float angle;  // Viewing angle in radians
    float move_speed;
    float rotation_speed;
} Player;

/**
 * @brief Loads the player's starting state from a file.
 * 
 * @param filename The path to the player data file.
 * @return A pointer to the loaded Player, or NULL on failure.
 */
Player* player_load(const char* filename);

/**
 * @brief Handles keyboard input to move and rotate the player.
 * 
 * @param player The player to update.
 * @param keystate The current state of the keyboard.
 * @param map The map for collision detection.
 */
void player_handle_input(Player* player, const Uint8* keystate, const Map* map);

/**
 * @brief Frees the memory allocated for a Player.
 * 
 * @param player A pointer to the Player to be freed.
 */
void player_free(Player* player);

#endif // PLAYER_H
