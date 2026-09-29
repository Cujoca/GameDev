#include "EntityCollections.h"
#include <raylib.h>
#include <stdlib.h>

/* Just mallocs a new Entity pointer which is filled with arbitrary values (-1)
 * If nothing is changed when used, the arbitrary numbers should cause errors.
 *
 * By default:
 *  coords -> {-1, -1}
 *  state -> -1
 */
static Entity* createEntity () {
  Entity* out = (Entity*)malloc(sizeof(Entity));

  out->coords = (Vector2){-1, -1};
  out->state = -1;

  return out;
}

//static void freeEntity(Entity* old) {}

/* Takes an Entity and creates a listnode for it.
 *
 * args:
 *  Entity* val -> the Entity being put into a node
 *
 * returns:
 *  EntityList* out -> the filled EntityList with default values, see below for details
 *
 * By default:
 *  val  -> val
 *  next -> NULL
 *  prev -> NULL
 *
 *  hasVal  -> 1(true)
 *  hasNext -> 0(false)
 *  hasPrev -> 0(false)
 */
static EntityList* createEntityList(Entity* val) {
  EntityList* out = (EntityList*)malloc(sizeof(EntityList));
  out->val = val;
  out->hasNext = 0;
  out->hasPrev = 0;
  out->hasVal  = 1;

  out->next = NULL;
  out->prev = NULL;

  return out;
}

/* Creates a fully empty node (no val like above) intended to be used as the pointer
 * to a list of other nodes, need a dummy head like this for efficient removal, there's
 * zero chance of this head accidentally being freed (inside this library anyways) so
 * if something fucks up then it's the user's fault, not mine. Try better next time bozo.
 *
 * args: none
 *
 * returns:
 *  EntityList* out -> dummy list node with default values, see below for details
 *
 * By default:
 *  val  -> NULL
 *  next -> NULL
 *  prev -> NULL
 *
 *  hasVal  -> 0(false)
 *  hasNext -> 0(false)
 *  hasPrev -> 0(false)
 */
static EntityList* dummyEntityList () {
  EntityList* out = (EntityList*)malloc(sizeof(EntityList));

  out->val = NULL;
  out->hasVal = 0;

  out->hasNext = 0;
  out->hasPrev = 0;
  out->next = NULL;
  out->prev = NULL;

  return out;
}

/* Frees all content associated with an EntityList node, along with the node itself.
 * Also sets the node's prev and next to NULL, just in case something weird happened.
 * NOTE:  does in fact free the val inside, the node itself should be moved if you want
 *        to also move the entity. Or create a new one idk.
 *
 *  args:
 *    EntityList* old -> the node to be freed
 */
static void freeEntityList (EntityList* old) {
  if (old->hasVal) { free(old->val); }
  old->next = NULL;
  old->prev = NULL;

  free(old);
}

/* Frees an entire list from front to back, including the dummy head.
 *
 * args:
 *  EntityList* head -> dummy head of list to be freed
 */
static void freeEntireList (EntityList* head) {
  EntityList* cur = head;

  while (cur->hasNext) {
    cur = cur->next;
    freeEntityList(cur->prev);
  }

  freeEntityList(cur);
}

/* Removes a given node from a list. Do note that this is NOT supposed to be called on the
 * dummy head, despite supporting the ability to do so. Returns the removed node rather than
 * automatically freeing, this is to support later list functions but also because the removed
 * node might not always be immediately freed, it's up to the user.
 *
 * args:
 *  EntityList* cur -> node to be removed
 *
 * returns:
 *  EntityList* cur -> the removed node with all of its connections severed
 */
static EntityList* remove (EntityList* cur) {

  // if has both neighbours
  if (cur->hasNext && cur->hasPrev) {
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
  }

  // if is tail, set prev to tail
  else if (cur->hasPrev) {
    cur->prev->next = NULL;
    cur->hasPrev = 0;
  }

  // if is somehow head, set next to head
  else if (cur->hasNext) {
    cur->next->prev = NULL;
    cur->hasNext = 0;
  }

  cur->next = NULL;
  cur->prev = NULL;
  cur->hasNext = 0;
  cur->hasPrev = 0;

  return cur;
}

/* Removes the last element of a list, following LIFO order. Function assumes
 * that the head node is the dummy head, though it doesn't really matter much in this case.
 *
 * args:
 *  EntityList* head -> the head node of the list
 *
 * returns:
 *  EntityList* cur -> the removed node
 */
static EntityList* pop (EntityList* head) {
  EntityList* cur = head;

  while (cur->hasNext) { cur = cur->next; }

  return remove(cur);
}

/* Removes the first node of a list. Assumes that the head node is the dummy node. Unlike
 * pop, this actually matters since if a node that is NOT the dummy head is passed in, it will
 * remove the next node, not removing the actual first node of the list.
 *
 * NOTE:  please use this only on the dummy head, otherwise you'll get incorrect behaviour
 *        and break the classic queue structure.
 *
 * args:
 *  EntityList* head -> dummy head of the list
 *
 * returns:
 *  EntityList* N/A -> the removed node of the list
 */
static EntityList* dequeue (EntityList* head) {
  // if is empty list, just returns the dummy head.
  if (!head->hasNext) {return head;}

  // head next should be the first node of the list, removing it follows FIFO order.
  return remove(head->next);
}

/* Appends a node to a list. In other words, adds a node to the end of the list.
 *
 * args:
 *  EntityList* head -> dummy head of the list to add to
 *  EntityList* add  -> node to be added to list
 */
static void append (EntityList* head, EntityList* add) {
  EntityList* cur = head;

  while (cur->hasNext) { cur = cur->next; }
  // checks if list is empty. If is empty, then we don't set the addition's
  // prev to be the dummy head, as the dummy head to list connection is one
  // sided, dummy head points to list, list DOES NOT point to dumy head.
  if (cur == head) { head->next = add; }
  else {
    cur->next = add;
    cur->hasNext = 1;
  }


  add->prev = cur;
  add->hasPrev = 1;
}

/* Prepends a node to a list. In other words, adds a node to the start of the list.
 *
 * args:
 *  EntityList* head -> dummy head of the list to add to
 *  EntityList* add  -> node to be added to list
 */
static void prepend (EntityList* head, EntityList* add) {
  // if list is not empty, need to insert in node naturally.
  // otherwise add's next stays NULL.
  if (head->hasNext) {
    head->next->prev = add;
    head->next->hasPrev = 1;
    add->next = head->next;
    add->hasNext = 1;
  }

  head->next = add;
  head->hasNext = 1;
}

/* Is just a wrapper for append, only renamed so that it follows general stack language
 *
 * args:
 *  EntityList* head -> dummy head of stack to add to
 *  EntityList* add  -> node being pushed onto stack
 */
static void push (EntityList* head, EntityList* add) { append(head, add); }

/* Is just a wrapper for append, only renamed so that it follows general queue language
 *
 * args:
 *  EntityList* head -> dummy head of queue to add to
 *  EntityList* add  -> node being enqueued onto queue
 */
static void enqueue (EntityList* head, EntityList* add) { append(head, add); }

