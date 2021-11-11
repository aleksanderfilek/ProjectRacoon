#ifndef MENU_WIDGET_H_
#define MENU_WIDGET_H_

#include"Game/menu.h"

void widgetConstructMainMenu(GameMenu* menu);
void widgetConstructPlayMenu(GameMenu* menu);

void playClick();
void quitClick();
void backToMenuClick();
void gameClick();



#endif