#ifndef GAME_UI_H_
#define GAME_UI_H_

#include"Hero\Hero.h"


#include<stdint.h>
#include<stdbool.h>

typedef struct
{
  HeroTexture* texture;
  HeroInt2 position;
  HeroInt2 size;
  bool isHovering;
} UIButton;

UIButton* uiButtonCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size);
void uiButtonUpdate(UIButton** buttons, uint32_t number, int mouseX, int mouseY);
void uiButtonDraw(HeroSpriteBatch* spriteBatch, UIButton** buttons, uint32_t number);
void uiButtonDestory(UIButton* button);

#endif
