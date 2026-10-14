#ifndef TILES_H
#define TILES_H

#include <raylib.h>
#include <stdbool.h>

// Kenney Tiny Dungeon tilesheet (info in assets/tinydungeon/Tilesheet.txt)
// Original is 12x11 tiles, 16x16 each with 1px spacing.
// Using tilemapBig.png which is just the same thing scaled up 3x, so 48x48 tiles w/ 3px spacing.
// The single tile PNGs in TILE_DIR are still 16x16 though
#define TILE_SHEET_PATH "assets/tinydungeon/Tilemap/tilemapBig.png"
#define TILE_DIR        "assets/tinydungeon/Tiles"

#define TILE_SCALE   3                    // tilemapBig is 3x
#define TILE_SIZE    (16 * TILE_SCALE)    // 48
#define TILE_SPACING (1  * TILE_SCALE)    // 3
#define TILE_COLS    12
#define TILE_ROWS    11
#define TILE_COUNT   (TILE_COLS * TILE_ROWS)   // 132

// Empty spot in a level grid
#define TILE_NONE (-1)

// id = row * TILE_COLS + col
#define TILE_PLAYER_FIRST  96
#define TILE_PLAYER_LAST  100

// Whole sheet as one texture + a rect for each tile.
// Use this for drawing, way cheaper than 132 separate textures
typedef struct TileSet {
  Texture2D sheet;
  Rectangle frames[TILE_COUNT];
  int       count;
  bool      loaded;
} TileSet;

// All the tile ids on the sheet. rXcY = row X, col Y
enum Tiles {
  INVALID = -1,

  // Dirt floors
  TILE_FLOOR_DIRT   =  0,
  TILE_FLOOR_DIRT_1 = 12,   // brown pebbles
  TILE_FLOOR_DIRT_2 = 24,   // grey pebbles

  // Sand floors, 48-49 are plain, 50-53 have darker edges for transitions
  TILE_FLOOR_SAND   = 48,
  TILE_FLOOR_SAND_2 = 49,
  TILE_FLOOR_SAND_3 = 50,
  TILE_FLOOR_SAND_4 = 51,
  TILE_FLOOR_SAND_5 = 52,
  TILE_FLOOR_SAND_6 = 53,
  TILE_FLOOR_SAND_7 = 42,
  
  TILE_FLOOR_SPIKES = 41,

  // 3x3 wall ring, middle is TILE_FLOOR_BRICK_1
  TILE_WALL_1_TOP_LEFT  =  1,
  TILE_WALL_1_TOP       =  2,
  TILE_WALL_1_TOP_RIGHT =  3,
  TILE_WALL_1_RIGHT     = 15,
  TILE_WALL_1_BOT_RIGHT = 27,
  TILE_WALL_1_BOT       = 26,
  TILE_WALL_1_BOT_LEFT  = 25,
  TILE_WALL_1_LEFT      = 13,

  // 2x2 stone arch, only corners so can't make it bigger
  TILE_WALL_2_TOP_LEFT  =  4,
  TILE_WALL_2_TOP_RIGHT =  5,
  TILE_WALL_2_BOT_RIGHT = 17,
  TILE_WALL_2_BOT_LEFT  = 16,

  TILE_WALL_STONE_BANNER = 29,
  TILE_WALL_STONE_BARS   = 28,

  // Pillar, repeat MID to make it taller
  TILE_WALL_PILLAR_TOP =  6,
  TILE_WALL_PILLAR_MID = 18,
  TILE_WALL_PILLAR_BOT = 30,

  // Wall fountain, 1 is simple, 2 is gargoyle
  TILE_WALL_DECO_1_DRY =  7,
  TILE_WALL_DECO_1_WET =  8,
  TILE_WALL_DECO_2_DRY = 19,
  TILE_WALL_DECO_2_WET = 20,

  // Cauldron, same deal as the fountain
  TILE_WALL_DECO_CAULDRON_1_DRY = 31,
  TILE_WALL_DECO_CAULDRON_1_WET = 32,
  TILE_WALL_DECO_CAULDRON_2_DRY = 43,
  TILE_WALL_DECO_CAULDRON_2_WET = 44,

  // Door anim frames, 0 = corridor, 3 = closed
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

  // Brick floors, 40 and 14 tile seamlessly
  // LEFT/MID/RIGHT make a strip, NARROW is a 1 wide strip
  TILE_FLOOR_BRICK          = 40,
  TILE_FLOOR_BRICK_1        = 14,   // middle of the WALL_1 ring
  TILE_FLOOR_BRICK_2_LEFT   = 57,
  TILE_FLOOR_BRICK_2_MID    = 40,
  TILE_FLOOR_BRICK_2_RIGHT  = 59,
  TILE_FLOOR_BRICK_NARROW   = 58,

  // Stairs, MID repeats, SINGLE is just 1 wide
  TILE_STAIRS_LEFT   = 36,
  TILE_STAIRS_MID    = 37,
  TILE_STAIRS_RIGHT  = 38,
  TILE_STAIRS_SINGLE = 39,

  // Minecart, named by direction it's going
  TILE_MINECART_VERT = 55,
  TILE_MINECART_HORI = 54,
  TILE_MINECART_DIAG = 56,

  // Rail ends, named by which side the track stops
  TILE_RAIL_END_UP    = 67,
  TILE_RAIL_END_BOT   = 68,
  TILE_RAIL_END_LEFT  = 79,
  TILE_RAIL_END_RIGHT = 80,

  // Rail loop pieces (TOP/BOT and LEFT/RIGHT look the same)
  TILE_RAIL_TOP       = 70,
  TILE_RAIL_TOP_RIGHT = 71,
  TILE_RAIL_TOP_LEFT  = 69,
  TILE_RAIL_RIGHT     = 83,
  TILE_RAIL_BOT_RIGHT = 95,
  TILE_RAIL_BOT       = 94,
  TILE_RAIL_BOT_LEFT  = 93,
  TILE_RAIL_LEFT      = 81,

  // Crate stack, repeat MID downwards
  TILE_CRATE_TOP    = 63,
  TILE_CRATE_MID    = 75,

  // Headstones for decorative effect
  TILE_GRAVE_CROSS = 64,
  TILE_GRAVE_HEADSTONE = 65,
  TILE_GRAVE_DIRT = 66,

  // Could perhaps be destroyable for small loot?
  TILE_PEWDS_HATES_BARRELS = 82,

  // Make interactable?
  TILE_OBJ_TABLE = 72,
  TILE_OBJ_STOOL = 73,
  TILE_OBJ_ANVIL = 74,

  TILE_OBJ_CHEST_CLO = 89,
  TILE_OBJ_CHEST_MID = 90,
  TILE_OBJ_CHEST_OPN = 91,

  TILE_WALL_FENCE_LEFT  = 76,
  TILE_WALL_FENCE_MID   = 77,
  TILE_WALL_FENCE_RIGHT = 78,

  // Item sprites
 
  // Shields
  ITEM_SHIELD_WOOD = 101,
  ITEM_SHIELD_METAL = 102,

  // Weapons
 
  // Swords
  ITEM_WEAPON_SWORD_1 = 103,
  ITEM_WEAPON_SWORD_2 = 104,
  ITEM_WEAPON_SWORD_3 = 105,
  ITEM_WEAPON_SWORD_4 = 106,
  ITEM_WEAPON_SWORD_5 = 107,

  // Hammer
  ITEM_WEAPON_HAMMER = 117,

  // Axes
  ITEM_WEAPON_AXE_DOUBLE = 118,
  ITEM_WEAPON_AXE_SINGLE = 119,

  // Staves
  ITEM_WEAPON_STAFF_ELECTRIC = 129,
  ITEM_WEAPON_STAFF_ICE      = 130,
  ITEM_WEAPON_STAFF_PHYSICAL = 131,

  // Potions
  ITEM_POTION_HP_BIG    = 115,
  ITEM_POTION_HP_SMALL  = 127,
  ITEM_POTION_SPD_BIG   = 114,
  ITEM_POTION_SPD_SMALL = 126,
  ITEM_POTION_DEF_BIG   = 113,
  ITEM_POTION_DEF_SMALL = 125,
  ITEM_POTION_MP_BIG    = 116,
  ITEM_POTION_MP_SMALL  = 128,

  // Enemy sprites
  CHAR_ENEMY_GHOST     = 108,
  CHAR_ENEMY_CYCLOPS   = 109,
  CHAR_ENEMY_CRAB      = 110,
  CHAR_ENEMY_LICH      = 111,
  CHAR_ENEMY_BAT       = 120,
  CHAR_ENEMY_WRAITH    = 121,
  CHAR_ENEMY_SPUNGUS   = 122,
  CHAR_ENEMY_RAT_BROWN = 123,
  CHAR_ENEMY_RAT_GRAY  = 124,
  CHAR_ENEMY_MIMIC     = 92,

  // Player sprites
  CHAR_PLAYER_WIZARD_M = 84,
  CHAR_PLAYER_WIZARD_F = 99,
  CHAR_PLAYER_KNIGHT   = 96,
  CHAR_PLAYER_BARB     = 87,
};

// Loads the sheet + all the rects. Returns false if it fails
// Has to be called after InitWindow
bool LoadTileSet   (TileSet* set);
void UnloadTileSet (TileSet* set);

// Rect for a tile id, empty rect if the id is bad
Rectangle TileFrame (const TileSet* set, int id);

// Draws tile at pos (top left), scale 1.0 = 48x48
// Does nothing if id is bad or set isn't loaded
void DrawTile (const TileSet* set, int id, Vector2 pos, float scale, Color tint);

// Loading tiles as separate files
// Only use if a tile needs to be its own texture (like for an Entity), use TileSet otherwise
// These are the 16x16 ones, not the 3x

// Gets path to a tile png, e.g. "assets/tinydungeon/Tiles/tile_0096.png"
// Uses a static buffer so it gets overwritten next call. NULL if id is bad
const char* TilePath (int id);

// Loads tiles first to last (inclusive) into out, needs room for last - first + 1
// Returns how many loaded, or 0 if it fails (cleans up after itself)
int LoadTileRange (Texture2D* out, int first, int last);

// Loads every tile as its own texture, out needs TILE_COUNT spots
int LoadAllTiles (Texture2D out[TILE_COUNT]);

void UnloadTileTextures (Texture2D* tex, int count);

#endif /* TILES_H */
