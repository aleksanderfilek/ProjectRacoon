#ifndef GAME_BRICKS_H_
#define GAME_BRICKS_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"
#include"Game/physics.h"
#include"Game/ball.h"

#include<stdbool.h>

#define BRICKS_COLUMNS 25
#define BRICKS_ROWS 12
#define BRICKS_COUNT BRICKS_ROWS*BRICKS_COLUMNS
#define BRICKS_BLOCK_SIZE BRICKS_COUNT*sizeof(uint8_t)

typedef struct
{
  int arg1;
} GameBrickData;

typedef struct
{
  GameSpriteSheet* spriteSheet;
  uint8_t ids[BRICKS_COUNT];
  uint8_t currentIds[BRICKS_COUNT];
  GameBrickData datas[BRICKS_COUNT];
  BoxCollider2D colliders[BRICKS_COUNT];
  uint32_t count;
  uint32_t currentCount;
}GameBricks;

GameBricks* gameBricksCreate();
void gameBricksDraw(const GameBricks* bricks, HeroSpriteBatch* spriteBatch);
void gameBricksDestory(GameBricks* bricks);
int gameBricksCheckCollisions(GameBricks* bricks, GameBall* ball);
void gameBricksLoadLevel(GameBricks* bricks, const char* path);
void gameBricksResolveChange(GameBricks* bricks, int index);
void gameBricksDataSet(GameBricks* bricks);
void gameBricksReset(GameBricks* bricks);

#endif