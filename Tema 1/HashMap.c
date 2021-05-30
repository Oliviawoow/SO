#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10000

typedef struct data {
    char *key;
    char *value;
    struct data *next;
} data;

typedef struct {
    data **entries;
} myhash;

/* my hash function */
int hash(const char *key) {
    long int value = 0;
    int i;
    int key_len = strlen(key);

    for (i = 0; i < key_len; ++i) {
        value += key[i] * 3;
    }

    value = value % HASH_SIZE;
    return value;
}

/* allocates memory for my data */
data *hashmap_pair(const char *key, const char *value) {
    data *entry = malloc(sizeof(data) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    strcpy(entry->key, key);
    strcpy(entry->value, value);
    entry->next = NULL;
    return entry;
}

/* create my HashMap that is NULL */
myhash *hashmap_create(void) {
    int i;
    myhash *hashmap = malloc(sizeof(myhash));
    hashmap->entries = malloc(sizeof(data*) * HASH_SIZE);

    for (i = 0; i < HASH_SIZE; ++i) {
        hashmap->entries[i] = NULL;
    }

    return hashmap;
}

/* insert value in my hashmap */
void hashmap_set(myhash *hashtable, const char *key, const char *value) {
    int slot = hash(key);
    data *entry = hashtable->entries[slot];

    if (entry == NULL) {
        hashtable->entries[slot] = hashmap_pair(key, value);
        return;
    }
    data *prev;

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        }
        prev = entry;
        entry = prev->next;
    }

    prev->next = hashmap_pair(key, value);
}

/* look for the value with a key */
char *hashmap_get(myhash *hashtable, const char *key) {
    int slot = hash(key);
    data *entry = hashtable->entries[slot];

    if (entry == NULL) {
        return NULL;
    }

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}
