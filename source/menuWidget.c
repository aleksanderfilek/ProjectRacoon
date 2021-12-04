#include"Game/menuWidget.h"
#include"Game/state.h"
#include"Game/file.h"
#include"Game/sharedata.h"

#include"Hero/Hero.h"

#include<stdlib.h>
#include<stdio.h>
#include"Game/ui.h"

extern HeroCore* core;

void widgetConstructMainMenu(GameMenu* menu)
{
  menu->widgets[0] = uiWidgetCreate();

  UIWidget* widget = menu->widgets[0];

  uiWidgetButtonsNumber(widget, 2);
  widget->buttons[0] = uiButtonCreate(menu->textures[0], (HeroInt2){447,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], playClick, menu);
  widget->buttons[1] = uiButtonCreate(menu->textures[1], (HeroInt2){447,375},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], quitClick, core);

  uiWidgetLabelsNumber(widget, 1);
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 16);
  widget->labels[0] = uiLabelCreate("Created by Aleksander Filek", font, (HeroColor){0,0,0,0},
    UIALLIGMENT_BOTTOMRIGHT, (HeroInt2){0,0},(HeroInt2){1280,720});
  heroFontUnload(font);
}

void widgetConstructPlayMenu(GameMenu* menu)
{
  menu->widgets[1] = uiWidgetCreate();
  UIWidget* widget = menu->widgets[1];

  uiWidgetButtonsNumber(widget, 5);
  widget->buttons[0] = uiButtonCreate(menu->textures[2], (HeroInt2){447,50},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], backToMenuClick, menu);

  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 32);

  uiWidgetLabelsNumber(widget, 4);

  int y = 150;
  for(int i = 0; i < 4; i++)
  {
    widget->buttons[i+1] = uiButtonCreate(menu->textures[4], (HeroInt2){447,y},(HeroInt2){386,64});
    uiButtonSetClickFunc(widget->buttons[i+1], gameClick, menu->levelsPaths[i]);

    char* name = gameFileGetName(menu->levelsPaths[i]);
    widget->labels[i] = uiLabelCreate(name, font, (HeroColor){255,255,255,255},
      UIALLIGMENT_CENTER, (HeroInt2){447,y},(HeroInt2){386,64});
    free(name);
    y+= 84;
  }

  heroFontUnload(font);
}

void playClick(void* arg)
{
  gameChangeState((GameMenu*)arg, MENUSTATE_LEVELS);
}

void quitClick(void* arg)
{
  heroCoreClose((HeroCore*)arg);
}

void backToMenuClick(void* arg)
{
  gameChangeState((GameMenu*)arg, MENUSTATE_MAIN);
}

void gameClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_PLAY);

  char* path = strdup((char*)arg);
  GameSharedDataSystem* sharedata = heroCoreModuleGet(core, "data");
  gameSharedDataAdd(sharedata, "level", path, NULL);
}