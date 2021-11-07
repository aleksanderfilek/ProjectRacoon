#include"Game/leveleditorWidget.h"
#include"Game/file.h"

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdio.h>

#ifdef DEBUG
extern HeroCore* core;

void updateTitle(GameLevelEditor* levelEditor);

void constructToolWidget(GameLevelEditor* levelEditor)
{
  levelEditor->toolWidget = uiWidgetCreate();

  levelEditor->toolWidget->buttonNumber = 5;
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
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[0], newBtnClick, levelEditor);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnH"));
  levelEditor->toolWidget->buttons[1] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[1], saveBtnClick, levelEditor);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnH"));
  levelEditor->toolWidget->buttons[2] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[2], openBtnClick, levelEditor);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "exitBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "exitBtnH"));
  levelEditor->toolWidget->buttons[3] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[3],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[3], exitBtnClick, levelEditor);

  int panelPosx = normal.z;

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "playBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "playBtnH"));
  levelEditor->toolWidget->buttons[4] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){610,0}, (HeroInt2){30, 30});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[4], gameBtnClick, levelEditor);

  levelEditor->toolWidget->imageNumber = 6;
  levelEditor->toolWidget->images = (UIImage**)malloc(levelEditor->toolWidget->imageNumber * sizeof(UIImage*));
  levelEditor->toolWidget->images[0] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,0}, (HeroInt2){30,30});
  uiImageSetRect(levelEditor->toolWidget->images[0], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "logo")));
  levelEditor->toolWidget->images[1] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){65,95}, (HeroInt2){510,250});
  uiImageSetRect(levelEditor->toolWidget->images[1], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
  levelEditor->toolWidget->images[2] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){0,420}, (HeroInt2){640,60});
  uiImageSetRect(levelEditor->toolWidget->images[2], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
  levelEditor->toolWidget->images[3] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){panelPosx+30,0}, (HeroInt2){580-panelPosx,30});
  uiImageSetRect(levelEditor->toolWidget->images[3], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
  levelEditor->toolWidget->images[4] = uiImageCreate(levelEditor->bricks->spriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){500,240});
  levelEditor->toolWidget->images[5] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){50,24});
  uiImageSetRect(levelEditor->toolWidget->images[5], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "highlight")));
  levelEditor->toolWidget->images[5]->visible = false;

  levelEditor->toolWidget->labelNumber = levelEditor->bricks->spriteSheet->length + 1;
  levelEditor->toolWidget->labels = (UILabel**)malloc(levelEditor->toolWidget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 18);
  for(int i = 0; i <= levelEditor->bricks->spriteSheet->length; i++)
  {
    levelEditor->toolWidget->labels[i] = uiLabelCreate(levelEditor->infoText[i], font, (HeroColor){255,255,255,255},
     UIALLIGMENT_TOPLEFT, (HeroInt2){5,425}, (HeroInt2){630,50});
    levelEditor->toolWidget->labels[i]->visible = false;
  }
  levelEditor->toolWidget->labels[0]->visible = true;

  heroFontUnload(font);
}

void constructSaveWidget(GameLevelEditor* levelEditor)
{
  levelEditor->saveWidget = uiWidgetCreate();
  levelEditor->saveWidget->visible = false;

  levelEditor->saveWidget->buttonNumber = 2;
  levelEditor->saveWidget->buttons = (UIButton**)malloc(levelEditor->saveWidget->buttonNumber * sizeof(UIButton*));
  HeroInt4 normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "yesD"));
  HeroInt4 highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "yesH"));
  levelEditor->saveWidget->buttons[0] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){180,250}, (HeroInt2){50,30});
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[0],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[0],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[0],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->saveWidget->buttons[0], saveYesBtnClick, NULL);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "noD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "noH"));
  levelEditor->saveWidget->buttons[1] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){410,250}, (HeroInt2){50,30});
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[1],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[1],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->saveWidget->buttons[1],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->saveWidget->buttons[1], saveNoBtnClick, NULL);

  levelEditor->saveWidget->imageNumber = 1;
  levelEditor->saveWidget->images = (UIImage**)malloc(levelEditor->saveWidget->imageNumber * sizeof(UIImage*));
  levelEditor->saveWidget->images[0] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
      (HeroInt2){170,190}, (HeroInt2){300,100});
  uiImageSetRect(levelEditor->saveWidget->images[0], 
    gameSpriteSheetGetRectByName(levelEditor->levelEditorSpriteSheet, "panel"));

  levelEditor->saveWidget->labelNumber = 1;
  levelEditor->saveWidget->labels = (UILabel**)malloc(levelEditor->saveWidget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 18);
  levelEditor->saveWidget->labels[0] = uiLabelCreate("Save?",font,(HeroColor){0xFF,0xFF,0xFF,0xFF},
    UIALLIGMENT_CENTER, (HeroInt2){170,190}, (HeroInt2){300,60});
  heroFontUnload(font);
}

void newBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->changed == true)
  {
    levelEditor->saveWidget->visible = true;
    void** args = (void**)malloc(2*sizeof(void*));
    args[0] = levelEditor;
    args[1] = (void*)newBtnClick;
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[0], saveYesBtnClick, (void*)args);
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[1], saveNoBtnClick, (void*)args);
    return;
  }

  char const * lFilterPatterns[1] = { "*.he" };
  char* filePath = heroFileSaveDialogOpen("New file", "levelname.he", 1, lFilterPatterns, NULL);
  if(filePath == NULL)
  {
    printf("[Level editor] New file path not selected\n");
    return;
  }

  levelEditor->currentPath = filePath;
  levelEditor->changed = false;

  updateTitle(levelEditor);
  levelEditor->title[strlen(levelEditor->title) - 1] = ' ';
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);
  memset(levelEditor->bricks->ids, 0, BRICKS_COLUMNS*BRICKS_ROWS*sizeof(uint8_t));
}

void saveBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->currentPath == NULL)
  {
    printf("[Level editor] Nothing to save!\n");
    return;
  }

  char const * lFilterPatterns[1] = { "*.he" };
  char* filePath = heroFileSaveDialogOpen("Save file", "levelname.he", 1, lFilterPatterns, NULL);
  if(filePath == NULL)
  {
    printf("[Level editor] Save file path not selected\n");
    return;
  }

  levelEditor->currentPath = filePath;

  gameSave(levelEditor);

  updateTitle(levelEditor);
}

void openBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->changed == true)
  {
    levelEditor->saveWidget->visible = true;
    void** args = (void**)malloc(2*sizeof(void*));
    args[0] = levelEditor;
    args[1] = (void*)openBtnClick;
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[0], saveYesBtnClick, (void*)args);
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[1], saveNoBtnClick, (void*)args);
    return;
  }

  char const * lFilterPatterns[1] = { "*.he" };
  char* filePath = heroFileDialogOpen("Select file", "", 1, lFilterPatterns, NULL, 0);
  if(filePath == NULL)
  {
    printf("[Level editor] Open file path not selected\n");
    return;
  }

  gameBricksLoadLevel(levelEditor->bricks, filePath);

  levelEditor->currentPath = filePath;
  levelEditor->changed = false;

  updateTitle(levelEditor);

  printf("[Level editor] Level loaded, path: %s\n", filePath);

  gameDrawMain(levelEditor);
}

void exitBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->changed == true)
  {
    levelEditor->saveWidget->visible = true;
    void** args = (void**)malloc(2*sizeof(void*));
    args[0] = levelEditor;
    args[1] = (void*)exitBtnClick;
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[0], saveYesBtnClick, (void*)args);
    uiButtonSetClickFunc(levelEditor->saveWidget->buttons[1], saveNoBtnClick, (void*)args);
    return;
  }

  printf("[Level editor] Editor closed\n");
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}

void gameBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->currentPath == NULL)
  {
    printf("[Level editor] Trying to start empty level!\n");
    return;
  }

  levelEditor->playing = !levelEditor->playing;

  if(levelEditor->playing == true)
  {
    HeroInt4 normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
      gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "stopBtnD"));
    HeroInt4 highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
      gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "stopBtnH"));

    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_NORMAL,normal);
    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_HOVER,highlight);
    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_CLICK,highlight);

    levelEditor->play = gameDebugPlayInit(levelEditor->mainWindow, levelEditor->input,
      levelEditor->shader, levelEditor->bricks);
    gameDebugPlayDraw(levelEditor->play);
  }
  else
  {
    HeroInt4 normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
      gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "playBtnD"));
    HeroInt4  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
      gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "playBtnH"));

    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_NORMAL,normal);
    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_HOVER,highlight);
    uiButtonSetStateRect(levelEditor->toolWidget->buttons[4],UIBUTTONSTATE_CLICK,highlight);

    gameDebugPlayDestroy(levelEditor->play);
    levelEditor->play = NULL;
    gameDrawTool(levelEditor);
  }

  gameDrawMain(levelEditor);
}

void updateTitle(GameLevelEditor* levelEditor)
{
  char* name = gameFileGetName(levelEditor->currentPath);
  memset(levelEditor->title, 0, sizeof(levelEditor->title));
  sprintf(levelEditor->title, "%s %s  ", "ProjectRacoon - LevelEditor", name);
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);

  free(name);
}

void saveYesBtnClick(void* arg)
{
  void** args = (void**)arg;
  GameLevelEditor* levelEditor = (GameLevelEditor*)args[0];
  void (*func)(void*arg);
  func = args[1];

  gameSave(levelEditor);

  levelEditor->changed = false;

  levelEditor->title[strlen(levelEditor->title) - 1] = ' ';
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);

  levelEditor->saveWidget->visible = false;

  func(levelEditor);

  free(args);
}

void saveNoBtnClick(void* arg)
{
  void** args = (void**)arg;
  GameLevelEditor* levelEditor = (GameLevelEditor*)args[0];
  void (*func)(void*arg);
  func = args[1];

  levelEditor->changed = false;

  levelEditor->title[strlen(levelEditor->title) - 1] = ' ';
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);

  levelEditor->saveWidget->visible = false;

  func(levelEditor);

  free(args);
}
#endif