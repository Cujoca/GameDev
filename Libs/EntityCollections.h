#include <raylib.h>
#include <stdlib.h>

/* Simplistic struct to hold basic data about an entity, meant to be used in collections of entities.
 * Could expand upon a bit to hold more advanced details about an entity, but I have no clue what
 * I need just yet, so I'll hold off on that until I finish out the rest of the lib so I know
 * what I need.
 *
 * Members
 *  Vector2 coords - the x and y coordinates of the entity.
 *  int state      - taken from lab2a, in case the entity has a state that may or may not
 *                   change in between ticks, such as an animated sprite running sideways.
 */
typedef struct Entity {
  Vector2 coords;
  int state;
} Entity;

/* A struct to hold a dynamically allocated doubly linked list of entities.
 * Each node is its own self-contained instance, exists independantly of the
 * head or the rest of the list, save for its direct neighbours. Uses ints
 * to hold the state of its connections, similar to scala practice.
 *
 * Members:
 *  Entity pointer val             - the actual entity held by the node.
 *  int hasNext, hasPrev, hasVal   - status checks for each of the node's pointers, to avoid referencing null.
 *  EntityList pointers next, prev - the pointers to the node's neighbours.
 */
typedef struct EntityList {
  Entity* val;
  int hasNext, hasPrev, hasVal;
  struct EntityList *next, *prev;
} EntityList;

// entity creation and deletion functions
static Entity* createEntity ();
//static void    freeEntity (Entity* old); might be needed later

// list creation and deletion functions
static EntityList* createEntityList (Entity* val);
static EntityList* dummyEntityList  ();
static void        freeEntity       (EntityList* old);
static void        freeEntireList   (EntityList* head);

// list, stack, and queue removal operations.
// Do note that pop and dequeue need the dummy node, not the node being removed.
static EntityList* remove  (EntityList* cur); // removes at cur position
static EntityList* pop     (EntityList* head); // stack (removes tail)
static EntityList* dequeue (EntityList* head); // queue (removes head)

// list, stack, and queue addition operations. Do note that the head is assumed to be the dummy node.
static void append  (EntityList* head, EntityList* add); // adds onto tail
static void prepend (EntityList* head, EntityList* add); // adds before head
static void push    (EntityList* head, EntityList* add); // stack (will be wrapper for append)
static void enqueue (EntityList* head, EntityList* add); // queue (will be wrapper for append)
