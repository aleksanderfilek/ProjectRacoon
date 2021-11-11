#include"Game/menu.h"
#include"Game/state.h"
#include"Game/file.h"
#include"Game/sharedata.h"

#include"Game/menuWidget.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GameMenu* menu);
static void draw(GameMenu* menu);

void* gameMenuInit()
{
  GameMenu* menu = (GameMenu*)malloc(sizeof(GameMenu));

  HeroWindow* window = heroCoreModuleGet(core, "window");
  menu->sdlWindow = heroWindowGetSdlWindow(window);
  menu->input = heroCoreModuleGet(core, "input");

  heroWindowSetCurrentContext(window);

  menu->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");

  menu->spriteBatch = heroSpriteBatchInit(window, 10, 10, menu->shader);
  
  menu->texturesNumber = 5;
  menu->textures = (HeroTexture**)malloc(menu->texturesNumber * sizeof(HeroTexture*));
  menu->textures[0] = heroTextureLoad("assets/sprites/playBtn.png", 0);
  menu->textures[1] = heroTextureLoad("assets/sprites/quitBtn.png", 0);
  menu->textures[2] = heroTextureLoad("assets/sprites/backBtn.png", 0);
  menu->textures[3] = heroTextureLoad("assets/sprites/background.png", 0);
  menu->textures[4] = heroTextureLoad("assets/sprites/levelBtn.png", 0);


  menu->levelsPaths = gameFileGetInDirectory("assets/levels", &menu->levelsNumber);

  widgetConstructMainMenu(menu);
  widgetConstructPlayMenu(menu);

  gameChangeState(menu, MENUSTATE_MAIN);

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

  DEBUG_CODE(
    if(heroInputKeyPressed(menu->input, HERO_KEYCODE_LCTRL) && 
      heroInputKeyPressed(menu->input, HERO_KEYCODE_L)){
        GameState* state = heroCoreModuleGet(core, "state");
        gameStateChange(state, GAMESTATE_LEVELEDITOR);
      }
  )
}

void gameMenuDestroy(void* ptr)
{
  GameMenu* menu = (GameMenu*)ptr;

  char** levelsItr = menu->levelsPaths;
  while(*levelsItr)
  {
    free(*levelsItr);
    levelsItr++;
  }
  free(menu->levelsPaths);

  for(int i=0; i < menu->texturesNumber; i++)
  {
    heroTextureUnload(menu->textures[i]);
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

void gameChangeState(GameMenu* menu, MenuState state)
{
  menu->uiState = state;
  menu->currentWidget =  menu->widgets[(int)state];
}
