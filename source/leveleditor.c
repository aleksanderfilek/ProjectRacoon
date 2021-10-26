#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>

extern HeroCore* core;

void leveleditorclose(void**ptr);

void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  leveleditor->input = heroCoreModuleGet(core, "input");

  leveleditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  leveleditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");
  leveleditor->logoIndex = gameSpriteSheetGet(leveleditor->levelEditorSpriteSheet, "logo");

  leveleditor->mainWindow = heroCoreModuleGet(core, "window");
  leveleditor->mainSdlWindow = heroWindowGetSdlWindow(leveleditor->mainWindow);
  heroWindowSetBackgroundColor(leveleditor->mainWindow, (HeroColor){0,255,0,255});

  leveleditor->mainSpriteBatch = heroSpriteBatchInit(leveleditor->mainWindow, 10, 10, leveleditor->shader);


  leveleditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  heroWindowSetBackgroundColor(leveleditor->toolWindow, (HeroColor){255,0,0,255});
  heroWindowSetEvent(leveleditor->toolWindow, HERO_WINDOW_CLOSE, leveleditorclose);
  leveleditor->toolSdlWindow = heroWindowGetSdlWindow(leveleditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, leveleditor->toolWindow);

  leveleditor->toolSpriteBatch = heroSpriteBatchInit(leveleditor->toolWindow, 10, 10, leveleditor->shader);

  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  HeroInt4 rect = gameSpriteSheetGetRect(leveleditor->levelEditorSpriteSheet, leveleditor->logoIndex);

  heroWindowSetCurrent( leveleditor->mainWindow );

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(leveleditor->mainSpriteBatch);

  heroSpriteBatchDrawTextureEx(leveleditor->mainSpriteBatch, leveleditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,0}, (HeroInt2){30,30}, rect, 0.0f, (HeroColor){255,255,255,255});
  heroSpriteBatchEnd(leveleditor->mainSpriteBatch);

  SDL_GL_SwapWindow(leveleditor->mainSdlWindow);

  heroWindowSetCurrent( leveleditor->toolWindow );

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(leveleditor->toolSpriteBatch);

  heroSpriteBatchDrawTextureEx(leveleditor->toolSpriteBatch, leveleditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,0}, (HeroInt2){30,30}, rect, 0.0f, (HeroColor){255,255,255,255});
  heroSpriteBatchEnd(leveleditor->toolSpriteBatch);

  SDL_GL_SwapWindow(leveleditor->toolSdlWindow);

}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  gameSpriteSheetUnload(leveleditor->levelEditorSpriteSheet);

  heroShaderUnload(leveleditor->shader);
  heroSpriteBatchDestroy(leveleditor->toolSpriteBatch);
  heroSpriteBatchDestroy(leveleditor->mainSpriteBatch);

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