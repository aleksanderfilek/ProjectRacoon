#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include"Hero/Hero.h"

#include"Game/menu.h"
#include"Game/play.h"
#include"Game/leveleditor.h"

typedef enum
{
  GAMESTATE_MENU = 0,
  GAMESTATE_PLAY = 1,
  #ifndef DEBUG
  GAMESTATE_COUNT = 2,
  #else
  GAMESTATE_LEVELEDITOR = 2,
  GAMESTATE_COUNT = 3,
  #endif
  GAMESTATE_NONE
} GameStateEnum;

typedef struct
{
  void* currentStateData;
  void* (*init[GAMESTATE_COUNT])();
  void (*update[GAMESTATE_COUNT])(void*);
  void (*destory[GAMESTATE_COUNT])(void*);
  GameStateEnum currentState;
  GameStateEnum nextState;

} GameState;

GameState* gameStateInit();
void gameStateUpdate(void* ptr);
void gameStateDestroy(void* ptr);
void gameStateChange(GameState* gameState, GameStateEnum newState);

#endif