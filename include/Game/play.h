#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"

#include<stdbool.h>

#define BRICKS_COLUMNS 25
#define BRICKS_ROWS 12

typedef struct
{
  HeroWindow* window;
  SDL_Window* sdlWindow;
  HeroInput* input;

  HeroSpriteBatch* spriteBatch;

  GameSpriteSheet* brickSpriteSheet;
  uint8_t bricks[BRICKS_ROWS * BRICKS_COLUMNS];
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);

void gamePlayBricksDraw(GamePlay* play);

#endif