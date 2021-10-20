#include"Game\menu.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GameMenu* menu);
static void draw(GameMenu* menu);
static void changeState(GameMenu* menu, MenuState state);
static void playClick();
static void quitClick();
static void backToMenuClick();
static UIWidget* widgetConstructMainMenu(GameMenu* menu);
static UIWidget* widgetConstructPlayMenu(GameMenu* menu);

GameMenu* gameMenuInit()
{
  GameMenu* menu = (GameMenu*)malloc(sizeof(GameMenu));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  menu->sdlWindow = heroWindowGetSdlWindow(window);
  menu->input = heroCoreModuleGet(core, "input");

  menu->shader = heroShaderLoad("assets\\shaders\\shader.vert","assets\\shaders\\shader.frag");

  menu->spriteBatch = heroSpriteBatchInit(window, 10, 10, menu->shader);
  
  menu->texturesNumber = 4;
  menu->textures = (HeroTexture**)malloc(menu->texturesNumber * sizeof(HeroTexture*));
  menu->textures[0] = heroTextureLoad("assets\\sprites\\playBtn.png", 0);
  menu->textures[1] = heroTextureLoad("assets\\sprites\\quitBtn.png", 0);
  menu->textures[2] = heroTextureLoad("assets\\sprites\\backBtn.png", 0);
  menu->textures[3] = heroTextureLoad("assets\\sprites\\background.png", 0);

  menu->widgets[0] = widgetConstructMainMenu(menu);
  menu->widgets[1] = widgetConstructPlayMenu(menu);

  changeState(menu, MENUSTATE_MAIN);

  glClearColor(1.0f,1.0f,1.0f,1.0f);

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  return menu;
}

void gameMenuUpdate(void* ptr)
{
  GameMenu* menu = (GameMenu*)ptr;

  update(menu);

  draw(menu);
}

void gameMenuDestroy(void* ptr)
{
  GameMenu* menu = (GameMenu*)ptr;

  for(int i=0; i < menu->texturesNumber; i++)
  {
    heroTextureUnload(menu->textures[0]);
  }

  for(int i=0; i < 2; i++)
  {
    uiWidgetDestroy(menu->widgets[i]);
  }

  heroShaderUnload(menu->shader);

  heroSpriteBatchDestroy(menu->spriteBatch);
  
  free(menu->textures);
  free(menu);
}

static void update(GameMenu* menu)
{
  uiWidgetUpdate(menu->currentWidget, menu->input);

}

static void draw(GameMenu* menu)
{
  glClear(GL_COLOR_BUFFER_BIT);


  heroSpriteBatchBegin(menu->spriteBatch);
  uiWidgetDraw(menu->currentWidget, menu->spriteBatch);
  heroSpriteBatchEnd(menu->spriteBatch);
  
  SDL_GL_SwapWindow(menu->sdlWindow);
}

static void changeState(GameMenu* menu, MenuState state)
{
  menu->uiState = state;
  menu->currentWidget =  menu->widgets[(int)state];
}

static void playClick(void* arg)
{
  changeState((GameMenu*)arg, MENUSTATE_LEVELS);
}

static void quitClick(void* arg)
{
  heroCoreClose((HeroCore*)arg);
}

static void backToMenuClick(void* arg)
{
  changeState((GameMenu*)arg, MENUSTATE_MAIN);
}

static UIWidget* widgetConstructMainMenu(GameMenu* menu)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 2;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(menu->textures[0], (HeroInt2){50,50},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], playClick, menu);
  widget->buttons[1] = uiButtonCreate(menu->textures[1], (HeroInt2){50,150},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], quitClick, core);
  widget->imageNumber = 1;
  widget->images = (UIImage**)malloc(widget->imageNumber * sizeof(UIImage*));
  widget->images[0] = uiImageCreate(menu->textures[3], (HeroInt2){50,300},(HeroInt2){100,15});
  widget->labelNumber = 1;
  widget->labels = (UILabel**)malloc(widget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets\\fonts\\arial.ttf", 32);
  widget->labels[0] = uiLabelCreate("Hello, World!", font, (HeroColor){0,0,0,0},
                                   UIALLIGMENT_TOP, (HeroInt2){50,300},(HeroInt2){100,15});
  heroFontUnload(font);

  return widget;
}

static UIWidget* widgetConstructPlayMenu(GameMenu* menu)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 1;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(menu->textures[2], (HeroInt2){50,50},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], backToMenuClick, menu);

  return widget;
}