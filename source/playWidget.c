#include"Game/playWidget.h"
#include"Game/state.h"

#include"Hero/Hero.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

UIWidget* conctructPauseWidget(GamePlay* play)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 2;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(play->btnTextures[0], (HeroInt2){420,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], playBtnClick, play);
  widget->buttons[1] = uiButtonCreate(play->btnTextures[1], (HeroInt2){420,370},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], backBtnClick, play);

  return widget;
}

UIWidget* constructWinWidget(GamePlay* play)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 3;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(play->btnTextures[2], (HeroInt2){420,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], nextBtnClick, play);
  widget->buttons[1] = uiButtonCreate(play->btnTextures[2], (HeroInt2){420,370},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], replayBtnClick, play);
  widget->buttons[2] = uiButtonCreate(play->btnTextures[2], (HeroInt2){420,454},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[2], backBtnClick, play);

  widget->labelNumber = 3;
  widget->labels = (UILabel**)malloc(widget->labelNumber * sizeof(UILabel*));

  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 32);

  widget->labels[0] = uiLabelCreate("Next game", font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){420,286},(HeroInt2){386,64});
  widget->labels[1] = uiLabelCreate("Replay", font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){420,370},(HeroInt2){386,64});
  widget->labels[2] = uiLabelCreate("Back to menu", font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){420,454},(HeroInt2){386,64});

  heroFontUnload(font);

  return widget;
}

UIWidget* constructFailedWidget(GamePlay* play)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 2;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(play->btnTextures[2], (HeroInt2){420,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], replayBtnClick, play);
  widget->buttons[1] = uiButtonCreate(play->btnTextures[2], (HeroInt2){420,370},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], backBtnClick, play);

  widget->labelNumber = 2;
  widget->labels = (UILabel**)malloc(widget->labelNumber * sizeof(UILabel*));

  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 32);

  widget->labels[0] = uiLabelCreate("Replay", font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){420,286},(HeroInt2){386,64});
  widget->labels[1] = uiLabelCreate("Back to menu", font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){420,370},(HeroInt2){386,64});  

  heroFontUnload(font);

  return widget;
}

void playBtnClick(void* arg)
{
  GamePlay* play = (GamePlay*)arg;
  play->paused = false;
  play->widgets[0]->visible = false;
  play->currentWidget = PLAY_STATE_GAME;
}

void backBtnClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}

void nextBtnClick(void* arg)
{
  GamePlay* play = (GamePlay*)arg;
}

void replayBtnClick(void* arg)
{
  GamePlay* play = (GamePlay*)arg;
  play->currentWidget = PLAY_STATE_GAME;
  gamePlayRestart(play);
}