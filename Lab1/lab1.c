#include <raylib.h>

// Shape structs
typedef struct ColorRect {
    Color color;
    int x;
    int y;
    int height;
    int width;
  } ColorRect;

  typedef struct ColorBall {
    Color color;
    int x;
    int y;
    int rad;
  } ColorBall;

  typedef struct ColorTri {
    Color color;
    Vector2 side1;
    Vector2 side2;
    Vector2 side3;
  } ColorTri;

// Static methods for drawing shapes
static void DrawColorRect (ColorRect shape);
static void DrawColorTri  (ColorTri shape);
static void DrawColorBall (ColorBall shape);

int main (void) {
  
  const int scn_height = 600;
  const int scn_width = 800;

  InitWindow(scn_width, scn_height, "Lab 1 - cojo0003");

  SetTargetFPS(60);

  // Define Shapes
  ColorTri triangle1;
  triangle1.side1 = (Vector2){ 230, 250 };
  triangle1.side2 = (Vector2){ 250, 250 };
  triangle1.side3 = (Vector2){ 240, 230 };
  triangle1.color = YELLOW;

  ColorTri triangle2;
  triangle2.side1 = (Vector2){ 530, 250 };
  triangle2.side2 = (Vector2){ 550, 250 };
  triangle2.side3 = (Vector2){ 540, 230 };
  triangle2.color = WHITE;

  ColorBall ball1;
  ball1.x = 400;
  ball1.y = 0;
  ball1.rad = 20;
  ball1.color = BLUE;

  ColorBall ball2;
  ball2.x = 100;
  ball2.y = 500;
  ball2.rad = 40;
  ball2.color = GREEN;


  ColorRect rect1;
  rect1.x = 100;
  rect1.y = 100;
  rect1.width = 40;
  rect1.height = 30;
  rect1.color = (Color){ 0, 0, 0, 40};

  ColorRect rect2;
  rect2.x = 700;
  rect2.y = 0;
  rect2.width = 100;
  rect2.height = 200;
  rect2.color = RED;

  // bool for determining which way ball moves
  bool dirCircle = true;

  while (!WindowShouldClose()) {

    // Draw the shapes
    BeginDrawing(); {
      ClearBackground(VIOLET);
      DrawColorTri(triangle1);
      DrawColorTri(triangle2);

      DrawColorBall(ball1);
      DrawColorBall(ball2);

      DrawColorRect(rect1);
      DrawColorRect(rect2);
    }
    EndDrawing();

    // Move the ball down, stop at bottom of scene
    if (ball1.y == scn_height-20) { dirCircle = false; }
    else if (ball1.y == 20)       { dirCircle = true; }

    if (dirCircle) { ball1.y += 2; } 
    else { ball1.y-=2; }
  }

}

// Implemented static functions for drawing shapes
static void DrawColorTri (ColorTri shape)  { 
  DrawTriangle(shape.side1, 
               shape.side2, 
               shape.side3, 
               shape.color
               );
}
static void DrawColorBall (ColorBall shape) { 
  DrawCircle(shape.x, 
             shape.y, 
             shape.rad, 
             shape.color
             ); 
}
static void DrawColorRect (ColorRect shape) { 
  DrawRectangle(shape.x, 
                shape.y, 
                shape.width, 
                shape.height, 
                shape.color
                ); 
}
