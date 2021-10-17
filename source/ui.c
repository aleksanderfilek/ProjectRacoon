#include"Game\ui.h"

#include<stdlib.h>

UIButton* uiButtonCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size)
{
  UIButton* button = (UIButton*)malloc(sizeof(UIButton));

  button->texture = texture;
  button->position = position;
  button->size = size;
  button->isHovering = false;

  return button;
}

void uiButtonUpdate(UIButton** buttons, uint32_t number, int mouseX, int mouseY)
{
  for(int i = 0; i < number; i++)
  {
    UIButton* button = buttons[i];
    
    if(button->position.x <= mouseX &&  mouseX <= button->position.x + button->size.x && 
      button->position.y <= mouseY &&  mouseY <= button->position.y + button->size.y)
      {
        button->isHovering = true;
      }
      else 
      {
        button->isHovering = false;
      }
  }
}

void uiButtonDraw(HeroSpriteBatch* spriteBatch, UIButton** buttons, uint32_t number)
{
  for(int i = 0; i < number; i++)
  {
    UIButton* button = buttons[i];
    heroSpriteBatchDrawTexture(spriteBatch, button->texture, button->position, button->size);
  }
}

void uiButtonDestory(UIButton* button)
{
  free(button);
}