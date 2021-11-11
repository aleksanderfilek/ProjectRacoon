#include"Game/playWidget.h"
#include"Game/state.h"

#include"Hero/Hero.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

void conctructPauseWidget(GamePlay* play)
{
  play->pauseWidget = uiWidgetCreate();
  play->pauseWidget->visible = false;
  play->pauseWidget->buttonNumber = 2;
  play->pauseWidget->buttons = (UIButton**)malloc(play->pauseWidget->buttonNumber * sizeof(UIButton*));
  play->pauseWidget->buttons[0] = uiButtonCreate(play->pauseTextures[0], (HeroInt2){420,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(play->pauseWidget->buttons[0], playBtnClick, play);
  play->pauseWidget->buttons[1] = uiButtonCreate(play->pauseTextures[1], (HeroInt2){420,370},(HeroInt2){386,64});
  uiButtonSetClickFunc(play->pauseWidget->buttons[1], quitBtnClick, play);
}

void playBtnClick(void* arg)
{
  GamePlay* play = (GamePlay*)arg;
  play->paused = !play->paused;
  play->pauseWidget->visible = play->paused;
}

void quitBtnClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}