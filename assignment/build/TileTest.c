/* TileTest -- draws every tile in the TileSet on a 12 x 11 grid matching the
 * sheet layout, with each tile's array index above it. Useful for checking
 * the names in Libs/tiles.h against the actual art.
 *
 * Build and run from the assignment root so TILE_SHEET_PATH resolves:
 *   cc -std=c99 -Wall -Wextra -I. build/TileTest.c Libs/tiles.c \
 *      -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib \
 *      -framework IOKit -framework Cocoa -framework OpenGL -o build/TileTest
 *   ./build/TileTest
 *
 * Press ESC or close the window to exit. */

#include <raylib.h>

#include "Libs/tiles.h"

#define LABEL_SIZE 10
#define LABEL_H    12
#define GAP         6
#define MARGIN     10

#define CELL_W (TILE_SIZE + GAP)             /* 54 */
#define CELL_H (LABEL_H + TILE_SIZE + GAP)   /* 66 */

#define SCREEN_W (MARGIN * 2 + TILE_COLS * CELL_W)
#define SCREEN_H (MARGIN * 2 + TILE_ROWS * CELL_H)

int main (void) {
  InitWindow(SCREEN_W, SCREEN_H, "TileTest -- tinydungeon tilemapBig.png");
  SetTargetFPS(60);

  TileSet set;
  bool ok = LoadTileSet(&set);

  if (ok) {
    /* The sheet is already pre-scaled 3x, so keep sampling exact. */
    SetTextureFilter(set.sheet, TEXTURE_FILTER_POINT);
  }

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground((Color){24, 24, 32, 255});

    if (!ok) {
      DrawText("Could not load " TILE_SHEET_PATH, MARGIN, MARGIN, 20, RED);
      DrawText("Run this from the assignment root directory.",
               MARGIN, MARGIN + 28, 20, GRAY);
      EndDrawing();
      continue;
    }

    for (int id = 0; id < set.count; id++) {
      int col = id % TILE_COLS;
      int row = id / TILE_COLS;

      int x = MARGIN + col * CELL_W;
      int y = MARGIN + row * CELL_H;

      /* A lighter panel behind each tile so transparent pixels are visible. */
      DrawRectangle(x, y + LABEL_H, TILE_SIZE, TILE_SIZE,
                    (Color){56, 56, 68, 255});

      DrawText(TextFormat("%d", id), x, y, LABEL_SIZE, (Color){230, 200, 90, 255});
      DrawTile(&set, id, (Vector2){(float)x, (float)(y + LABEL_H)}, 1.0f, WHITE);
    }

    EndDrawing();
  }

  UnloadTileSet(&set);
  CloseWindow();
  return 0;
}
