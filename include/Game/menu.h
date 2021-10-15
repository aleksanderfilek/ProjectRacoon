#ifndef GAME_MENU_H_
#define GAME_MENU_H_

#include"Hero\Hero.h"

#include"Game\ui.h"


typedef struct
{
  HeroSpriteBatch* spriteBatch;
  HeroTexture** textures;
  HeroShader* shader;
  UIButton** buttons;
  SDL_Window* sdlWindow;
} GameMenu;

GameMenu* gameMenuInit();
void gameMenuUpdate(void* ptr);
void gameMenuDestroy(void* ptr);

#endif