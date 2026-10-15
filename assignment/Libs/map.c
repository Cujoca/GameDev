#include "map.h"
#include "raylib.h"
#include "tiles.h"
#include <stdlib.h>

GameMap* initMap () {

  int width = 1000;
  int height = 625;

  GameMap* out = (GameMap*)malloc(sizeof(GameMap));
  out->height = height;
  out->width = width;

  // load map tile array with initial dirt tiles with an invalid pos.
  for (int i = 0; i < 200; i++) {
    out->tiles[i] = (Tile){(Vector2){-1,-1},TILE_FLOOR_DIRT};
  }
  return out;
}

int drawMap(GameMap* map, const TileSet* set) {
  if (map == NULL || set == NULL || !set->loaded) return -1;
  for (int i = 0; i < 200; i++) {

    Tile temp = map->tiles[i];
    if (temp.tex == -1) continue;

    DrawTile(set, 
             temp.tex, 
             temp.pos, 
             1.0f, 
             WHITE);
  }
  return 0;
}


