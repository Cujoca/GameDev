#include "./tiles.h"

#include <stdio.h>
#include <string.h>

static bool TileIdValid (int id) {
  return id >= 0 && id < TILE_COUNT;
}

bool LoadTileSet (TileSet* set) {
  if (set == NULL) { return false; }

  memset(set, 0, sizeof(*set));

  set->sheet = LoadTexture(TILE_SHEET_PATH);
  if (set->sheet.id == 0) {
    TraceLog(LOG_WARNING, "TILES: could not load sheet %s", TILE_SHEET_PATH);
    return false;
  }

  /* Walk the grid row by row so frames[id] matches Tiled's numbering. */
  for (int row = 0; row < TILE_ROWS; row++) {
    for (int col = 0; col < TILE_COLS; col++) {
      int id = row * TILE_COLS + col;
      set->frames[id] = (Rectangle){
        (float)(col * (TILE_SIZE + TILE_SPACING)),
        (float)(row * (TILE_SIZE + TILE_SPACING)),
        (float)TILE_SIZE,
        (float)TILE_SIZE
      };
    }
  }

  set->count  = TILE_COUNT;
  set->loaded = true;
  return true;
}

void UnloadTileSet (TileSet* set) {
  if (set == NULL || !set->loaded) { return; }

  UnloadTexture(set->sheet);
  memset(set, 0, sizeof(*set));
}

Rectangle TileFrame (const TileSet* set, int id) {
  if (set == NULL || !set->loaded || !TileIdValid(id)) {
    return (Rectangle){0, 0, 0, 0};
  }
  return set->frames[id];
}

void DrawTile (const TileSet* set, int id, Vector2 pos, float scale, Color tint) {
  if (set == NULL || !set->loaded || !TileIdValid(id)) { return; }

  Rectangle src = set->frames[id];
  Rectangle dst = { pos.x, pos.y, src.width * scale, src.height * scale };

  DrawTexturePro(set->sheet, src, dst, (Vector2){0, 0}, 0.0f, tint);
}

const char* TilePath (int id) {
  static char path[256];

  if (!TileIdValid(id)) { return NULL; }

  snprintf(path, sizeof(path), "%s/tile_%04d.png", TILE_DIR, id);
  return path;
}

int LoadTileRange (Texture2D* out, int first, int last) {
  if (out == NULL || !TileIdValid(first) || !TileIdValid(last) || last < first) {
    TraceLog(LOG_WARNING, "TILES: bad tile range [%d, %d]", first, last);
    return 0;
  }

  int loaded = 0;
  for (int id = first; id <= last; id++) {
    out[loaded] = LoadTexture(TilePath(id));

    if (out[loaded].id == 0) {
      TraceLog(LOG_WARNING, "TILES: could not load %s", TilePath(id));
      UnloadTileTextures(out, loaded);   /* roll back the partial load */
      return 0;
    }
    loaded++;
  }

  return loaded;
}

int LoadAllTiles (Texture2D out[TILE_COUNT]) {
  return LoadTileRange(out, 0, TILE_COUNT - 1);
}

void UnloadTileTextures (Texture2D* tex, int count) {
  if (tex == NULL) { return; }

  for (int i = 0; i < count; i++) {
    if (tex[i].id != 0) {
      UnloadTexture(tex[i]);
      tex[i] = (Texture2D){0};
    }
  }
}
