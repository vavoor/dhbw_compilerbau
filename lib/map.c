#include "map.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_MAP_SIZE 101

typedef struct MapElement_R* MapElement;


/* The hash table is implemented as an array of linked lists. The hash value is used as
index into the array. */
struct Map_R {
  int size; /* size of the hash table */
  int count; /* number of elements in the map */
  MapElement* elements; /* array with size elements */
};


/* MapElement_R is an entry in the hash table implemented as element in the linked list. */
struct MapElement_R {
  const char* key;
  void* value;
  MapElement next;
};


/* Calculates the hash value of key, which is in the range [0..size). */
static int hash(const char* key, int size)
{
  int value = 0;
  const char* p;

  assert(key!=NULL);
  assert(size>0);

  for (p=key; *p!=0; p++) {
    value = ((value<<5)|(((int)*p)&0x0000001F))%size;
  }

  return value;
}


/* Looks up an element by key in the hash table. Returns a pointer to the pointer that
points at the element if it's present or where the element has to be inserted if it's not
present. */
static MapElement* findElementHandle(const char* key, Map map, int* found)
{
  int hashvalue = hash(key,map->size);
  MapElement* element = &map->elements[hashvalue]; /* pointer to the slot in the hash table */

  while (*element!=NULL) {
    if (strcmp(key,(*element)->key)==0) {
      *found = 1;
      return element;
    }
    element = &(*element)->next;
  }

  *found = 0;
  return element;
}


static MapElement findInList(const char* key, MapElement first)
{
  MapElement elem = first;
  while (elem!=NULL) {
    if (strcmp(key,elem->key)==0) {
      return elem;
    }
    elem = elem->next;
  }

  return NULL;
}


static MapElement newElement(const char* key, void* value)
{
  MapElement element = malloc(sizeof(struct MapElement_R));
  assert(element!=NULL);

  element->key = key;
  element->value = value;
  element->next = NULL;

  return element;
}


Map newMap(int size)
{
  Map map;

  map = malloc(sizeof(struct Map_R));
  assert(map!=NULL);

  if (size>0) {
    map->size = size;
  }
  else {
    map->size = DEFAULT_MAP_SIZE;
  }

  map->count = 0;
  map->elements = calloc(map->size,sizeof(MapElement));
  assert(map->elements!=NULL);

  return map;
}


void deleteMap(Map map, int freeKey)
{
  int i;
  MapElement e;

  assert(map!=NULL);

  for (i=0; i<map->size; i++) {
    e = map->elements[i];
    while (e!=NULL) {
      MapElement next = e->next;
      if (freeKey) {
        free((void*)e->key);
      }
      free(e);
      e = next;
    }
  }

  free(map->elements);
  free(map);
}


void* insertIntoMap(const char* key, void* value, Map map, int *found)
{
  assert(key!=NULL);
  assert(map!=NULL);

  int hashvalue = hash(key,map->size);
  MapElement element = findInList(key,map->elements[hashvalue]);

  if (element==NULL) {
    element = newElement(key,value);
    element->next = map->elements[hashvalue];
    map->elements[hashvalue] = element;
    map->count++;
    *found = 0;
  }
  else {
    *found = 1;
  }

  return element->value;
}


void* findInMap(const char* key, Map map)
{
  assert(key!=NULL);
  assert(map!=NULL);

  int hashvalue = hash(key,map->size);
  MapElement element = findInList(key,map->elements[hashvalue]);

  return element!=NULL? element->value : NULL;
}


int numberOfElementsInMap(Map map)
{
  assert(map!=NULL);
  return map->count;
}


const char* insertSymbol(const char *sym, Map map)
{
  assert(sym!=NULL);
  assert(map!=NULL);

  int hashvalue = hash(sym,map->size);
  MapElement element = findInList(sym,map->elements[hashvalue]);

  if (element==NULL) {
    char* s = strdup(sym);
    element = newElement(s,s);
    element->next = map->elements[hashvalue];
    map->elements[hashvalue] = element;
    map->count++;
  }

  return element->value;
}


Cursor firstInMap(Map map)
{
  Cursor c;
  int i;

  assert(map!=NULL);

  c.map = map;
  c.i = -1;
  c.p = NULL;

  for (i=0; i<map->size; i++) {
    if (map->elements[i]!=NULL) {
      c.i = i;
      c.p = map->elements[i];
      break;
    }
  }

  return c;
}


int advanceCursor(Cursor* c)
{
  int i;
  assert(c!=NULL);
  MapElement e = (MapElement) c->p;

  e = e->next;
  if (e==NULL) {
    for (i=c->i+1; i<c->map->size; i++) {
      if (c->map->elements[i]!=NULL) {
        c->i = i;
        c->p = c->map->elements[i];
        return 1; // valid
      }
    }
    c->p = NULL;
    return 0; // invalid
  }
  else {
    c->p = e;
    return 1; // valid pointer
  }
}


int isEndOfMap(Cursor* c)
{
  assert(c!=NULL);
  return c->p==NULL;
}


const char* keyOfCursor(Cursor* c)
{
  assert(c!=NULL);
  MapElement e = c->p;
  return e!=NULL? e->key : NULL;
}


void *dataOfCursor(Cursor* c)
{
  assert(c!=NULL);
  MapElement e = c->p;
  return e!=NULL? e->value : NULL;
}

