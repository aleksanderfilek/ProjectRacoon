#include"Game\menu.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GameMenu* menu);
static void draw(GameMenu* menu);
static void changeState(GameMenu* menu, MenuState state);

GameMenu* gameMenuInit()
{
  GameMenu* menu = (GameMenu*)malloc(sizeof(GameMenu));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  menu->sdlWindow = heroWindowGetSdlWindow(window);
  menu->input = heroCoreModuleGet(core, "input");

  menu->shader = heroShaderLoad("assets\\shaders\\shader.vert","assets\\shaders\\shader.frag");

  menu->spriteBatch = heroSpriteBatchInit(window, 10, 10, menu->shader);
  
  menu->texturesNumber = 2;
  menu->textures = (HeroTexture**)malloc(menu->texturesNumber * sizeof(HeroTexture*));
  menu->textures[0] = heroTextureLoad("assets\\sprites\\playBtn.png", 0);
  menu->textures[1] = heroTextureLoad("assets\\sprites\\quitBtn.png", 0);

  menu->uiStates[0].buttonsNumber = 2;
  menu->uiStates[0].buttons = (UIButton**)malloc(menu->uiStates[0].buttonsNumber * sizeof(UIButton*));
  menu->uiStates[0].buttons[0] = uiButtonCreate(menu->textures[0], (HeroInt2){50,50},(HeroInt2){386,64});
  menu->uiStates[0].buttons[1] = uiButtonCreate(menu->textures[1], (HeroInt2){50,150},(HeroInt2){386,64});

  changeState(menu, MENUSTATE_MAIN);

  glClearColor(0.0f,1.0f,1.0f,1.0f);
  
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

  for(int i=0; i < menu->buttonsNumber; i++)
  {
    uiButtonDestory(menu->buttons[0]);
  }

  heroShaderUnload(menu->shader);

  heroSpriteBatchDestroy(menu->spriteBatch);
  
  free(menu->buttons);
  free(menu->textures);
  free(menu);
}

static void update(GameMenu* menu)
{
  int mouseX, mouseY;
  heroInputGetMousePosition(menu->input, &mouseX, &mouseY);
  uiButtonUpdate(menu->buttons, menu->buttonsNumber, mouseX, mouseY);
  if(menu->buttons[0]->isHovering == true && heroInputMouseButtonDown(menu->input, HERO_MOUSE_LEFT) == true)
  {
    printf("play\n");
  }
  if(menu->buttons[1]->isHovering == true && heroInputMouseButtonDown(menu->input, HERO_MOUSE_LEFT) == true)
  {
    heroCoreClose(core);
  }
}

static void draw(GameMenu* menu)
{
  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(menu->spriteBatch);
  uiButtonDraw(menu->spriteBatch, menu->buttons, menu->buttonsNumber);
  heroSpriteBatchEnd(menu->spriteBatch);
  
  SDL_GL_SwapWindow(menu->sdlWindow);
}

static void changeState(GameMenu* menu, MenuState state)
{
  menu->uiState = state;
  menu->uiCurrentState =  menu->uiStates[(int)state];
}