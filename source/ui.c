#include"Game\ui.h"

#include<stdlib.h>
#include<string.h>

UIButton* uiButtonCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size)
{
  UIButton* button = (UIButton*)malloc(sizeof(UIButton));

  button->texture = texture;
  button->position = position;
  button->size = size;
  button->state = UIBUTTONSTATE_NORMAL;
  for(int i = 0; i < UIBUTTONSTATE_COUNT; i++)
  {
    button->rect[i] = (HeroInt4){0,0,size.x,size.y};
  }

  return button;
}

void uiButtonSetStateRect(UIButton* button, UIButtonState state, HeroInt4 rect)
{
  button->rect[(int)state] = rect;
}

void uiButtonSetClickFunc(UIButton* button, void (*click)(void*), void* arg)
{
  button->click = click;
  button->arg = arg;
}

void uiButtonUpdate(UIButton** buttons, uint32_t number, HeroInput* input)
{
  int mouseX, mouseY;
  heroInputGetMousePosition(input, &mouseX, &mouseY);
  bool leftClick = heroInputMouseButtonDown(input, HERO_MOUSE_LEFT);

  for(int i = 0; i < number; i++)
  {
    UIButton* button = buttons[i];
    
    if(button->position.x <= mouseX &&  mouseX <= button->position.x + button->size.x && 
      button->position.y <= mouseY &&  mouseY <= button->position.y + button->size.y)
    {
      button->state = UIBUTTONSTATE_HOVER;

      if(leftClick == true)
      {
        button->state = UIBUTTONSTATE_CLICK;
        button->click(button->arg);
      }

      continue;
    }
    button->state = UIBUTTONSTATE_NORMAL;
  }
}

void uiButtonDraw(HeroSpriteBatch* spriteBatch, UIButton** buttons, uint32_t number)
{
  HeroColor color = {255,255,255,255};

  for(int i = 0; i < number; i++)
  {
    UIButton* button = buttons[i];
    heroSpriteBatchDrawTexture(spriteBatch, button->texture, button->position, button->size);
    heroSpriteBatchDrawTextureEx(spriteBatch, button->texture, button->position, button->size, button->rect[button->state], 0.0f, color);
  }
}

void uiButtonDestory(UIButton* button)
{
  free(button);
}

UIWidget* uiWidgetCreate()
{
  UIWidget* widget = (UIWidget*)malloc(sizeof(UIWidget));
  memset(widget, 0, sizeof(UIWidget));
  return widget;
}

void uiWidgetUpdate(UIWidget* widget, HeroInput* input)
{
  uiButtonUpdate(widget->buttons, widget->buttonNumber, input);
}

void uiWidgetDraw(UIWidget* widget, HeroSpriteBatch* spriteBatch)
{
  uiButtonDraw(spriteBatch, widget->buttons, widget->buttonNumber);
}

void uiWidgetDestroy(UIWidget* widget)
{
  for(int i = 0; i < widget->buttonNumber; i++)
  {
    uiButtonDestory(widget->buttons[i]);
  }
  free(widget->buttons);
  free(widget);
}