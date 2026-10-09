#ifndef PLAY_H
#define PLAY_H
#include <raylib.h>
#include <stdlib.h>

#define PLAYER_START 96
#define PLAYER_END 100
#define ASSET_DIR "./assets/tinydungeon/Tiles"



enum EntityType {
  PLAYER,
  ENEMY,
  TERRAIN,
  ITEM
};

typedef struct Entity {
  Texture2D tex;
  int entityType;
  Vector2 coords;
  bool real;
} Entity;

Texture2D* LoadPlayerSprites  (char* filepath);
Texture2D* LoadEnemySprites   (char* filepath);
Texture2D* LoadTerrainSprites (char* filepath);
Texture2D* LoadItemSprites (char* filepath);


Entity* makePlayer ();
Entity* makeTerrainArray  (int size);
Entity* makeEnemyArray    (int size);






#endif /* PLAY_H */
