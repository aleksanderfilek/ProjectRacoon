#ifndef MENU_WIDGET_H_
#define MENU_WIDGET_H_

#include"Game/menu.h"

void widgetMenuMainConstruct(Menu* menu, HeroFont* font);
void widgetMenuLevelsConstruct(Menu* menu, HeroFont* font);
void widgetMenuSettingsConstruct(Menu* menu, HeroFont* font);

void optionSwitch(Menu* menu);
void optionClick(Menu* menu);

void exitClick(void* arg);

#endif