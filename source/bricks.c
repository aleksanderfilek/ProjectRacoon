#include"Game/bricks.h"

#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdio.h>

static void addBrickEvent(GameBricks* bricks, uint32_t index, BrickEventType type);

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

  bricks->events = (BrickEvent*)malloc(BRICKEVENTS_CAPACITY * sizeof(BrickEvent));
  bricks->eventsCapacity = BRICKEVENTS_CAPACITY;
  bricks->eventsNumber = 0;

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
  free(bricks->events);

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

  bricks->count = 0;
  for(int i = 0; i < BRICKS_COLUMNS * BRICKS_ROWS; i++)
  {
    if(bricks->ids[i] > 0)
    {
      bricks->count++;
    }
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
    case 10:
    case 11:
      bricks->currentIds[index] = 0;
      bricks->currentCount--;
    break;
    case 7:
      bricks->currentIds[index] = 9;
      break;
    case 8:
      bricks->currentIds[index] = 0;
      bricks->currentCount--;
      break;
    case 9: //indestructible
      break;
    case 12:
      addBrickEvent(bricks, index, BRICK_EVENT_EXPLOSION);
      bricks->currentIds[index] = 0;
      bricks->currentCount--;
      break;
  }
}

void gameBricksReset(GameBricks* bricks)
{
  memcpy(bricks->currentIds, bricks->ids, BRICKS_BLOCK_SIZE);
  bricks->currentCount = bricks->count;

  bricks->eventsNumber = 0;
  bricks->eventsCapacity = BRICKEVENTS_CAPACITY;
  bricks->events = (BrickEvent*)realloc(bricks->events, bricks->eventsCapacity*sizeof(BrickEvent));
  memset(bricks->events, 0 , bricks->eventsCapacity * sizeof(BrickEvent));
}

void gameBricksAnimation(GameBricks* bricks)
{
  for(int i = 0; i < BRICKS_COUNT; i++)
  {
    int id = bricks->currentIds[i] - 1;
    switch(id)
    {
      case 10:
        bricks->currentIds[i] = 12;
        break;
      case 11:
        bricks->currentIds[i] = 11;
        break;
    }
  }
}

static void addBrickEvent(GameBricks* bricks, uint32_t index, BrickEventType type)
{
  int freeIndex = -1;
  for(int i = 0; i < bricks->eventsNumber; i++)
  {
    if(bricks->events[i].index == index)
    {
      return;
    }
    if(bricks->events[i].type == BRICK_EVENT_NONE)
    {
      freeIndex = i;
      break;
    }
  }

  if(freeIndex >= 0)
  {
    bricks->events[freeIndex].type = type;
    bricks->events[freeIndex].index = index;
    switch (type)
    {
    case BRICK_EVENT_EXPLOSION:
      bricks->events[freeIndex].timer = EXPLOSION_TIMER;
      break;
    }
    return;
  }

  if(bricks->eventsNumber >= bricks->eventsCapacity)
  {
    bricks->eventsCapacity += BRICKEVENTS_CAPACITY;
    bricks->events = (BrickEvent*)realloc(bricks->events, bricks->eventsCapacity*sizeof(BrickEvent));
  }

  bricks->events[bricks->eventsNumber].type = type;
  bricks->events[bricks->eventsNumber].index = index;
  switch (type)
    {
    case BRICK_EVENT_EXPLOSION:
      bricks->events[bricks->eventsNumber].timer = EXPLOSION_TIMER;
      break;
    }
  bricks->eventsNumber++;
}

void gameBricksEvents(GameBricks* bricks, double deltaTime)
{
  for(int i = 0; i < bricks->eventsNumber; i++)
  {
    if(bricks->events[i].type == BRICK_EVENT_NONE)
    {
      continue;
    }

    if(bricks->events[i].timer <= 0.0f)
    {
      switch(bricks->events[i].type)
      {
        case BRICK_EVENT_EXPLOSION:
          for(int y = -1; y <= 1; y++)
          {
            for(int x = -1; x <= 1; x++)
            {
              int idY = bricks->events[i].index / BRICKS_COLUMNS + y;
              if(idY < 0 || idY >= BRICKS_ROWS) continue;
              int idX = bricks->events[i].index % BRICKS_COLUMNS + x;
              if(idX < 0 || idX >= BRICKS_COLUMNS) continue;
              int id = bricks->events[i].index + BRICKS_COLUMNS*y + x;
              if(bricks->events[i].index != id)
              {
                gameBricksResolveChange(bricks, id);
              }
            }
          }
          break;
      }
      bricks->events[i].type = BRICK_EVENT_NONE;
      continue;
    }
    bricks->events[i].timer -= (float)deltaTime;
  }
}