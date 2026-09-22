#include <raylib.h>
#include <stdlib.h>

// A rectangle and its colour, bundled so the gun is one value.
typedef struct ColorRect {
  int pos_x;
  int pos_y;
  int width;
  int height;
  Color color;
} ColorRect;

/* Doubly linked list of bullets in flight. val is the bullet's x position.
 *
 * You may be wondering why I chose a DLL.
 * I needed two things:
 *  - keep an dynamic number of bullets in memory
 *  - be able to remove any bullet, as the gun position can change,
 *    meaning that bullet 1 might have a smaller x value than bullet 2
 *
 * A DLL keeps the size dynamic. I can't use a queue, since because of point 2
 * the bullet's x positions aren't specifically in order, so I can't assume that
 * the removal of bullets will follow a FIFO order.
 *
 * I could have made a map. Problem is, I don't know how to make a map in C.
 * I've done more than my fair share of prolog-style list exercises in C, so I
 * chose what I know.
 */
typedef struct BulletList {
  int val;
  int hasNext;
  int hasPrev;
  struct BulletList* next;
  struct BulletList* prev;
} BulletList;

// I'm gonna have to start putting these in a library so I don't have to redefine every time.
// maybe a list of 'Entities' where the list records a Vector2 of position, and a link to the sprite?
// or I could actually be a good programmer and figure out a map.

static void fireGun ();
static void drawBullet (int pos_x);
static BulletList* createEmptyBullet ();
static BulletList* remove (BulletList* bullet);
static void append (BulletList* list, BulletList* add);

int main (void) {

  const int height = 600;
  const int width = 800;

  // Dummy head, never drawn, so the list always has something to append to.
  BulletList* bulletsDummy = createEmptyBullet();

  InitWindow(width, height, "Lab 2: cojo0003");

  SetTargetFPS(60);

  const ColorRect rect = (ColorRect){0, 0, 200, 50, RED};
  const int rectPos_Y = 550;
  int rectPos_X = 0;

  while (!WindowShouldClose()) {

    // Take away 100 so that the mouse is in the center of the rectangle, not the corner.
    rectPos_X = GetMouseX() - 100;

    if (rectPos_X > width - rect.width) { rectPos_X = width - rect.width; } 
    else if (rectPos_X < 0) { rectPos_X = 0; }

    // H toggles the cursor.
    if (IsKeyPressed(KEY_H)) {
      if (IsCursorHidden()) { ShowCursor(); }
      else { HideCursor(); }
    }

    // Left click spawns a bullet at the current mouse position.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      BulletList* newBullet = createEmptyBullet();
      newBullet->val = GetMouseX();
      append(bulletsDummy, newBullet);
    }

    BeginDrawing(); {
      ClearBackground(RAYWHITE);
      DrawRectangle(rectPos_X, rectPos_Y, rect.width, rect.height, rect.color);

      // Move every bullet along, dropping the ones that have left the screen.
      // cur trails one behind, so remove() relinks it onto the next bullet for us.
      BulletList* cur = bulletsDummy;
      while (cur->hasNext) {

        BulletList* bullet = cur->next;
        bullet->val += 5;
        if (bullet->val >= width) { free(remove(bullet)); }
        else {
          drawBullet(bullet->val);
          cur = bullet;
        }
      }

    } EndDrawing();
  }

  // Free any bullets still in flight, then the dummy head.
  while (bulletsDummy->hasNext) { free(remove(bulletsDummy->next)); }
  free(bulletsDummy);

  CloseWindow();

  return 0;
}

// Draws a single bullet at the given x position.
static void drawBullet (int pos_x) {
  DrawRectangle (pos_x+100, 575, 100, 50, BLACK);
}

// Allocates a blank unlinked node. Returns NULL if out of memory.
static BulletList* createEmptyBullet () {
  BulletList* out = malloc(sizeof(BulletList));
  if (out == NULL) { return NULL; }
  out -> val = -1;
  out -> hasNext = 0;
  out -> hasPrev = 0;
  out -> next = NULL;
  out -> prev = NULL;
  return out;
}

// Removes all connections to all other nodes, then returns the node for deletion or other use.
static BulletList* remove (BulletList* list) {

  // Remove pointers from existing nodes.
  if (list->hasNext && list->hasPrev) {
    list->prev->next = list->next;
    list->next->prev = list->prev;
  }
  // If this is the end of the list, set the prev node to the end.
  else if (!(list->hasNext) && list->hasPrev) {
    list->prev->next = NULL;
    list->prev->hasNext = 0;
  }
  // If this is the start of the list, set the next node to the start.
  else if (!(list->hasPrev) && list->hasNext) {
    list->next->prev = NULL;
    list->next->hasPrev = 0;
  }

  // Remove pointers to existing nodes.
  list->next = NULL;
  list->prev = NULL;
  list->hasNext = 0;
  list->hasPrev = 0;
 
  return list;
}

// Appends a new node to the end of the list.
static void append (BulletList* list, BulletList* add) {
  BulletList* cur = list;
  while (cur->hasNext) { cur = cur->next; }
  cur->next = add;
  cur->hasNext = 1;
  add->hasPrev = 1;
  add->prev = cur;
}
