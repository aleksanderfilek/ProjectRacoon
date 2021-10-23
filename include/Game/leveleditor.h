#ifndef GAME_LEVELEDITOR_H_
#define GAME_LEVELEDITOR_H_

#include<stdbool.h>

#include"Hero\Hero.h"

#include"Game\state.h"

typedef struct
{
  SDL_Window* sdlWindow;
  HeroInput* input;

  HeroWindow* toolWindow;
  SDL_Window* toolSdlWindow;
} GameLevelEditor;

void* gameLevelEditorInit();
void gameLevelEditorUpdate(void* ptr);
void gameLevelEditorDestroy(void* ptr);

#endif