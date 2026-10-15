#include "raylib.h"
#include "tiles.h"

typedef struct Tile {
  Vector2 pos;
  enum Tiles tex;
} Tile;

typedef struct GameMap {
  int width;
  int height;
  Tile tiles[200];
} GameMap;


GameMap* initMap();
int drawMap (GameMap* map, const TileSet* set);
