#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to determine map dimensions without loading it all at once.
static void get_map_dimensions(FILE* file, int* width, int* height) {
    *width = 0;
    *height = 0;
    int current_width = 0;
    char c;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            if (current_width > *width) {
                *width = current_width;
            }
            current_width = 0;
            (*height)++;
        } else {
            current_width++;
        }
    }
    // Account for the last line if it doesn't end with a newline
    if (current_width > 0) {
        if (current_width > *width) {
            *width = current_width;
        }
        (*height)++;
    }

    // Rewind the file to be read again
    rewind(file);
}

Map* map_load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open map file %s\n", filename);
        return NULL;
    }

    int width = 0, height = 0;
    get_map_dimensions(file, &width, &height);

    if (width == 0 || height == 0) {
        fprintf(stderr, "Error: Map file %s is empty or invalid.\n", filename);
        fclose(file);
        return NULL;
    }

    // Allocate the map structure
    Map* map = (Map*)malloc(sizeof(Map));
    if (!map) {
        fclose(file);
        return NULL;
    }
    map->width = width;
    map->height = height;

    // Allocate the grid rows
    map->grid = (char**)malloc(height * sizeof(char*));
    if (!map->grid) {
        free(map);
        fclose(file);
        return NULL;
    }

    // Allocate each row and read the file content
    char* line_buffer = (char*)malloc(width + 2); // +2 for newline and null terminator
    for (int i = 0; i < height; ++i) {
        if (fgets(line_buffer, width + 2, file) == NULL) {
            // Handle potential read error or premature EOF
            map_free(map); // Clean up partially allocated map
            free(line_buffer);
            fclose(file);
            return NULL;
        }
        // Strip newline character if present
        line_buffer[strcspn(line_buffer, "\n")] = 0;

        map->grid[i] = strdup(line_buffer);
        if (!map->grid[i]) {
            map_free(map);
            free(line_buffer);
            fclose(file);
            return NULL;
        }
    }

    free(line_buffer);
    fclose(file);
    return map;
}

void map_free(Map* map) {
    if (!map) {
        return;
    }
    if (map->grid) {
        for (int i = 0; i < map->height; ++i) {
            free(map->grid[i]);
        }
        free(map->grid);
    }
    free(map);
}