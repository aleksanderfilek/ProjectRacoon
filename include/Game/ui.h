#ifndef GAME_UI_H_
#define GAME_UI_H_

#include"Hero\Hero.h"

#include<stdint.h>
#include<stdbool.h>

typedef enum
{
  UIBUTTONSTATE_NORMAL = 0,
  UIBUTTONSTATE_HOVER = 1,
  UIBUTTONSTATE_CLICK = 2,
  UIBUTTONSTATE_COUNT = 3
} UIButtonState;

typedef struct
{
  HeroTexture* texture;
  HeroInt2 position;
  HeroInt2 size;
  UIButtonState state;
  HeroInt4 rect[UIBUTTONSTATE_COUNT];
  void (*click)(void*);
  void* arg;
} UIButton;

UIButton* uiButtonCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size);
void uiButtonSetStateRect(UIButton* button, UIButtonState state, HeroInt4 rect);
void uiButtonSetClickFunc(UIButton* button, void (*click)(void*), void* arg);
void uiButtonUpdate(UIButton** buttons, uint32_t number, HeroInput* input);
void uiButtonDraw(HeroSpriteBatch* spriteBatch, UIButton** buttons, uint32_t number);
void uiButtonDestory(UIButton* button);

typedef struct
{
  HeroTexture* texture;
  HeroInt2 position;
  HeroInt2 size;
  HeroInt4 rect;
} UIImage;

UIImage* uiImageCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size);
void uiImageSetRect(UIImage* image, HeroInt4 rect);
void uiImageDraw(UIImage** images, uint32_t number, HeroSpriteBatch* spriteBatch);
void uiImageDestroy(UIImage* image);

typedef struct
{
  UIButton** buttons;
  uint32_t buttonNumber;
} UIWidget;

UIWidget* uiWidgetCreate();
void uiWidgetUpdate(UIWidget* widget, HeroInput* input);
void uiWidgetDraw(UIWidget* widget, HeroSpriteBatch* spriteBatch);
void uiWidgetDestroy(UIWidget* widget);

#endif
