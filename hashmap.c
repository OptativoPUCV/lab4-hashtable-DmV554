#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  /*long casilla = hash(key, map->capacity);

  Pair *nuevoPair = createPair(key, value);

  if(map->buckets[casilla] == NULL || map->buckets[casilla]->key == NULL) {
    map->buckets[casilla] = nuevoPair;
  } else {
    while (map->buckets[casilla] != NULL && map->buckets[casilla]->key != NULL)
  { if (strcmp(map->buckets[casilla]->key, key) == 0) {
        map->buckets[casilla]->value = value;
        return;
      }
      casilla = (casilla + 1) % map->capacity;
    }

    map->buckets[casilla] = nuevoPair;
  }

  map->size += 1;*/

  long casilla = hash(key, map->capacity);

  while (map->buckets[casilla] != NULL && map->buckets[casilla]->key != NULL) {
    if (strcmp(map->buckets[casilla]->key, key) == 0) {
      map->buckets[casilla]->value = value;
      return;
    }
    casilla = (casilla + 1) % map->capacity;
  }

  // if (map->buckets[casilla] == NULL || map->buckets[casilla]->key == NULL) {
  Pair *newPair = createPair(key, value);
  map->buckets[casilla] = newPair;

  map->size += 1;
  //}
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)

  Pair** viejoBuckets = map->buckets;
  int capacidadVieja = map->capacity;

  map->capacity *= 2;
 // map->buckets = calloc(map->capacity, sizeof(Pair*));
    map->buckets = realloc(map->buckets, map->capacity);


  /*map->size = 0;

  for(int i=0; i < capacidadVieja; i++) {
       Pair* elemTabla = viejoBuckets[i];
        if(elemTabla != NULL){
            insertMap(map, elemTabla->key, elemTabla->value);
        }
    }*/

  free(viejoBuckets);

}

HashMap *createMap(long capacity) {
  HashMap *mapa = malloc(sizeof(HashMap));
  mapa->buckets = malloc(sizeof(Pair) * capacity);
  for (int i = 0; i < capacity; i++) {
    mapa->buckets[i] = NULL;
  }
  mapa->capacity = capacity;
  mapa->size = 0;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap *map, char *key) {
  Pair *datoABorrar = searchMap(map, key);

  if (datoABorrar != NULL) {
    datoABorrar->key = NULL;
    map->size--;
  }
}

Pair *searchMap(HashMap *map, char *key) {
  long casilla = hash(key, map->capacity);

  while (map->buckets[casilla] != NULL && map->buckets[casilla]->key != NULL) {
    map->current = casilla;
    if (strcmp(map->buckets[casilla]->key, key) == 0) {
      Pair *pairRetornado = map->buckets[casilla];
      return pairRetornado;
    }
    casilla = (casilla + 1) % map->capacity;
  }

  return NULL;
}

Pair * firstMap(HashMap * map) {
   for (int i = 0; i < map->capacity; i++) {
        Pair * elemPair = map->buckets[i];
        if (elemPair != NULL && elemPair->key != NULL) {
            map->current = i;
            return elemPair;
        }
    }

    return NULL;
}

Pair *nextMap(HashMap *map) { 
  for (int i = map->current + 1; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
        map->current = i; 
        return map->buckets[i];
    }
  }
  return NULL; 
}
