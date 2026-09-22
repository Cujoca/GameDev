#include <raylib.h>

typedef struct ColorRectangle {
  int posX;
  int posY;
  int height;
  int width;
  Color color;
} ColorRectangle;

typedef struct ColorTriangle {
  Vector2 side1;
  Vector2 side2;
  Vector2 side3;
  Color color;
} ColorTriangle;

typedef struct ColorCircle {
  int posX;
  int posY;
  int radius;
  Color color;
} ColorCircle;


static Vector2 drawWithinLimits (int item_X, int item_Y,
                             int item_width, int item_height,
                             int screen_X, int screen_Y);
static void drawColorRect (ColorRectangle shape);
static void drawColorCirc (ColorCircle shape);
static void drawColorTri  (ColorTriangle shape);
