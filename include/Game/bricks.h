#ifndef GAME_BRICKS_H_
#define GAME_BRICKS_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"
#include"Game/physics.h"
#include"Game/ball.h"

#define BRICKS_COLUMNS 25
#define BRICKS_ROWS 12

typedef struct
{
  GameSpriteSheet* spriteSheet;
  uint8_t ids[BRICKS_ROWS * BRICKS_COLUMNS];
  uint8_t currentIds[BRICKS_ROWS * BRICKS_COLUMNS];
  BoxCollider2D colliders[BRICKS_ROWS * BRICKS_COLUMNS];
}GameBricks;

GameBricks* gameBricksCreate();
void gameBricksDraw(const GameBricks* bricks, HeroSpriteBatch* spriteBatch);
void gameBricksDestory(GameBricks* bricks);
void gameBricksCheckCollisions(GameBricks* bricks, GameBall* ball);
void gameBricksLoadLevel(GameBricks* bricks, const char* path);

#endif