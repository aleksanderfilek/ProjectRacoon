#ifndef GAME_BALL_H_
#define GAME_BALL_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"

#define BALL_SPEED 200

typedef struct
{
  GameSpriteSheet* spriteSheet;
  GameSpriteSheetIndex currentSpriteIndex;
  HeroInt2 size;
  HeroInt4 rect;

  HeroFloat2 position;
  HeroFloat2 direction;
  HeroFloat2 velocity;
  float speedModifier;
}GameBall;

GameBall* ballCreate();
void ballDestroy(GameBall* ball);
bool ballUpdate(GameBall* ball, double deltaTime);
void ballDraw(GameBall* ball, HeroSpriteBatch* spriteBatch);

#endif