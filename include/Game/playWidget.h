#ifndef PLAY_WIDGET_H_
#define PLAY_WIDGET_H_

#include"Game/play.h"

UIWidget* conctructPauseWidget(GamePlay* play);
UIWidget* constructWinWidget(GamePlay* play);
UIWidget* constructFailedWidget(GamePlay* play);

void playBtnClick(void* arg);
void backBtnClick(void* arg);
void nextBtnClick(void* arg);
void replayBtnClick(void* arg);

#endif