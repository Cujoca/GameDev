#include <raylib.h>
#include "Core/menu.h"

void createMenu (int temp) {
  GuiButton((Rectangle){200, 50}, "Play");
  GuiButton((Rectangle){200, 50}, "Settings");
  GuiButton((Rectangle){200, 50}, "Exit");
}
