#ifndef GAME_RACKET_H_
#define GAME_RACKET_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"
#include"Game/ball.h"

#define RACKET_SPEED 300

typedef struct
{
  GameSpriteSheet* spriteSheet;
  HeroInt4 rect;
  HeroInt2 size;

  HeroFloat2 position;
  float speedModifier;
}GameRacket;

GameRacket* racketCreate();
void racketDestroy(GameRacket* racket);
void racketUpdate(GameRacket* racket, double deltaTime, HeroInput* input);
void racketDraw(GameRacket* racket, HeroSpriteBatch* spriteBatch);
void racketPositioning(GameRacket* racket, GameBall* ball);
void racketBallBounce(GameRacket* racket, GameBall* ball);

#endif