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

typedef enum
{
  UIALLIGMENT_TOPLEFT,
  UIALLIGMENT_TOP,
  UIALLIGMENT_TOPRIGHT,
  UIALLIGMENT_LEFT,
  UIALLIGMENT_CENTER,
  UIALLIGMENT_RIGHT,
  UIALLIGMENT_BOTTOMLEFT,
  UIALLIGMENT_BOTTOM,
  UIALLIGMENT_BOTTOMRIGHT
} UIAlligment;

typedef struct
{
  HeroTexture* texture;
  HeroInt2 position;
  HeroInt2 size;
  HeroInt4 rect;
  HeroColor color;
} UILabel;

UILabel* uiLabelCreate(const char* text, HeroFont* font, HeroColor color, UIAlligment alligment, HeroInt2 position, HeroInt2 size);
void uiLabelDraw(UILabel** labels, uint32_t number, HeroSpriteBatch* spriteBatch);
void uiLabelDestroy(UILabel* label);

typedef struct
{
  UIButton** buttons;
  uint32_t buttonNumber;
  UIImage** images;
  uint32_t imageNumber;
  UILabel** labels;
  uint32_t labelNumber;
} UIWidget;

UIWidget* uiWidgetCreate();
void uiWidgetUpdate(UIWidget* widget, HeroInput* input);
void uiWidgetDraw(UIWidget* widget, HeroSpriteBatch* spriteBatch);
void uiWidgetDestroy(UIWidget* widget);

#endif
