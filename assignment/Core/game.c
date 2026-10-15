#include "Core/menu.h"
#include "Core/play.h"
#include "Libs/tiles.h"
#include "Libs/map.h"
#include <raylib.h>
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include "Vendor/raygui.h"

enum GAME_STATE {
MENU = 1,
  EXIT = 0,
  PLAY = 2,
  SETTINGS = 3
};

int main (void) {

  int GAME_STATE = MENU;

  InitWindow(800, 600, "Menu test");
  SetTargetFPS(60);

  // Loaded once, before the loop: LoadTileSet reads the sheet off disk and
  // uploads a texture, so it must not happen per frame. Has to come after
  // InitWindow.
  TileSet tileSet;
  if (!LoadTileSet(&tileSet)) {
    CloseWindow();
    return 1;
  }

  GameMap* map = initMap();
  if (map == NULL) {
    UnloadTileSet(&tileSet);
    CloseWindow();
    return 1;
  }

  // One loop for the whole game, with the state switch inside it. Every
  // frame has to reach EndDrawing -- that is what swaps buffers and polls
  // input, so a state that loops on its own without calling it hangs the
  // window.
  while (!WindowShouldClose() && GAME_STATE != EXIT) {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (GAME_STATE) {

      case MENU: {
        if (GuiButton((Rectangle){300, 50, 200, 100}, "Play"))      { GAME_STATE = PLAY; }
        if (GuiButton((Rectangle){300, 200, 200, 100}, "Settings")) { GAME_STATE = SETTINGS;}
        if (GuiButton((Rectangle){300, 350, 200, 100}, "Exit"))     { GAME_STATE = EXIT; }
        break;
      }

      case SETTINGS: {
        // do settings menu
        break;
      }

      case PLAY: {


        Entity *player = (Entity*)malloc(sizeof(Entity));

        player->entityType = PLAYER;



        drawMap(map, &tileSet);
        // do play loop
        break;
      }
    }

    EndDrawing();
  }

  free(map);
  UnloadTileSet(&tileSet);
  CloseWindow();

  return 0;
}
