#include"Game/bricks.h"

#include<stdlib.h>
#include<string.h>
#include<math.h>

GameBricks* gameBricksCreate()
{
  GameBricks* bricks = (GameBricks*)malloc(sizeof(GameBricks));
  memset(bricks->ids, 1, BRICKS_COLUMNS*BRICKS_ROWS*sizeof(uint8_t));
  bricks->spriteSheet = gameSpriteSheetLoad("assets/sprites/Bricks.he");

  HeroFloat2 size = (HeroFloat2){
    bricks->spriteSheet->rects[0].z - bricks->spriteSheet->rects[0].x,
    bricks->spriteSheet->rects[0].w - bricks->spriteSheet->rects[0].y };

  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      HeroFloat2 position = { 15 + 50*x, 15 + 24*y };
      bricks->colliders[index].position = position;
      bricks->colliders[index].size = size;
    }
  }

  return bricks;
}

void gameBricksDraw(const GameBricks* bricks, HeroSpriteBatch* spriteBatch)
{
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      if(bricks->ids[index] == 0)
      {
        continue;
      }

      HeroInt2 position = { 15 + 50*x, 15 + 24*y };
      HeroInt4 rect = gameSpriteSheetGetRect(bricks->spriteSheet, bricks->ids[index]-1);
      heroSpriteBatchDrawTextureEx(spriteBatch, bricks->spriteSheet->texture,
        position, (HeroInt2){50, 24}, rect, 0.0f, (HeroColor){0xFF,0xFF,0xFF,0xFF});
    }
  }
}

void gameBricksDestory(GameBricks* bricks)
{
  gameSpriteSheetUnload(bricks->spriteSheet);
  free(bricks);
}

static int collisionEdgeVariant(HeroFloat2 vector);

void gameBricksCheckCollisions(GameBricks* bricks, GameBall* ball)
{
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      if(bricks->ids[index] == 0)
      {
        continue;
      }

      Collision collision = detectBoxCircleCollision(&bricks->colliders[index], &ball->collider);
      if(collision.collided == true)
      {
        bricks->ids[index] = 0;
        int edge = collisionEdgeVariant(collision.direction);
        if(edge == 1 || edge == 3)
        {
          ball->velocity.x = -ball->velocity.x;
          float penetration = ball->collider.radius * fabs(collision.direction.x);
          if(edge == 1)
          {
            ball->position.x -= penetration;
          }
          else
          {
            ball->position.x += penetration;
          }
        }
        else
        {
          ball->velocity.y = -ball->velocity.y;
          float penetration = ball->collider.radius * fabs(collision.direction.y);
          if(edge == 0)
          {
            ball->position.y -= penetration;
          }
          else
          {
            ball->position.y += penetration;
          }
        }
        return;
      }
    }
  }
}

static int collisionEdgeVariant(HeroFloat2 vector)
{
  HeroFloat2 compass[4] = {
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 0.0f, -1.0f },
    { -1.0f, 0.0f }
  };
  float max = 0.0f;
  unsigned int bestMatch = -1;
  for (unsigned int i = 0; i < 4; i++)
  {
    float dotProduct = heroMathDotProductF2(heroMathNormalizeF2(vector), compass[i]);
    if(dotProduct > max)
    {
      max = dotProduct;
      bestMatch = i;
    }
  }

  return bestMatch;
}