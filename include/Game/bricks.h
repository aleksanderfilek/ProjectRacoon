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

#define BRICKEVENTS_CAPACITY 16

#define EXPLOSION_TIMER 0.25f

typedef enum
{
  BRICK_EVENT_NONE = 0,
  BRICK_EVENT_EXPLOSION 
} BrickEventType;

typedef struct
{
  BrickEventType type;
  uint32_t index;
  float timer;
} BrickEvent;

typedef struct
{
  GameSpriteSheet* spriteSheet;
  uint8_t ids[BRICKS_COUNT];
  uint8_t currentIds[BRICKS_COUNT];
  BoxCollider2D colliders[BRICKS_COUNT];
  uint32_t count;
  uint32_t currentCount;

  BrickEvent* events;
  uint32_t eventsNumber;
  uint32_t eventsCapacity;
}GameBricks;

GameBricks* gameBricksCreate();
void gameBricksDraw(const GameBricks* bricks, HeroSpriteBatch* spriteBatch);
void gameBricksDestory(GameBricks* bricks);
int gameBricksCheckCollisions(GameBricks* bricks, GameBall* ball);
void gameBricksLoadLevel(GameBricks* bricks, const char* path);
void gameBricksResolveChange(GameBricks* bricks, int index);
void gameBricksDataSet(GameBricks* bricks);
void gameBricksReset(GameBricks* bricks);
void gameBricksAnimation(GameBricks* bricks);
void gameBricksEvents(GameBricks* bricks, double deltaTime);

#endif