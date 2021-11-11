#include"Game/bricks.h"

#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdio.h>

GameBricks* gameBricksCreate()
{
  GameBricks* bricks = (GameBricks*)malloc(sizeof(GameBricks));
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
      if(bricks->currentIds[index] == 0)
      {
        continue;
      }

      HeroInt2 position = { 15 + 50*x, 15 + 24*y };
      HeroInt4 rect = gameSpriteSheetGetRect(bricks->spriteSheet, bricks->currentIds[index]-1);
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

int gameBricksCheckCollisions(GameBricks* bricks, GameBall* ball)
{
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      if(bricks->currentIds[index] == 0)
      {
        continue;
      }

      Collision collision = detectBoxCircleCollision(&bricks->colliders[index], &ball->collider);
      if(collision.collided == true)
      {
        int edge = collisionEdgeVariant(collision.direction);
        if(edge == 1 || edge == 3)
        {
          ball->velocity.x = -ball->velocity.x;          
        }
        else
        {
          ball->velocity.y = -ball->velocity.y;
        }
        HeroFloat2 shiftVector = heroMathMultiplyF2(collision.direction, -1.0f);
        ball->position = heroMathAddF2(ball->position, shiftVector);
        return index;
      }
    }
  }

  return -1;
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

void gameBricksLoadLevel(GameBricks* bricks, const char* path)
{
  printf("[Bricks] Load level, path: %s\n", path);
  FILE* file = fopen(path, "rb");
  if(file == NULL)
  {
    printf("[Bricks] Could not load level, path: %s\n", path);
  }

  fread(bricks->ids, sizeof(uint8_t), BRICKS_COLUMNS * BRICKS_ROWS, file);

  for(int i = 0; i < BRICKS_COLUMNS * BRICKS_ROWS; i++)
  {
    bricks->count++;
  }

  fclose(file);
}

void gameBricksResolveChange(GameBricks* bricks, int index)
{
  if(index < 0) return;

  int id = bricks->currentIds[index] - 1;
  switch(id)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      bricks->currentIds[index] = 0;
      bricks->currentCount--;
    break;
    case 7:
      bricks->datas[index].arg1--;
      bricks->currentIds[index] = 9;
      break;
    case 8:
      bricks->datas[index].arg1--;
      bricks->currentIds[index] = 0;
      bricks->currentCount--;
      break;
    case 9: //indestructible
      break;
  }
}

void gameBricksDataSet(GameBricks* bricks)
{
  for(int i = 0; i < BRICKS_COUNT; i++)
  {
    switch(i)
    {
      case 7:
        bricks->datas[i].arg1 = 2;
        break;
      case 8:
        bricks->datas[i].arg1 = 1;
        break;
    }
  }
}

void gameBricksReset(GameBricks* bricks)
{
  memcpy(bricks->currentIds, bricks->ids, BRICKS_BLOCK_SIZE);
  bricks->currentCount = bricks->count;
  gameBricksDataSet(bricks);
}