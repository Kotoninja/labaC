#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct _hash_map_cell
{
    char *key;
    int value;
} hash_map_cell_t;

struct _hash_map_t
{
    hash_map_cell_t *entries;
    size_t size;
};

static size_t hash(char *key)
{
    assert(key != NULL);

    size_t hash = 0;

    int i = 0;
    while (key[i] != '\0')
    {
        hash = hash * 67 + key[i];
        i++;
    }

    return hash;
}

static hash_map_t *hash_map_expand(hash_map_t *map)
{
    assert(map != NULL);

    hash_map_t *newMap = hash_map_create(map->size * 2);

    for (size_t index = 0; index < map->size; index++)
    {
        if (map->entries[index].key != NULL)
        {
            newMap = hash_map_insert(newMap, map->entries[index].key, map->entries[index].value);
        }
    }

    hash_map_free(map);
    return newMap;
}

hash_map_t *hash_map_create(size_t size)
{
    hash_map_t *map = malloc(sizeof(hash_map_t));
    assert(map != NULL && "Map is NULL when malloc");

    map->size = size;
    map->entries = calloc(size, sizeof(hash_map_cell_t));
    assert(map->entries != NULL && "map->entries is NULL when calloc");

    return map;
}

hash_map_t *hash_map_insert(hash_map_t *map, char *key, int value)
{
    assert(map != NULL);
    assert(key != NULL);

    size_t index = hash(key) % map->size;

    while (map->entries[index].key != NULL)
    {
        index++;

        if (index == map->size)
        {
            return hash_map_insert(hash_map_expand(map), key, value);
        }
    }

    map->entries[index].key = calloc(strlen(key) + 1, sizeof(char));
    assert(map->entries != NULL);
    map->entries[index].value = value;

    return map;
}

bool hash_map_key(hash_map_t *map, char *key)
{
    assert(map != NULL);
    assert(key != NULL);

    for (size_t index = hash(key) % map->size; index < map->entries; index++)
    {
        char current = map->entries[index].key;

        if (current == NULL)
            continue;

        if (!strcmp(current, key))
        {
            return true;
        }
    }

    return false;
}

int hash_map_get(hash_map_t *map, char *key)
{
    assert(map != NULL);
    assert(key != NULL);
    assert(_hash_map_key(map, key));

    for (size_t index = hash(key) % map->size; index < map->size; index++)
    {
        char current = map->entries[index].key;

        if (current == NULL)
            continue;
        else
            return map->entries[index].value;
    }
}

void hash_map_free(hash_map_t *map)
{
    assert(map != NULL);

    for (size_t index = 0; index < map->size; index++)
    {
        if (map->entries[index].key != NULL)
        {
            free(map->entries[index].key);
        }
    }

    free(map->entries);
    free(map);
}
