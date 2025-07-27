#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A portable implementation of strdup, as it's not standard C99
static char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* new_s = malloc(len);
    if (new_s) {
        memcpy(new_s, s, len);
    }
    return new_s;
}

// Helper function to determine map dimensions.
static void get_map_dimensions(FILE* file, int* width, int* height) {
    *width = 0;
    *height = 0;
    int current_width = 0;
    int c; // Use int to correctly handle EOF

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
    if (current_width > 0) {
        if (current_width > *width) {
            *width = current_width;
        }
        (*height)++;
    }
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

    Map* map = (Map*)malloc(sizeof(Map));
    if (!map) { fclose(file); return NULL; }
    map->width = width;
    map->height = height;

    map->grid = (char**)malloc(height * sizeof(char*));
    if (!map->grid) {
        free(map);
        fclose(file);
        return NULL;
    }
    // Initialize pointers to NULL for safer cleanup in case of allocation failure
    for (int i = 0; i < height; ++i) map->grid[i] = NULL;

    char* line_buffer = (char*)malloc(width + 2);
    if (!line_buffer) {
        map_free(map);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < height; ++i) {
        if (fgets(line_buffer, width + 2, file) == NULL) {
            map_free(map);
            free(line_buffer);
            fclose(file);
            return NULL;
        }
        line_buffer[strcspn(line_buffer, "\n")] = 0;

        map->grid[i] = my_strdup(line_buffer); // Use our portable version
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
    if (!map) return;
    if (map->grid) {
        // The height is known, so we can safely iterate
        for (int i = 0; i < map->height; ++i) {
            if (map->grid[i]) { // Check if pointer is not NULL before freeing
                free(map->grid[i]);
            }
        }
        free(map->grid);
    }
    free(map);
}
