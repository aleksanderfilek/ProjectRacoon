#include"Game\menu.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

GameMenu* gameMenuInit()
{
  GameMenu* menu = (GameMenu*)malloc(sizeof(GameMenu));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  menu->sdlWindow = heroWindowGetSdlWindow(window);

  menu->shader = heroShaderLoad("assets\\shaders\\shader.vert","assets\\shaders\\shader.frag");
  if(menu->shader==NULL)
  {
    printf("Could not create shader\n");
    exit(-1);
  }
  menu->spriteBatch = heroSpriteBatchInit(10, 10, menu->shader);

  menu->textures = (HeroTexture**)malloc(sizeof(HeroTexture*));
  menu->textures[0] = heroTextureLoad("assets\\sprites\\playBtn.png", 0);
  if(menu->textures[0]==NULL)
  {
    printf("Could not create texture\n");
    exit(-1);
  }

  menu->buttons = (UIButton**)malloc(sizeof(UIButton*));
  menu->buttons[0] = uiButtonCreate(menu->textures[0], (HeroInt2){50,50},(HeroInt2){386,64});
  
  glClearColor(0.0f,1.0f,1.0f,1.0f);
  
  return menu;
}

void gameMenuUpdate(void* ptr)
{
  GameMenu* menu = (GameMenu*)ptr;

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(menu->spriteBatch);
  uiButtonDraw(menu->spriteBatch, menu->buttons, 1);
  heroSpriteBatchEnd(menu->spriteBatch);
  
  SDL_GL_SwapWindow(menu->sdlWindow);
}

void gameMenuDestroy(void* ptr)
{
  GameMenu* menu = (GameMenu*)ptr;

  uiButtonDestory(menu->buttons[0]);

  heroShaderUnload(menu->shader);

  //heroTextureUnload(menu->textures[0]);

  heroSpriteBatchDestroy(menu->spriteBatch);
  
  free(menu->buttons);
  free(menu->textures);
  free(menu);
}