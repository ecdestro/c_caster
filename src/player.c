#include "player.h"
#include <stdio.h>
#include <stdlib.h>

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

    // The file is expected to have a simple format: name x y angle
    // We are only interested in x, y, and angle for now.
    char name_buffer[32];
    if (fscanf(file, "%s %f %f %f", name_buffer, &player->x, &player->y, &player->angle) != 4) {
        fprintf(stderr, "Error: Invalid player file format in %s\n", filename);
        free(player);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return player;
}

void player_free(Player* player) {
    if (player) {
        free(player);
    }
}

