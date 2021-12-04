#include"Game/ui.h"

#include<stdlib.h>
#include<string.h>

#define Min(a,b) (a<b)?a:b;
#define Max(a,b) (a>b)?a:b;

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
  bool leftPressed = heroInputMouseButtonPressed(input, HERO_MOUSE_LEFT);
  for(int i = 0; i < number; i++)
  {
    UIButton* button = buttons[i];
    
    if(button->position.x <= mouseX &&  mouseX <= button->position.x + button->size.x && 
      button->position.y <= mouseY &&  mouseY <= button->position.y + button->size.y)
    {
      button->state = UIBUTTONSTATE_HOVER;

      if(leftClick == true)
      {
        button->click(button->arg);
      }
      if(leftPressed == true)
      {
        button->state = UIBUTTONSTATE_CLICK;
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
    heroSpriteBatchDrawTextureEx(spriteBatch, button->texture, button->position, button->size, button->rect[(int)button->state], 0.0f, color);
  }
}

void uiButtonDestory(UIButton* button)
{
  free(button);
}

UIImage* uiImageCreate(HeroTexture* texture, HeroInt2 position, HeroInt2 size)
{
  UIImage* image = (UIImage*)malloc(sizeof(UIImage));

  image->texture = texture;
  image->position = position;
  image->size = size;
  image->rect = (HeroInt4){0,0,size.x,size.y};
  image->visible = true;
  
  return image;
}

void uiImageSetRect(UIImage* image, HeroInt4 rect)
{
  image->rect = rect;
}

void uiImageDraw(UIImage** images, uint32_t number, HeroSpriteBatch* spriteBatch)
{
  HeroColor color = {255,255,255,255};

  for(int i = 0; i < number; i++)
  {
    UIImage* image = images[i];
    if(image->visible == false)
    {
      continue;
    }
    heroSpriteBatchDrawTextureEx(spriteBatch, image->texture, image->position, image->size, image->rect, 0.0f, color);
  }
}

void uiImageDestroy(UIImage* image)
{
  free(image);
}

UILabel* uiLabelCreate(const char* text, HeroFont* font, HeroColor color, UIAlligment alligment, HeroInt2 position, HeroInt2 size)
{
  UILabel* label = (UILabel*)malloc(sizeof(UILabel));
  label->visible = true;
  label->texture = heroTextureFromText(text, &color, font, 0);
  HeroInt2 textureSize = heroTextureGetSize(label->texture);

  switch(alligment)
  {
    case UIALLIGMENT_TOPLEFT:
      label->position = position;
    break;
    case UIALLIGMENT_TOP:
      label->position.x = position.x + (size.x - textureSize.x)/2;
      label->position.y = position.y;
    break;
    case UIALLIGMENT_TOPRIGHT:
      label->position.x = position.x + size.x - textureSize.x;
      label->position.y = position.y;
    break;
    case UIALLIGMENT_LEFT:
      label->position.x = position.x;
      label->position.y = position.y + (size.y - textureSize.y)/2;
    break;
    case UIALLIGMENT_CENTER:
      label->position.x = position.x + (size.x - textureSize.x)/2;
      label->position.y = position.y + (size.y - textureSize.y)/2;
    break;
    case UIALLIGMENT_RIGHT:
      label->position.x = position.x + size.x - textureSize.x;
      label->position.y = position.y + (size.y - textureSize.y)/2;
    break;
    case UIALLIGMENT_BOTTOMLEFT:
      label->position.x = position.x;
      label->position.y = position.y + size.y - textureSize.y;
    break;
    case UIALLIGMENT_BOTTOM:
      label->position.x = position.x + (size.x - textureSize.x)/2;
      label->position.y = position.y + size.y - textureSize.y;
    break;
    case UIALLIGMENT_BOTTOMRIGHT:
      label->position.x = position.x + size.x - textureSize.x;
      label->position.y = position.y + size.y - textureSize.y;
    break;
  }

  int lpsx = label->position.x + textureSize.x;
  int lpsy = label->position.y + textureSize.y;
  int psx = position.x + size.x;
  int psy = position.y + size.y;

  label->rect.x = (label->position.x >= position.x)? 0 : position.x - label->position.x;
  label->rect.y = (label->position.y >= position.y)? 0 : position.y - label->position.y;
  label->rect.z = (lpsx <= psx)? textureSize.x : position.x + size.x  - label->position.x;
  label->rect.w = (lpsy <= psy)? textureSize.y : position.y + size.y  - label->position.y;

  label->size.x = Min(lpsx, psx);
  label->size.x -= (label->position.x >= position.x)?label->position.x:position.x;
  label->size.y = Min(lpsy, psy);
  label->size.y -= (label->position.y >= position.y)?label->position.y:position.y;

  label->position.x = Max(label->position.x, position.x);
  label->position.y = Max(label->position.y, position.y);

  return label;
}

void uiLabelDraw(UILabel** labels, uint32_t number, HeroSpriteBatch* spriteBatch)
{
  HeroColor color = {255,255,255,255};

  for(int i = 0; i < number; i++)
  {
    UILabel* label = labels[i];
    if(label->visible == false)
    {
      continue;
    }

    heroSpriteBatchDrawTextureEx(spriteBatch, label->texture, label->position, label->size, label->rect, 0.0f, color);
  }
}

void uiLabelDestroy(UILabel* label)
{
  heroTextureUnload(label->texture);
  free(label);
}


UIWidget* uiWidgetCreate()
{
  UIWidget* widget = (UIWidget*)malloc(sizeof(UIWidget));
  memset(widget, 0, sizeof(UIWidget));
  widget->visible = true;
  return widget;
}

void uiWidgetUpdate(UIWidget* widget, HeroInput* input)
{
  if(widget->visible == false)
  {
    return;
  }
  uiButtonUpdate(widget->buttons, widget->buttonNumber, input);
}

void uiWidgetDraw(UIWidget* widget, HeroSpriteBatch* spriteBatch)
{
  if(widget->visible == false)
  {
    return;
  }
  uiImageDraw(widget->images, widget->imageNumber, spriteBatch);
  uiButtonDraw(spriteBatch, widget->buttons, widget->buttonNumber);
  uiLabelDraw(widget->labels, widget->labelNumber, spriteBatch);
}

void uiWidgetDestroy(UIWidget* widget)
{
  for(int i = 0; i < widget->buttonNumber; i++)
  {
    uiButtonDestory(widget->buttons[i]);
  }
  free(widget->buttons);
  for(int i = 0; i < widget->imageNumber; i++)
  {
    uiImageDestroy(widget->images[i]);
  }
  free(widget->images);
  for(int i = 0; i < widget->labelNumber; i++)
  {
    uiLabelDestroy(widget->labels[i]);
  }
  free(widget->labels);
  free(widget);
}

void uiWidgetButtonsNumber(UIWidget* widget, uint32_t number)
{
  widget->buttonNumber = number;
  widget->buttons = (UIButton**)malloc(number * sizeof(UIButton*));
}

void uiWidgetImagesNumber(UIWidget* widget, uint32_t number)
{
  widget->imageNumber = number;
  widget->images = (UIImage**)malloc(number * sizeof(UIImage*));
}

void uiWidgetLabelsNumber(UIWidget* widget, uint32_t number)
{
  widget->labelNumber = number;
  widget->labels = (UILabel**)malloc(number * sizeof(UILabel*));
}