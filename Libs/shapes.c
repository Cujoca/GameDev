#include "shapes.h"
#include "raylib.h"

static Vector2 drawWithinLimits (int item_X, int item_Y, 
                             int item_width, int item_height,
                             int screen_X, int screen_Y) {
  int out_X = item_X;
  int out_Y = item_Y;

  if (item_X + item_width > screen_X) out_X = screen_X - item_width;
  if (item_X < 0) out_X = 0;
  if (item_Y + item_height > screen_Y) out_Y = screen_Y - item_height;
  if (item_Y < 0) out_Y = 0;

  return (Vector2){out_X, out_Y};
}

static void drawColorRect (ColorRectangle shape) {
  DrawRectangle(shape.posX, shape.posY, shape.width, shape.height, shape.color);
}

static void drawColorCirc (ColorCircle shape) {
  DrawCircle(shape.posX, shape.posY, shape.radius, shape.color);
}

static void drawColorTri (ColorTriangle shape) {
  DrawTriangle(shape.side1, shape.side2, shape.side3, shape.color);
}
