#ifndef GAME_MENU_H_
#define GAME_MENU_H_

#include"Hero\Hero.h"

#include"Game\ui.h"

#include<stdint.h>

typedef enum
{
  MENUSTATE_MAIN = 0,
  MENUSTATE_LEVELS = 1,
  MENUSTATE_COUNT = 2
} MenuState;

typedef struct
{
  SDL_Window* sdlWindow;
  HeroInput* input;

  HeroSpriteBatch* spriteBatch;

  // assets
  HeroTexture** textures;
  uint32_t texturesNumber;
  HeroShader* shader;

  MenuState uiState;
  UIWidget* currentWidget;
  UIWidget* widgets[2];

} GameMenu;

GameMenu* gameMenuInit();
void gameMenuUpdate(void* ptr);
void gameMenuDestroy(void* ptr);

#endif