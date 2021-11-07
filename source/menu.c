#include"Game/state.h"
#include"Game/file.h"
#include"Game/sharedata.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GameMenu* menu);
static void draw(GameMenu* menu);
static void changeState(GameMenu* menu, MenuState state);
static void playClick();
static void quitClick();
static void backToMenuClick();
static void gameClick();
static UIWidget* widgetConstructMainMenu(GameMenu* menu);
static UIWidget* widgetConstructPlayMenu(GameMenu* menu);

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

static void gameClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_PLAY);

  GameSharedDataSystem* sharedata = heroCoreModuleGet(core, "data");
  gameSharedDataAdd(sharedata, "level", arg, NULL);
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
  widget->labelNumber = 1;
  widget->labels = (UILabel**)malloc(widget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 16);
  widget->labels[0] = uiLabelCreate("Created by Aleksander Filek", font, (HeroColor){0,0,0,0},
    UIALLIGMENT_BOTTOMRIGHT, (HeroInt2){0,0},(HeroInt2){1280,720});
  heroFontUnload(font);

  return widget;
}

static UIWidget* widgetConstructPlayMenu(GameMenu* menu)
{
  UIWidget* widget = uiWidgetCreate();

  widget->buttonNumber = 2;
  widget->buttons = (UIButton**)malloc(widget->buttonNumber * sizeof(UIButton*));
  widget->buttons[0] = uiButtonCreate(menu->textures[2], (HeroInt2){50,50},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[0], backToMenuClick, menu);
  widget->buttons[1] = uiButtonCreate(menu->textures[4], (HeroInt2){50,150},(HeroInt2){386,64});
  uiButtonSetClickFunc(widget->buttons[1], gameClick, menu->levelsPaths[0]);

  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 32);

  widget->labelNumber = 1;
  widget->labels = (UILabel**)malloc(widget->labelNumber * sizeof(UILabel*));

  char* name = gameFileGetName(menu->levelsPaths[0]);
  widget->labels[0] = uiLabelCreate(name, font, (HeroColor){255,255,255,255},
    UIALLIGMENT_CENTER, (HeroInt2){50,150},(HeroInt2){386,64});
  free(name);
  heroFontUnload(font);

  return widget;
}