#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/**
@file map.h

@brief A map is used to store name-value pairs. The name is a null terminated string, the value
is an arbitrary pointer.

Note that only pointers are stored in the map. Neither key nor value are copied.
*/


typedef struct Map_R* Map;

typedef struct Cursor_R {
  Map map;
  int i;
  void* p;
} Cursor;

/** Creates a new map. The map is implemented as a hash table.
@param size is the size of the hash table. If size==0, a default size is used.
@return a new map
*/
Map newMap( int size );

/** Releases the memory occupied by the map.
@param map is the map to delete
@param freeKey controls whether memory allocated by keys are released
*/
void deleteMap(Map map, int freeKey);

/** Inserts a name-value pair into the map. If the key already exists, the value is
not inserted.
@param key is the null-terminated string used as key for later look up
@param value is a pointer to an arbitrary data structure which can later be looked up by key
@param map is the map into which the name value pair is inserted
@param found is used to return whether there is already an entry with key
@return the value associated with key, i.e. either the newly inserted value or the
already existing value
*/
void* insertIntoMap(const char* key, void* value, Map map, int *found);

int numberOfElementsInMap(Map);

/** Looks up a value by name in the map.
@param key is the name of the value
@param map is where to look up the value
@return the value associated with key or NULL if there's no value associated with this key in the map
*/
void* findInMap(const char* key, Map map);

/** This is a shortcut funtion to use a map as symbol table. A symbol is a unique representation
for a name, which allows for fast comparision (comparision of pointers versus character by character
string comparision)

Example:
symbol1 = insertSymbol("ABC",map);
symbol2 = insertSymbol("ABC",map);
assert(symbol1==symbol2);

The symbol is implemented as a char pointer to the stored symbol name. Technically,
the pointer to the symbol name is used as key and value in the map.

@param sym is the null-terminated string to be turned into a symbol
@param map is the symbol table
@return the symbol representing the name
*/
const char* insertSymbol(const char *sym, Map map);


/** @brief Retrieves a pointer (cursor) to the first element in the map.

Example:
Cursor c;
for (c=firstInMap(map); !isEndOfMap(&c); advanceCursor(&c)) {
  const char *key = keyOfCursor(&c);
  void *data = dataOfCursor(&c);
  ...
}
*/
Cursor firstInMap(Map);

/** @brief Advances the cursor.

@param[in,out] c is the cursor aquired by a call to firstInMap, which is advanced
@return true if the cursor could be advanced and on return points to a valid element in the map
*/
int advanceCursor(Cursor* c);

/** @brief Checks if the cursor points to an element in the map.

@param[in] c is the cursor aquired by firstInMap
@return true if the cursor points to an element in the map or false if it has been advanced to the end
*/
int isEndOfMap(Cursor* c);

/** @brief Retrieves the key of the element to which a cursror points
*/
const char* keyOfCursor(Cursor* c);

/** @brief Retrieves the value of the element to which a cursror points
*/
void *dataOfCursor(Cursor* c);

#endif

