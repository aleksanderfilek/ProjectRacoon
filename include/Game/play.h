#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include"Hero/Hero.h"

#include<stdbool.h>

typedef struct
{
  SDL_Window* sdlWindow;
  HeroInput* input;
} GamePlay;

void* gamePlayInit();
void gamePlayUpdate(void* ptr);
void gamePlayDestroy(void* ptr);

#endif