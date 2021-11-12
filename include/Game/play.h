#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#define ANIMATION_FRAME_TIME 1.0f

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/bricks.h"
#include"Game/racket.h"
#include"Game/ball.h"
#include"Game/ui.h"

typedef enum
{
    PLAY_STATE_GAME = 0,
    PLAY_STATE_PAUSE,
    PLAY_STATE_FAILED,
    PLAY_STATE_WIN
} PlayStateEnum;

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

  UIWidget* gameWidget;

  PlayStateEnum currentWidget;
  UIWidget* widgets[3];
  HeroTexture* btnTextures[3];

  HeroSound* sounds[2];
  uint32_t currentSound;

  float animationTimer;
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);
void gamePlayRestart(GamePlay* play);

void setPlayState(GamePlay* play, PlayStateEnum state);

DEBUG_CODE( 
void* gameDebugPlayInit(HeroWindow* window, HeroInput* input, 
  HeroShader* shader, GameBricks* bricks); 
void gameDebugPlayDestroy(void* ptr);
void gameDebugPlayDraw(GamePlay* play);
void gameDebugPlayUpdate(GamePlay* play);
  )
#endif