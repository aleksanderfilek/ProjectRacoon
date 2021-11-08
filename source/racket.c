#include"Game/racket.h"

#include<stdlib.h>

GameRacket* racketCreate()
{
  GameRacket* racket = (GameRacket*)malloc(sizeof(GameRacket));

  racket->spriteSheet = gameSpriteSheetLoad("assets/sprites/Racket.he");
  racket->rect = gameSpriteSheetGetRectByName(racket->spriteSheet, "racketstandard");
  racket->size = (HeroInt2){racket->rect.z - racket->rect.x , racket->rect.w - racket->rect.y};
  racket->position = (HeroFloat2){ 604.0f, 695.0f};

  racket->collider = (BoxCollider2D){ racket->position, (HeroFloat2){racket->size.x,racket->size.y} };

  racket->speedModifier = 1.0f;
  racket->ballInRange = false;

  return racket;
}

void racketDestroy(GameRacket* racket)
{
  gameSpriteSheetUnload(racket->spriteSheet);

  free(racket);
}

void racketUpdate(GameRacket* racket, double deltaTime, HeroInput* input)
{
  int dir = 0;
  if(heroInputKeyPressed(input, HERO_KEYCODE_LEFT))
  {
    dir = -1;
  }
  else if(heroInputKeyPressed(input, HERO_KEYCODE_RIGHT))
  {
    dir = 1;
  }

  racket->position.x += (double)dir * RACKET_SPEED * (double)racket->speedModifier * deltaTime;

  if(racket->position.x < 15)
  {
    racket->position.x = 15.0f;
  }
  else if(racket->position.x + racket->size.x > 1265)
  {
    racket->position.x = 1265 - racket->size.x;
  }

  racket->collider.position = racket->position;
}

void racketDraw(GameRacket* racket, HeroSpriteBatch* spriteBatch)
{
  HeroInt2 position = {(int)racket->position.x, (int)racket->position.y};
  
  heroSpriteBatchDrawTextureEx(spriteBatch, racket->spriteSheet->texture,
    position, racket->size, racket->rect, 0.0f, (HeroColor){0xFF,0xFF,0xFF,0xFF});
}

void racketPositioning(GameRacket* racket, GameBall* ball)
{
  ball->position = racket->position;
  ball->position = heroMathAddF2(ball->position, (HeroFloat2){32, -8});
}

bool racketBallBounce(GameRacket* racket, GameBall* ball)
{
  Collision collision = detectBoxCircleCollision(&racket->collider, &ball->collider);
  if(collision.collided == true)
  {
    float centerRacket = racket->position.x + racket->size.x / 2.0f;
    float distance = (ball->position.x + ball->collider.radius) - centerRacket;
    float percentage = distance / (racket->size.x / 2.0f);

    float strength = 2.0f;
    HeroFloat2 oldVelocity = ball->velocity;
    ball->velocity.x = 100.0f * percentage * strength;
    ball->velocity.y = -ball->velocity.y;
    ball->velocity = heroMathMultiplyF2(heroMathNormalizeF2(ball->velocity), heroMathLengthF2(oldVelocity));
    
    return true;
  }
  return false;
}