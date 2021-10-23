#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>

extern HeroCore* core;

void close(void*ptr);

void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  leveleditor->sdlWindow = heroWindowGetSdlWindow(window);
  leveleditor->input = heroCoreModuleGet(core, "input");

  glClearColor(0.9f,0.0f,0.9f,1.0f);

  leveleditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  heroWindowSetEvent(leveleditor->toolWindow, HERO_WINDOW_CLOSE, close);
  leveleditor->toolSdlWindow = heroWindowGetSdlWindow(leveleditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, leveleditor->toolWindow);

  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  // glClear(GL_COLOR_BUFFER_BIT);

  // SDL_GL_SwapWindow(leveleditor->toolSdlWindow);
}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventRemoveWindow(event, leveleditor->toolWindow);
  heroWindowDestroy(leveleditor->toolWindow);

  free(ptr);
}

void close(void*ptr)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}