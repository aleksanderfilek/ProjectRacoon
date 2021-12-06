#ifndef GAME_MENU_H_
#define GAME_MENU_H_

#include"Hero/Hero.h"

#include"Game/ui.h"
#include"Game/levels.h"

typedef enum MenuState
{
  MENUSTATE_MAIN = 0,
  MENUSTATE_LEVELS,
  MENUSTATE_SETTINGS,
  MENUSTATE_COUNT
} MenuState;

typedef struct Menu
{
  HeroWindow* window;
  HeroInput* input;

  HeroSpriteBatch* spriteBatch;

  MenuState currentState;
  UIWidget* stateWidgets[MENUSTATE_COUNT];
  int activeOptionIndex;

} Menu;

void* menuInit();
void menuUpdate(void* ptr);
void menuDestroy(void* ptr);
void menuDraw(Menu* menu);

#endif