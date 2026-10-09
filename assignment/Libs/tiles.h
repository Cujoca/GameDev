#ifndef TILES_H
#define TILES_H

#include <raylib.h>
#include <stdbool.h>

/* Kenney "Tiny Dungeon" sheet metadata (see assets/tinydungeon/Tilesheet.txt).
 * The original tilemap.png is a 12 x 11 grid of 16x16 tiles, 1px spacing
 * between tiles, no outer margin, so 12*16 + 11*1 = 203 by 11*16 + 10*1 = 186.
 * We draw from tilemapBig.png instead -- a 3x nearest-neighbour upscale of it,
 * so every measurement triples: 48x48 tiles, 3px spacing, 609 x 558 overall.
 * Tile ids are unchanged; the per-tile PNGs under TILE_DIR are still 16x16. */
#define TILE_SHEET_PATH "assets/tinydungeon/Tilemap/tilemapBig.png"
#define TILE_DIR        "assets/tinydungeon/Tiles"

#define TILE_SCALE   3                    /* tilemapBig.png is 3x tilemap.png */
#define TILE_SIZE    (16 * TILE_SCALE)    /* 48 */
#define TILE_SPACING (1  * TILE_SCALE)    /* 3  */
#define TILE_COLS    12
#define TILE_ROWS    11
#define TILE_COUNT   (TILE_COLS * TILE_ROWS)   /* 132 */

/* Sentinel for "no tile here" when indexing a level grid. */
#define TILE_NONE (-1)

/* Tile id ranges, indexed as id = row * TILE_COLS + col.
 * Keep every range the game cares about in one place so the rest of the
 * code never hardcodes a raw number. */
#define TILE_PLAYER_FIRST  96
#define TILE_PLAYER_LAST  100

/* One GPU texture plus a source rectangle per tile id. This is what the
 * game should draw from: 132 tiles cost one texture and one bind, so
 * raylib can batch the whole map into a single draw call. */
typedef struct TileSet {
  Texture2D sheet;
  Rectangle frames[TILE_COUNT];
  int       count;
  bool      loaded;
} TileSet;

/* Ids are id = row * TILE_COLS + col; the (r,c) in each comment is the tile's
 * cell on the sheet. Anything INVALID has no matching art in tinydungeon --
 * TileFrame and DrawTile already no-op on out-of-range ids, so those names
 * stay usable without drawing the wrong thing. */
enum Tiles {
  INVALID = -1,

  /* Dirt floor. 0 is flat, the other two are speckled. (r0c0, r1c0, r2c0) */
  TILE_FLOOR_DIRT   =  0,
  TILE_FLOOR_DIRT_1 = 12,   /* brown pebbles */
  TILE_FLOOR_DIRT_2 = 24,   /* grey pebbles  */

  /* Sand floor, r4c0..r4c5. 48-49 are flat fill, 50-53 carry a darker edge
   * for transitions. The sheet only has six, hence no seventh. */
  TILE_FLOOR_SAND   = 48,
  TILE_FLOOR_SAND_2 = 49,
  TILE_FLOOR_SAND_3 = 50,
  TILE_FLOOR_SAND_4 = 51,
  TILE_FLOOR_SAND_5 = 52,
  TILE_FLOOR_SAND_6 = 53,
  TILE_FLOOR_SAND_7 = 42,   /* only six sand variants exist */

  /* Closed 3x3 wall ring on dirt, r0c1..r2c3. The centre (14) is brick
   * floor -- see TILE_FLOOR_BRICK_1. 1/3, 13/15 and 25/27 are mirror pairs. */
  TILE_WALL_1_TOP_LEFT  =  1,
  TILE_WALL_1_TOP       =  2,
  TILE_WALL_1_TOP_RIGHT =  3,
  TILE_WALL_1_RIGHT     = 15,
  TILE_WALL_1_BOT_RIGHT = 27,
  TILE_WALL_1_BOT       = 26,
  TILE_WALL_1_BOT_LEFT  = 25,
  TILE_WALL_1_LEFT      = 13,

  /* 2x2 stone arch on dirt, r0c4..r1c5. Corners only -- this one has no
   * straight edge pieces, so it cannot be stretched past 2x2. */
  TILE_WALL_2_TOP_LEFT  =  4,
  TILE_WALL_2_TOP_RIGHT =  5,
  TILE_WALL_2_BOT_RIGHT = 17,
  TILE_WALL_2_BOT_LEFT  = 16,

  /* 1x3 pillar, r0c6..r2c6. MID repeats to make it taller. */
  TILE_WALL_PILLAR_TOP =  6,
  TILE_WALL_PILLAR_MID = 18,
  TILE_WALL_PILLAR_BOT = 30,

  /* Wall fountain, r0c7..r1c8: _1 is the spout, _2 the gargoyle below it.
   * Col 7 is dry, col 8 has the green flow. */
  TILE_WALL_DECO_1_DRY =  7,
  TILE_WALL_DECO_1_WET =  8,
  TILE_WALL_DECO_2_DRY = 19,
  TILE_WALL_DECO_2_WET = 20,

  /* Cauldron, r2c7..r3c8, same dry/wet split and same top/bottom split. */
  TILE_WALL_DECO_CAULDRON_1_DRY = 31,
  TILE_WALL_DECO_CAULDRON_1_WET = 32,
  TILE_WALL_DECO_CAULDRON_2_DRY = 43,
  TILE_WALL_DECO_CAULDRON_2_WET = 44,

  /* Door animation, r0c9..r3c11. Frame 0 lies flat open, 3 is shut, so
   * counting up closes the door and counting down opens it. */
  TILE_DOOR_0_SINGLE       =  9,
  TILE_DOOR_0_DOUBLE_LEFT  = 10,
  TILE_DOOR_0_DOUBLE_RIGHT = 11,
  TILE_DOOR_1_SINGLE       = 21,
  TILE_DOOR_1_DOUBLE_LEFT  = 22,
  TILE_DOOR_1_DOUBLE_RIGHT = 23,
  TILE_DOOR_2_SINGLE       = 33,
  TILE_DOOR_2_DOUBLE_LEFT  = 34,
  TILE_DOOR_2_DOUBLE_RIGHT = 35,
  TILE_DOOR_3_SINGLE       = 45,
  TILE_DOOR_3_DOUBLE_LEFT  = 46,
  TILE_DOOR_3_DOUBLE_RIGHT = 47,

  /* Brick floor. 40 and 14 are both seamless fills with different patterns;
   * 57 | 40 | 59 is the capped 3-wide strip, so its middle is the plain fill.
   * 58 is a one-tile-wide strip, capped on both sides. */
  TILE_FLOOR_BRICK          = 40,
  TILE_FLOOR_BRICK_1        = 14,   /* the WALL_1 ring interior */
  TILE_FLOOR_BRICK_2_LEFT   = 57,
  TILE_FLOOR_BRICK_2_MID    = 40,
  TILE_FLOOR_BRICK_2_RIGHT  = 59,
  TILE_FLOOR_BRICK_NARROW   = 58,

  /* Stairs, r3c0..r3c3. MID repeats; SINGLE is the one-tile-wide flight. */
  TILE_STAIRS_LEFT   = 36,
  TILE_STAIRS_MID    = 37,
  TILE_STAIRS_RIGHT  = 38,
  TILE_STAIRS_SINGLE = 39,

  /* Minecart, r4c6..r4c8. 54 and 55 are the same cart turned 90 degrees --
   * identical pixel counts and transposed bounds (16x14 vs 14x16) -- so the
   * long axis names the direction of travel. 56 is the 3/4 view. */
  TILE_MINECART_VERT = 55,
  TILE_MINECART_HORI = 54,
  TILE_MINECART_DIAG = 56,

  /* Rail end caps: each one is open on a single side, so the name says which
   * side the track stops on. */
  TILE_RAIL_END_UP    = 67,
  TILE_RAIL_END_BOT   = 68,
  TILE_RAIL_END_LEFT  = 79,
  TILE_RAIL_END_RIGHT = 80,

  /* Rail ring, r5c9..r7c11, named by position in the ring. TOP and BOT are
   * the same horizontal tile, LEFT and RIGHT the same vertical one -- the
   * sheet stores them twice and they are pixel-identical. */
  TILE_RAIL_TOP       = 70,
  TILE_RAIL_TOP_RIGHT = 71,
  TILE_RAIL_TOP_LEFT  = 69,
  TILE_RAIL_RIGHT     = 83,
  TILE_RAIL_BOT_RIGHT = 95,
  TILE_RAIL_BOT       = 94,
  TILE_RAIL_BOT_LEFT  = 93,
  TILE_RAIL_LEFT      = 81,

  /* Wooden crates. 66 stands alone; 63 caps a vertical stack whose body (75)
   * repeats downwards. */
  TILE_CRATE_SINGLE = 66,
  TILE_CRATE_TOP    = 63,
  TILE_CRATE_MID    = 75,
};

/* Loads TILE_SHEET_PATH and fills in every source rectangle.
 * Returns false (and leaves *set zeroed) if the image could not be loaded.
 * Call after InitWindow -- raylib needs a GL context to upload a texture. */
bool LoadTileSet   (TileSet* set);
void UnloadTileSet (TileSet* set);

/* Source rectangle for a tile id; a zero-size rectangle if id is out of
 * range, which draws nothing rather than crashing. */
Rectangle TileFrame (const TileSet* set, int id);

/* Draws tile `id` with its top-left corner at `pos`, scaled by `scale`
 * (1.0f = 48x48 on screen, since the sheet is already the 3x one).
 * Out-of-range ids and unloaded sets are no-ops. */
void DrawTile (const TileSet* set, int id, Vector2 pos, float scale, Color tint);

/* ---- Individual-file loading -------------------------------------------
 * Only needed when a tile has to be its own Texture2D (e.g. passing one
 * into an Entity). Prefer the TileSet above for anything map-sized.
 * Note these are the original 16x16 PNGs, not the 3x sheet. */

/* Path to a single tile PNG, e.g. "assets/tinydungeon/Tiles/tile_0096.png".
 * Returns a pointer to a static buffer -- not reentrant, and the result is
 * only valid until the next call. NULL if id is out of range. */
const char* TilePath (int id);

/* Loads tile ids [first, last] inclusive into `out`, which the caller must
 * size to at least (last - first + 1). Returns how many loaded; on failure
 * the already-loaded textures are unloaded and 0 is returned, so the caller
 * never has to clean up a partial load. */
int LoadTileRange (Texture2D* out, int first, int last);

/* Loads every tile as its own texture. `out` must hold TILE_COUNT entries. */
int LoadAllTiles (Texture2D out[TILE_COUNT]);

void UnloadTileTextures (Texture2D* tex, int count);

#endif /* TILES_H */
