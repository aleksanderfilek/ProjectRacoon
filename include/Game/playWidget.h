#ifndef PLAY_WIDGET_H_
#define PLAY_WIDGET_H_

#include"Game/play.h"

typedef enum
{
    PLAY_STATE_GAME = 0,
    PLAY_STATE_PAUSE,
    PLAY_STATE_FAILED,
    PLAY_STATE_WIN
} PlayStateEnum;

void conctructPauseWidget(GamePlay* play);
void playBtnClick(void* arg);
void quitBtnClick(void* arg);



#endif