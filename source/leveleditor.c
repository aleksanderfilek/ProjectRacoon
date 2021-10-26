#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>

extern HeroCore* core;

void leveleditorclose(void**ptr);

void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  leveleditor->window = heroCoreModuleGet(core, "window");
  leveleditor->sdlWindow = heroWindowGetSdlWindow(leveleditor->window);
  leveleditor->input = heroCoreModuleGet(core, "input");

  leveleditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  heroWindowSetBackgroundColor(leveleditor->toolWindow, (HeroColor){255,0,0,255});
  heroWindowSetEvent(leveleditor->toolWindow, HERO_WINDOW_CLOSE, leveleditorclose);
  leveleditor->toolSdlWindow = heroWindowGetSdlWindow(leveleditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, leveleditor->toolWindow);

  heroWindowSetCurrent( leveleditor->toolWindow );

  leveleditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  leveleditor->spriteBatch = heroSpriteBatchInit(leveleditor->toolWindow, 10, 10, leveleditor->shader);

  leveleditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");
  leveleditor->logoIndex = gameSpriteSheetGet(leveleditor->levelEditorSpriteSheet, "logo");
  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(leveleditor->spriteBatch);

  HeroInt4 rect = gameSpriteSheetGetRect(leveleditor->levelEditorSpriteSheet, leveleditor->logoIndex);
  heroSpriteBatchDrawTextureEx(leveleditor->spriteBatch, leveleditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,0}, (HeroInt2){30,30}, rect, 0.0f, (HeroColor){255,255,255,255});
  heroSpriteBatchEnd(leveleditor->spriteBatch);

  SDL_GL_SwapWindow(leveleditor->toolSdlWindow);
}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  gameSpriteSheetUnload(leveleditor->levelEditorSpriteSheet);

  heroShaderUnload(leveleditor->shader);
  heroSpriteBatchDestroy(leveleditor->spriteBatch);

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