#include"Game/ball.h"

#include<stdlib.h>
#include<math.h>

static bool bounceWall(GameBall* ball);

GameBall* ballCreate()
{
  GameBall* ball = (GameBall*)malloc(sizeof(GameBall));

  ball->spriteSheet = gameSpriteSheetLoad("assets/sprites/Ball.he");
  ball->currentSpriteIndex = gameSpriteSheetGet(ball->spriteSheet, "ballstandard");
  ball->rect = gameSpriteSheetGetRect(ball->spriteSheet, ball->currentSpriteIndex);
  ball->size = (HeroInt2){ball->rect.z - ball->rect.x , ball->rect.w - ball->rect.y};

  ball->collider = (CircleCollider2D){heroFloat2zero, ball->size.x / 2.0f};

  ball->velocity = (HeroFloat2){000.0f, -350.0f};
  ball->speedModifier = 1.0f;

  return ball;
}

void ballDestroy(GameBall* ball)
{
  gameSpriteSheetUnload(ball->spriteSheet);

  free(ball);
}

bool ballUpdate(GameBall* ball, double deltaTime)
{
  float speed = ball->speedModifier * deltaTime;

  ball->position = heroMathAddF2(ball->position,heroMathMultiplyF2(ball->velocity, speed));

  ball->collider.position = ball->position;

  bounceWall(ball);
}

void ballDraw(GameBall* ball, HeroSpriteBatch* spriteBatch)
{
  HeroInt2 position = {(int)ball->position.x, (int)ball->position.y};
  HeroInt4 rect = gameSpriteSheetGetRect(ball->spriteSheet, ball->currentSpriteIndex);
  HeroInt2 size = {rect.z - rect.x , rect.w - rect.y};
  
  heroSpriteBatchDrawTextureEx(spriteBatch, ball->spriteSheet->texture,
    position, size, rect, 0.0f, (HeroColor){0xFF,0xFF,0xFF,0xFF});
}

static bool bounceWall(GameBall* ball)
{
  if(ball->position.x < 15.0f)
  {
    ball->velocity.x = -ball->velocity.x;
    ball->position.x = 15.0f;
  } 
  else if(ball->position.x + ball->size.x > 1265.0f)
  {
    ball->velocity.x = -ball->velocity.x;
    ball->position.x = 1265.0f - ball->size.x;
  }

  if(ball->position.y < 15.0f)
  {
    ball->velocity.y = -ball->velocity.y;
    ball->position.y = 15.0f;
  }
  else if(ball->position.y > 720.0f)
  {
    return false;
  }

  return true;
}