#include"Game/leveleditorWidget.h"

#include<stdlib.h>

extern HeroCore* core;

static void newBtnClick(void* arg);
static void saveBtnClick(void* arg);
static void openBtnClick(void* arg);
static void exitBtnClick(void* arg);

static void updateTitle(GameLevelEditor* levelEditor);

void constructToolWidget(GameLevelEditor* levelEditor)
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
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[3], exitBtnClick, NULL);

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
    (HeroInt2){normal.z+30,0}, (HeroInt2){640-normal.z,30});
  uiImageSetRect(levelEditor->toolWidget->images[3], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "panel")));
  levelEditor->toolWidget->images[4] = uiImageCreate(levelEditor->play->brickSpriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){500,240});
  levelEditor->toolWidget->images[5] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){50,24});
  uiImageSetRect(levelEditor->toolWidget->images[5], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "highlight")));
  levelEditor->toolWidget->images[5]->visible = false;

  levelEditor->toolWidget->labelNumber = levelEditor->play->brickSpriteSheet->length + 1;
  levelEditor->toolWidget->labels = (UILabel**)malloc(levelEditor->toolWidget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 18);
  for(int i = 0; i <= levelEditor->play->brickSpriteSheet->length; i++)
  {
    levelEditor->toolWidget->labels[i] = uiLabelCreate(levelEditor->infoText[i], font, (HeroColor){255,255,255,255},
     UIALLIGMENT_TOPLEFT, (HeroInt2){5,425}, (HeroInt2){630,50});
    levelEditor->toolWidget->labels[i]->visible = false;
  }
  levelEditor->toolWidget->labels[0]->visible = true;

  heroFontUnload(font);
}

void constructMainWidget(GameLevelEditor* levelEditor)
{
  levelEditor->mainWidget = uiWidgetCreate();

}

static void newBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

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
}

static void saveBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  char const * lFilterPatterns[1] = { "*.he" };
  char* filePath = heroFileSaveDialogOpen("Save file", "levelname.he", 1, lFilterPatterns, NULL);
  if(filePath == NULL)
  {
    printf("[Level editor] Save file path not selected\n");
    return;
  }

  levelEditor->currentPath = filePath;

  gameSave(levelEditor);
}

static void openBtnClick(void* arg)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)arg;

  if(levelEditor->changed == true)
  {
    printf("[Level editor] All changes lost\n");
  }

  char const * lFilterPatterns[1] = { "*.he" };
  char* filePath = heroFileDialogOpen("Select file", "", 1, lFilterPatterns, NULL, 0);
  if(filePath == NULL)
  {
    printf("[Level editor] Open file path not selected\n");
    return;
  }
  
  FILE* file = fopen(filePath, "rb");
  
  fread(levelEditor->play->bricks, sizeof(uint8_t), BRICKS_COLUMNS * BRICKS_ROWS, file);

  fclose(file);

  levelEditor->currentPath = filePath;
  levelEditor->changed = false;

  updateTitle(levelEditor);

  printf("[Level editor] Level loaded, path: %s\n", filePath);

  gameDrawMain(levelEditor);
}

static void exitBtnClick(void* arg)
{
  printf("[Level editor] Editor closed\n");
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}

static void updateTitle(GameLevelEditor* levelEditor)
{
  char* pch = (char*)malloc(strlen(levelEditor->currentPath) * sizeof(char));
  strcat(pch, levelEditor->currentPath);
  pch = strtok (pch,"/");
  char* name = NULL;
  while (pch != NULL)
  {
    if(pch != NULL)
    {
      name = pch;
    }
    pch = strtok (NULL, "/");
  }

  name = strtok (name, ".");

  sprintf(levelEditor->title, "%s %s  ", "ProjectRacoon - LevelEditor", name);
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);

  free(pch);
}