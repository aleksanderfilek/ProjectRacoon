#ifndef GAME_LEVELEDITOR_H_
#define GAME_LEVELEDITOR_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/state.h"
#include"Game/spritesheet.h"
#include"Game/ui.h"
#include"Game/play.h"

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

  char** infoText;
  UIWidget* toolWidget;
  UIWidget* saveWidget;
  
  char title[64];
  bool changed;
  uint32_t currentBrick;
  char* currentPath;
  HeroInt4 emptyBrickRect;
  GameBricks* bricks;

  bool playing;
  GamePlay* play;
} GameLevelEditor;

void* gameLevelEditorInit();
void gameLevelEditorUpdate(void* ptr);
void gameLevelEditorDestroy(void* ptr);

void gameDrawMain(GameLevelEditor* levelEditor);
void gameDrawTool(GameLevelEditor* levelEditor);
void gameSave(GameLevelEditor* levelEditor);

#endif