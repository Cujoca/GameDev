#include "Core/menu.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "Vendor/raygui.h"

enum GAME_STATE {
  MENU = 1,
  EXIT = 0,
  PLAY = 2,
  SETTINGS = 3
};

int main () {

  int GAME_STATE = MENU;

  InitWindow(800, 600, "Menu test");

  switch (GAME_STATE) {

    case MENU: {
      while (GAME_STATE != EXIT) {
        BeginDrawing();
        if (GuiButton((Rectangle){300, 50, 200, 100}, "Play"))      { GAME_STATE = PLAY; }
        if (GuiButton((Rectangle){300, 200, 200, 100}, "Settings")) { GAME_STATE = SETTINGS;}
        if (GuiButton((Rectangle){300, 350, 200, 100}, "Exit"))     { GAME_STATE = EXIT; }
        EndDrawing();
      }
      break;
    }

    case SETTINGS: {
      // do settings menu
      break;
    }

    case PLAY: {
      // do play loop
      break;
    }
  }

  return 1;
}
