#include <raylib.h>

typedef struct ColorRect {
  int width;
  int height;
  Color color;
} ColorRect;

int main (void) {

  const int height = 600;
  const int width = 800;

  InitWindow(width, height, "Lab 2: cojo0003");

  SetTargetFPS(60);

  const ColorRect rect = (ColorRect){200, 50, RED};
  const int rectPos_Y = 550;
  int rectPos_X = 0;

  while (!WindowShouldClose()) {

    // take away 100 so that mouse is in center of rectangle, not corner
    rectPos_X = GetMouseX() - 100;
    if (rectPos_X > width - rect.width) { rectPos_X = width - rect.width; }
    else if (rectPos_X < 0) { rectPos_X = 0; }

    if (IsKeyPressed(KEY_H)) {
      if (IsCursorHidden()) { ShowCursor(); }
      else { HideCursor(); }
    }

    BeginDrawing(); {
        ClearBackground(RAYWHITE);
        DrawRectangle(rectPos_X, rectPos_Y, rect.width, rect.height, rect.color);
      } EndDrawing();
  }

  CloseWindow();

  return 0;
}
