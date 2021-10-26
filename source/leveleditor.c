#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>

extern HeroCore* core;

void leveleditorclose(void**ptr);

void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  leveleditor->mainWindow = heroCoreModuleGet(core, "window");
  leveleditor->mainSdlWindow = heroWindowGetSdlWindow(leveleditor->mainWindow);
  //heroWindowSetBackgroundColor(leveleditor->mainWindow, (HeroColor){0,255,0,255});
  leveleditor->input = heroCoreModuleGet(core, "input");
glCheckError();
  //leveleditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  glCheckError();
//   heroWindowSetBackgroundColor(leveleditor->toolWindow, (HeroColor){255,0,0,255});
//   heroWindowSetEvent(leveleditor->toolWindow, HERO_WINDOW_CLOSE, leveleditorclose);
//   leveleditor->toolSdlWindow = heroWindowGetSdlWindow(leveleditor->toolWindow);
//   HeroEvent* event = heroCoreModuleGet(core, "event");
//   heroEventAddWindow(event, leveleditor->toolWindow);
// glCheckError();
//   leveleditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
//   glCheckError();
//   leveleditor->toolSpriteBatch = heroSpriteBatchInit(leveleditor->toolWindow, 10, 10, leveleditor->shader);
  //leveleditor->mainSpriteBatch = heroSpriteBatchInit(leveleditor->mainWindow, 10, 10, leveleditor->shader);

  // leveleditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");
  // leveleditor->logoIndex = gameSpriteSheetGet(leveleditor->levelEditorSpriteSheet, "logo");

  heroWindowSetCurrent( leveleditor->toolWindow );

  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  // glClear(GL_COLOR_BUFFER_BIT);

  // heroSpriteBatchBegin(leveleditor->toolSpriteBatch);

  // HeroInt4 rect = gameSpriteSheetGetRect(leveleditor->levelEditorSpriteSheet, leveleditor->logoIndex);
  // heroSpriteBatchDrawTextureEx(leveleditor->toolSpriteBatch, leveleditor->levelEditorSpriteSheet->texture,
  //   (HeroInt2){0,0}, (HeroInt2){30,30}, rect, 0.0f, (HeroColor){255,255,255,255});
  // heroSpriteBatchEnd(leveleditor->toolSpriteBatch);

  // SDL_GL_SwapWindow(leveleditor->toolSdlWindow);

  //heroWindowSetCurrent( leveleditor->mainWindow );

  //glClear(GL_COLOR_BUFFER_BIT);

  // heroSpriteBatchBegin(leveleditor->mainSpriteBatch);

  // // heroSpriteBatchDrawTextureEx(leveleditor->mainSpriteBatch, leveleditor->levelEditorSpriteSheet->texture,
  // //   (HeroInt2){0,0}, (HeroInt2){30,30}, rect, 0.0f, (HeroColor){255,255,255,255});
  // heroSpriteBatchEnd(leveleditor->mainSpriteBatch);

  //SDL_GL_SwapWindow(leveleditor->mainSdlWindow);
}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  gameSpriteSheetUnload(leveleditor->levelEditorSpriteSheet);

  heroShaderUnload(leveleditor->shader);
  heroSpriteBatchDestroy(leveleditor->toolSpriteBatch);

  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventRemoveWindow(event, leveleditor->toolWindow);
  heroWindowDestroy(leveleditor->toolWindow);

  free(ptr);
}

void leveleditorclose(void** ptr)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}