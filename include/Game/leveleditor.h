#ifndef GAME_LEVELEDITOR_H_
#define GAME_LEVELEDITOR_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/state.h"
#include"Game/spritesheet.h"
#include"Game/ui.h"

typedef struct
{
  HeroWindow* mainWindow;
  SDL_Window* mainSdlWindow;
  HeroInput* input;

  HeroWindow* toolWindow;
  SDL_Window* toolSdlWindow;

  HeroSpriteBatch* toolSpriteBatch;
  HeroSpriteBatch* mainSpriteBatch;
  HeroShader* shader;

  GameSpriteSheet* levelEditorSpriteSheet;

  UIWidget* toolWidget;
} GameLevelEditor;

void* gameLevelEditorInit();
void gameLevelEditorUpdate(void* ptr);
void gameLevelEditorDestroy(void* ptr);

#endif