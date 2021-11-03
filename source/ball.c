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

  ball->direction = (HeroFloat2){1.0f, -1.0f};
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
  float speed = BALL_SPEED * ball->speedModifier * deltaTime;
  ball->velocity = heroMathMultiplyF2(ball->direction, speed);

  ball->position = heroMathAddF2(ball->position, ball->velocity);

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
#include<stdio.h>
static bool bounceWall(GameBall* ball)
{
  if(ball->position.x < 15.0f)
  {
    float penetration = (ball->position.x - 15.0f) / fabs(ball->velocity.x);
    HeroFloat2 penetrationVector = heroMathMultiplyF2(ball->velocity, penetration);
    ball->position = heroMathAddF2(ball->position, penetrationVector);

    ball->direction.x *= -1.0f;
    penetrationVector.y *= -1.0f;
    ball->position = heroMathAddF2(ball->position, penetrationVector);
  } 
  else if(ball->position.x + ball->size.x > 1265.0f)
  {
    float penetration = (1265.0f - ball->position.x + ball->size.x) / ball->velocity.x;
    HeroFloat2 penetrationVector = heroMathMultiplyF2(ball->velocity, penetration);
    ball->position = heroMathAddF2(ball->position, penetrationVector);

    ball->direction.x *= -1.0f;
    penetrationVector.y *= -1.0f;
    ball->position = heroMathAddF2(ball->position, penetrationVector);
  }

  if(ball->position.y < 15.0f)
  {
    float penetration = (ball->position.y - 15.0f) / ball->velocity.y;
    HeroFloat2 penetrationVector = heroMathMultiplyF2(ball->velocity, penetration);
    ball->position = heroMathAddF2(ball->position, penetrationVector);

    ball->direction.y *= -1.0f;
    penetrationVector.y *= -1.0f;
    ball->position = heroMathAddF2(ball->position, penetrationVector);
  }
  else if(ball->position.y > 695.0f)
  {
    return false;
  }


  return true;
}