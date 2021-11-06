#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/bricks.h"
#include"Game/racket.h"
#include"Game/ball.h"

typedef struct
{
  HeroWindow* window;
  SDL_Window* sdlWindow;
  HeroInput* input;

  HeroShader* shader;
  HeroSpriteBatch* spriteBatch;

  GameRacket* racket;
  GameBall* ball;
  GameBricks* bricks;

  bool started;
  bool paused;
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);

#endif