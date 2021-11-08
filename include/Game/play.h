#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/bricks.h"
#include"Game/racket.h"
#include"Game/ball.h"
#include"Game/ui.h"

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

  UIWidget* pauseWidget;
  HeroTexture* pauseTextures[2];

  HeroSound* sounds[2];
  uint32_t currentSound;
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);
void gamePlayRestart(GamePlay* play);

DEBUG_CODE( 
void* gameDebugPlayInit(HeroWindow* window, HeroInput* input, 
  HeroShader* shader, GameBricks* bricks); 
void gameDebugPlayDestroy(void* ptr);
void gameDebugPlayDraw(GamePlay* play);
void gameDebugPlayUpdate(GamePlay* play);
  )
#endif