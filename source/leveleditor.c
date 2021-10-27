#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>

extern HeroCore* core;

static void leveleditorclose(void**ptr);
static void constructToolWidget(GameLevelEditor* levelEditor);
static void update(GameLevelEditor* levelEditor);
static void draw(GameLevelEditor* levelEditor);


void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  leveleditor->input = heroCoreModuleGet(core, "input");

  leveleditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  leveleditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");

  leveleditor->mainWindow = heroCoreModuleGet(core, "window");
  leveleditor->mainSdlWindow = heroWindowGetSdlWindow(leveleditor->mainWindow);
  heroWindowSetBackgroundColor(leveleditor->mainWindow, (HeroColor){0,255,0,255});

  leveleditor->mainSpriteBatch = heroSpriteBatchInit(leveleditor->mainWindow, 10, 10, leveleditor->shader);


  leveleditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  heroWindowSetBackgroundColor(leveleditor->toolWindow, (HeroColor){0x1E,0x1E,0x1E,0xFF});
  heroWindowSetEvent(leveleditor->toolWindow, HERO_WINDOW_CLOSE, leveleditorclose);
  leveleditor->toolSdlWindow = heroWindowGetSdlWindow(leveleditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, leveleditor->toolWindow);

  leveleditor->toolSpriteBatch = heroSpriteBatchInit(leveleditor->toolWindow, 10, 10, leveleditor->shader);

  constructToolWidget(leveleditor);

  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)ptr;

  update(levelEditor);

  draw(levelEditor);

}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)ptr;

  gameSpriteSheetUnload(leveleditor->levelEditorSpriteSheet);

  heroShaderUnload(leveleditor->shader);
  heroSpriteBatchDestroy(leveleditor->toolSpriteBatch);
  heroSpriteBatchDestroy(leveleditor->mainSpriteBatch);

  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventRemoveWindow(event, leveleditor->toolWindow);
  heroWindowDestroy(leveleditor->toolWindow);

  free(ptr);
}

static void update(GameLevelEditor* levelEditor)
{
  if(heroWindowIsFocused(levelEditor->toolWindow) == true)
  {
    uiWidgetUpdate(levelEditor->toolWidget, levelEditor->input);
  }
}

static void draw(GameLevelEditor* levelEditor)
{
  if(heroWindowIsFocused(levelEditor->toolWindow) == true)
  {
    heroWindowSetCurrentContext( levelEditor->toolWindow );

    glClear(GL_COLOR_BUFFER_BIT);

    heroSpriteBatchBegin(levelEditor->toolSpriteBatch);

    uiWidgetDraw(levelEditor->toolWidget, levelEditor->toolSpriteBatch);

    heroSpriteBatchEnd(levelEditor->toolSpriteBatch);

    SDL_GL_SwapWindow(levelEditor->toolSdlWindow);
  }
  else if(heroWindowIsFocused(levelEditor->mainWindow) == true)
  {
    heroWindowSetCurrentContext( levelEditor->mainWindow );

    glClear(GL_COLOR_BUFFER_BIT);

    heroSpriteBatchBegin(levelEditor->mainSpriteBatch);


    heroSpriteBatchEnd(levelEditor->mainSpriteBatch);

    SDL_GL_SwapWindow(levelEditor->mainSdlWindow);
  }
}

static void leveleditorclose(void** ptr)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}

static void constructToolWidget(GameLevelEditor* levelEditor)
{
  levelEditor->toolWidget = uiWidgetCreate();

  levelEditor->toolWidget->buttonNumber = 4;
  levelEditor->toolWidget->buttons = (UIButton**)malloc(levelEditor->toolWidget->buttonNumber * sizeof(UIButton*));
  HeroInt4 normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "newBtnD"));
  HeroInt4 highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "newBtnH"));
  levelEditor->toolWidget->buttons[0] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){30,0}, (HeroInt2){normal.z,normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[0],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[0],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[0],UIBUTTONSTATE_CLICK,highlight);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnH"));
  levelEditor->toolWidget->buttons[1] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_CLICK,highlight);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnH"));
  levelEditor->toolWidget->buttons[2] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_CLICK,highlight);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "exitBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "exitBtnH"));
  levelEditor->toolWidget->buttons[3] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_CLICK,highlight);

  levelEditor->toolWidget->imageNumber = 3;
  levelEditor->toolWidget->images = (UIImage**)malloc(levelEditor->toolWidget->imageNumber * sizeof(UIImage*));
  levelEditor->toolWidget->images[0] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,0}, (HeroInt2){30,30});
  uiImageSetRect(levelEditor->toolWidget->images[0], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "logo")));
  levelEditor->toolWidget->images[1] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){65,65}, (HeroInt2){510,310});
  uiImageSetRect(levelEditor->toolWidget->images[1], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
  levelEditor->toolWidget->images[2] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,420}, (HeroInt2){640,60});
  uiImageSetRect(levelEditor->toolWidget->images[2], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
}