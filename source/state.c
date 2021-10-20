#include"Game/state.h"

#include<stdlib.h>

static void checkState(GameState* gameState);

GameState* gameStateInit()
{
  GameState* state = (GameState*)malloc(sizeof(GameState));

  state->init[(int)GAMESTATE_MENU] = gameMenuInit;
  state->init[(int)GAMESTATE_PLAY] = gamePlayInit;
  DEBUG_CODE( state->init[(int)GAMESTATE_LEVELEDITOR] = gameLevelEditorInit; )

  state->update[(int)GAMESTATE_MENU] = gameMenuUpdate;
  state->update[(int)GAMESTATE_PLAY] = gamePlayUpdate;
  DEBUG_CODE( state->update[(int)GAMESTATE_LEVELEDITOR] = gameLevelEditorUpdate; )

  state->destory[(int)GAMESTATE_MENU] = gameMenuDestroy;
  state->destory[(int)GAMESTATE_PLAY] = gamePlayDestroy;
  DEBUG_CODE( state->destory[(int)GAMESTATE_LEVELEDITOR] = gameLevelEditorDestroy; )

  state->currentState = GAMESTATE_MENU;
  state->currentStateData = state->init[(int)state->currentState]();
  state->nextState = GAMESTATE_NONE;

  return state;
}

void gameStateUpdate(void* ptr)
{
  GameState* state = (GameState*)ptr;

  checkState(state);

  state->update[(int)state->currentState](state->currentStateData);
}

void gameStateDestroy(void* ptr)
{
  GameState* state = (GameState*)ptr;

  state->destory[(int)state->currentState](state->currentStateData);

  free(state);
}

void gameStateChange(GameState* gameState, GameStateEnum newState)
{
  gameState->nextState = newState;
}

static void checkState(GameState* gameState)
{
  if(gameState->nextState == GAMESTATE_NONE)
  {
    return;
  }

  gameState->destory[(int)gameState->currentState](gameState->currentStateData);
  gameState->currentState = gameState->nextState;
  gameState->currentStateData = gameState->init[(int)gameState->currentState]();
  gameState->nextState = GAMESTATE_NONE;
}