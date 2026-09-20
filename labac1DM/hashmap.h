#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>
#include <stdbool.h>

typedef struct _hash_map_t hash_map_t;

hash_map_t *hash_map_create(size_t size);                           // Initialization the hashmap
hash_map_t *hash_map_insert(hash_map_t *map, char *key, int value); // Insert a value into the hashmap with key
bool hash_map_key(hash_map_t *map, char *key);                      // Check, if key exist in a hashmap
int hash_map_get(hash_map_t *map, char *key);                       // Get value by key
void hash_map_free(hash_map_t *map);                                // free() hashmap
#endif