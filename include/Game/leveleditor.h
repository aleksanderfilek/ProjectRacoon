#ifndef GAME_LEVELEDITOR_H_
#define GAME_LEVELEDITOR_H_

#include<stdbool.h>

#include"Hero/Hero.h"

#include"Game/state.h"
#include"Game/spritesheet.h"
#include"Game/ui.h"

#define BRICKS_COLUMNS 25
#define BRICKS_ROWS 10

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
  GameSpriteSheet* brickSpriteSheet;
  char** infoText;
  UIWidget* toolWidget;
  UIWidget* mainWidget;
  
  bool changed;
  uint32_t currentBrick;

  uint8_t bricks[BRICKS_ROWS * BRICKS_COLUMNS];
} GameLevelEditor;

void* gameLevelEditorInit();
void gameLevelEditorUpdate(void* ptr);
void gameLevelEditorDestroy(void* ptr);

#endif