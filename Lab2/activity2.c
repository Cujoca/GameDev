#include <raylib.h>
#include <stdlib.h>

/* The blue bullet is a four frame spin animation on the bottom left of the sheet.
 * The frames sit in a row at a fixed stride, so one macro covers all of them.
 */
#define BULLET_SIZE   16
#define BULLET_FRAMES 4
#define BULLET_STRIDE 19
#define BULLET_ORIGIN_X 16
#define BULLET_ORIGIN_Y 121

#define BULLET_FRAME(i) ((Rectangle){BULLET_ORIGIN_X + BULLET_STRIDE * (i), \
                                     BULLET_ORIGIN_Y, BULLET_SIZE, BULLET_SIZE})

/* The gun sprite is 199 by 80 and already faces right, so it is drawn whole.
 * It sits flush with the bottom of the window, and its muzzle opening is the
 * gap on the right hand edge, centred 14 pixels down from the sprite's top.
 */
#define GUN_WIDTH    199
#define GUN_HEIGHT   80
#define GUN_MUZZLE_Y 14

/* How far below the gun's top edge a bullet is drawn. The muzzle opening is
 * centred on GUN_MUZZLE_Y, and a bullet is drawn from its own top left corner,
 * so half the sprite comes back off to centre it on the barrel.
 */
#define BULLET_OFFSET_Y (GUN_MUZZLE_Y - BULLET_SIZE / 2)

// A rectangle and its colour, bundled so the gun is one val_xue.
// NOTE: no longer used, can delete
typedef struct ColorRect {
  int pos_x;
  int pos_y;
  int width;
  int height;
  Color color;
} ColorRect;

/* Doubly linked list of bullets in flight. val_x is the bullet's x position.
 *
 * You may be wondering why I chose a DLL.
 * I needed two things:
 *  - keep an dynamic number of bullets in memory
 *  - be able to remove any bullet, as the gun position can change,
 *    meaning that bullet 1 might have a smaller x val_xue than bullet 2
 *
 * A DLL keeps the size dynamic. I can't use a queue, since because of point 2
 * the bullet's x positions aren't specifically in order, so I can't assume that
 * the removal_x of bullets will follow a FIFO order.
 *
 * I could have made a map. Problem is, I don't know how to make a map in C.
 * I've done more than my fair share of prolog-style list exercises in C, so I
 * chose what I know.
 */
typedef struct BulletList {
  int val_x;
  int val_y;
  int hasNext;
  int hasPrev;
  struct BulletList* next;
  struct BulletList* prev;
} BulletList;

// I'm gonna have to start putting these in a library so I don't have to redefine every time.
// maybe a list of 'Entities' where the list records a Vector2 of position, and a link to the sprite?
// or I could actually be a good programmer and figure out a map.

//static void fireGun ();
static void drawGun                   (int pos_y, Texture2D texture);
static void drawBullet                (Vector2 position, Texture2D texture, int state);
static BulletList* createEmptyBullet  ();
static BulletList* remove             (BulletList* bullet);
static void append                    (BulletList* list, BulletList* add);

int main (void) {

  const int height = 600;
  const int width = 800;

  int rectPos_y = 0;

  // was used as a placeholder for the gun sprite, now will be used for window maths
  const ColorRect rect = (ColorRect){0, 0, 199, 80, RED};


  // Dummy head, never drawn, so the list always has something to append to.
  BulletList* bulletsDummy = createEmptyBullet();

  InitWindow(width, height, "Lab 2: cojo0003");

  Texture2D gun = LoadTexture("../assets/Lasergewehr.png");
  Image sheet = LoadImage("../assets/BulletCollection.png");

  // Repaint the sheet's black backdrop to match the window, so the sprites blend in.
  ImageColorReplace(&sheet, BLACK, DARKGRAY);
  Texture2D bulletSheet = LoadTextureFromImage(sheet);
  UnloadImage(sheet);

  SetTargetFPS(60);


  while (!WindowShouldClose()) {

    // Take away 40 so that the mouse is in the center of the gun, not the corner.
    rectPos_y = GetMouseY() - 40;

    if (rectPos_y > height - rect.height) { rectPos_y = height - rect.height; }
    else if (rectPos_y < 0) { rectPos_y = 0; }

    // H toggles the cursor.
    if (IsKeyPressed(KEY_H)) {
      if (IsCursorHidden()) { ShowCursor(); }
      else { HideCursor(); }
    }

    // Left click spawns a bullet at the current mouse position.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      BulletList* newBullet = createEmptyBullet();
      newBullet->val_x = rect.width;
      newBullet->val_y = rectPos_y + BULLET_OFFSET_Y;
      append(bulletsDummy, newBullet);
    }

    BeginDrawing(); {
      ClearBackground(DARKGRAY);

      // Move every bullet along, dropping the ones that have left the screen.
      // cur trails one behind, so remove() relinks it onto the next bullet for us.
      BulletList* cur = bulletsDummy;
      while (cur->hasNext) {

        BulletList* bullet = cur->next;
        bullet->val_x += 5;
        if (bullet->val_x >= width) { free(remove(bullet)); }
        else {
          // Advance one spin frame every 10 pixels of travel, so the bullet turns as it flies.
          int state = (bullet->val_x / 20) % BULLET_FRAMES;
          drawBullet((Vector2){bullet->val_x, bullet->val_y}, bulletSheet, state);
          cur = bullet;
        }
      }

      drawGun(rectPos_y, gun);

    } EndDrawing();
  }

  // Free any bullets still in flight, then the dummy head.
  while (bulletsDummy->hasNext) { free(remove(bulletsDummy->next)); }
  free(bulletsDummy);

  UnloadTexture(gun);
  UnloadTexture(bulletSheet);

  CloseWindow();

  return 0;
}

// Draws the gun at the given x position, resting on the bottom of the window.
static void drawGun (int pos_y, Texture2D texture) {
  DrawTexture(texture, 0, pos_y, WHITE);
}

// Draws a single bullet at the given x position, with state choosing the spin frame.
static void drawBullet (Vector2 position, Texture2D texture, int state) {

  // Guard the index so a state from outside the animation cannot read the wrong sprite.
  const int frame = ((state % BULLET_FRAMES) + BULLET_FRAMES) % BULLET_FRAMES;

  DrawTextureRec(texture, BULLET_FRAME(frame), position, WHITE);
}

// Allocates a blank unlinked node. Returns NULL if out of memory.
static BulletList* createEmptyBullet () {
  BulletList* out = malloc(sizeof(BulletList));
  if (out == NULL) { return NULL; }
  out->val_x = -1;
  out->val_y = -1;
  out->hasNext = 0;
  out->hasPrev = 0;
  out->next = NULL;
  out->prev = NULL;
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
