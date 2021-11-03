#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/spritesheet.h"
#include"Game/racket.h"
#include"Game/ball.h"

#define BRICKS_COLUMNS 25
#define BRICKS_ROWS 12

typedef struct
{
  HeroWindow* window;
  SDL_Window* sdlWindow;
  HeroInput* input;

  HeroShader* shader;
  HeroSpriteBatch* spriteBatch;

  GameSpriteSheet* brickSpriteSheet;
  uint8_t bricks[BRICKS_ROWS * BRICKS_COLUMNS];

  GameRacket* racket;
  GameBall* ball;

  bool started;
  bool paused;
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);

void gamePlayBricksDraw(GamePlay* play);

#endif