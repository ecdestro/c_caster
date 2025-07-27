#ifndef MAP_H
#define MAP_H

// Represents the game map.
typedef struct {
    char** grid; // 2D array for the map layout
    int width;
    int height;
} Map;

/**
 * @brief Loads a map from a file.
 * 
 * @param filename The path to the map file.
 * @return A pointer to the loaded Map, or NULL on failure.
 */
Map* map_load(const char* filename);

/**
 * @brief Frees the memory allocated for a Map.
 * 
 * @param map A pointer to the Map to be freed.
 */
void map_free(Map* map);

#endif // MAP_H
