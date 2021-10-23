
#include"Game/play.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GamePlay* gamePlay);
static void draw(GamePlay* gamePlay);

void* gamePlayInit()
{
  GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  play->sdlWindow = heroWindowGetSdlWindow(window);
  play->input = heroCoreModuleGet(core, "input");

  glClearColor(0.9f,0.9f,0.9f,1.0f);

  return play;
}

void gamePlayUpdate(void* ptr)
{
  GamePlay* play = (GamePlay*)ptr;

  update(play);

  draw(play);
}

void gamePlayDestroy(void* ptr)
{
  free(ptr);
}

static void update(GamePlay* gamePlay)
{

}

static void draw(GamePlay* gamePlay)
{
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(gamePlay->sdlWindow);
}