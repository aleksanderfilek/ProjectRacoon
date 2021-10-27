#include"Game/leveleditor.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern HeroCore* core;

static void levelEditorclose(void**ptr);
static void constructToolWidget(GameLevelEditor* levelEditor);
static void update(GameLevelEditor* levelEditor);
static void draw(GameLevelEditor* levelEditor);
static void loadBricksInfo(GameLevelEditor* levelEditor);
static void selectBrick(GameLevelEditor* levelEditor, uint32_t index);

static void newBtnClick(void* arg);
static void saveBtnClick(void* arg);
static void openBtnClick(void* arg);
static void exitBtnClick(void* arg);

void* gameLevelEditorInit()
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));
  levelEditor->changed = false;
  levelEditor->currentBrick = 0;

  levelEditor->input = heroCoreModuleGet(core, "input");

  levelEditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  levelEditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");
  levelEditor->brickSpriteSheet = gameSpriteSheetLoad("assets/sprites/Bricks.he");
  loadBricksInfo(levelEditor);

  levelEditor->mainWindow = heroCoreModuleGet(core, "window");
  levelEditor->mainSdlWindow = heroWindowGetSdlWindow(levelEditor->mainWindow);
  heroWindowSetBackgroundColor(levelEditor->mainWindow, (HeroColor){0,255,0,255});

  levelEditor->mainSpriteBatch = heroSpriteBatchInit(levelEditor->mainWindow, 10, 10, levelEditor->shader);


  levelEditor->toolWindow = heroWindowInit("LevelEditorTools", 640, 480, 0);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  heroWindowSetBackgroundColor(levelEditor->toolWindow, (HeroColor){0x1E,0x1E,0x1E,0xFF});
  heroWindowSetEvent(levelEditor->toolWindow, HERO_WINDOW_CLOSE, levelEditorclose);
  levelEditor->toolSdlWindow = heroWindowGetSdlWindow(levelEditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, levelEditor->toolWindow);

  levelEditor->toolSpriteBatch = heroSpriteBatchInit(levelEditor->toolWindow, 10, 10, levelEditor->shader);

  constructToolWidget(levelEditor);

  return levelEditor;
}

void gameLevelEditorUpdate(void* ptr)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)ptr;

  update(levelEditor);

  draw(levelEditor);

}

void gameLevelEditorDestroy(void* ptr)
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)ptr;

  uiWidgetDestroy(levelEditor->toolWidget);

  for(int i = 1; i <= levelEditor->brickSpriteSheet->length; i++)
  {
    free(levelEditor->infoText[i]);
  }

  gameSpriteSheetUnload(levelEditor->levelEditorSpriteSheet);
  gameSpriteSheetUnload(levelEditor->brickSpriteSheet);

  heroShaderUnload(levelEditor->shader);
  heroSpriteBatchDestroy(levelEditor->toolSpriteBatch);
  heroSpriteBatchDestroy(levelEditor->mainSpriteBatch);

  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventRemoveWindow(event, levelEditor->toolWindow);
  heroWindowDestroy(levelEditor->toolWindow);

  free(ptr);
}

static void update(GameLevelEditor* levelEditor)
{
  if(heroWindowIsFocused(levelEditor->toolWindow) == true)
  {
    uiWidgetUpdate(levelEditor->toolWidget, levelEditor->input);

    int mouseX, mouseY;
    heroInputGetMousePosition(levelEditor->input, &mouseX, &mouseY);
    bool leftMouseClick = heroInputMouseButtonDown(levelEditor->input, HERO_MOUSE_LEFT);
    if(leftMouseClick == true && mouseX > 70 && mouseX < 570 && mouseY > 100 && mouseY < 340)
    {
      int normX = mouseX - 70;
      int normY = mouseY - 100;
      int iX = normX / 50;
      int iY = normY / 24;
      int index = 10 * iY + iX + 1;
      if(index > levelEditor->brickSpriteSheet->length)
      {
        index = 0;
      }

      selectBrick(levelEditor, index);
    }
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

static void levelEditorclose(void** ptr)
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
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[0], newBtnClick, NULL);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "saveBtnH"));
  levelEditor->toolWidget->buttons[1] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[1],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[1], saveBtnClick, NULL);

  normal = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnD"));
  highlight = gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "openBtnH"));
  levelEditor->toolWidget->buttons[2] = uiButtonCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){normal.x + 30,0}, (HeroInt2){normal.z - normal.x, normal.w});
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_NORMAL,normal);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_HOVER,highlight);
  uiButtonSetStateRect(levelEditor->toolWidget->buttons[2],UIBUTTONSTATE_CLICK,highlight);
  uiButtonSetClickFunc(levelEditor->toolWidget->buttons[2], openBtnClick, NULL);

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
  levelEditor->toolWidget->images[4] = uiImageCreate(levelEditor->brickSpriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){500,240});
  levelEditor->toolWidget->images[5] = uiImageCreate(levelEditor->levelEditorSpriteSheet->texture,
    (HeroInt2){70,100}, (HeroInt2){50,24});
  uiImageSetRect(levelEditor->toolWidget->images[5], gameSpriteSheetGetRect(levelEditor->levelEditorSpriteSheet, 
    gameSpriteSheetGet(levelEditor->levelEditorSpriteSheet, "highlight")));
  levelEditor->toolWidget->images[5]->visible = false;

  levelEditor->toolWidget->labelNumber = levelEditor->brickSpriteSheet->length + 1;
  levelEditor->toolWidget->labels = (UILabel**)malloc(levelEditor->toolWidget->labelNumber * sizeof(UILabel*));
  HeroFont* font = heroFontLoad("assets/fonts/arial.ttf", 18);
  for(int i = 0; i <= levelEditor->brickSpriteSheet->length; i++)
  {
    levelEditor->toolWidget->labels[i] = uiLabelCreate(levelEditor->infoText[i], font, (HeroColor){255,255,255,255},
     UIALLIGMENT_TOPLEFT, (HeroInt2){5,425}, (HeroInt2){630,50});
    levelEditor->toolWidget->labels[i]->visible = false;
  }
  levelEditor->toolWidget->labels[0]->visible = true;

  heroFontUnload(font);
}

static void loadBricksInfo(GameLevelEditor* levelEditor)
{
  levelEditor->infoText = (char**)malloc(
    (levelEditor->brickSpriteSheet->length + 1) * sizeof(char*));
  memset(levelEditor->infoText, 0, (levelEditor->brickSpriteSheet->length + 1) * sizeof(char*));
  levelEditor->infoText[0] = "None";

  FILE* file = fopen("assets/debug/BricksInfo.txt", "r");
  if(!file)
  {
    printf("Could not load bricks info file!\n");
    exit(-1);
  }

  size_t bufferSize;
  for(int i = 1; i <= levelEditor->brickSpriteSheet->length; i++)
  {
    getline(&levelEditor->infoText[i], &bufferSize, file);
    uint32_t lastChar = strlen(levelEditor->infoText[i]) - 1;
    levelEditor->infoText[i][lastChar] = ' ';
  }

  fclose(file);
}

static void selectBrick(GameLevelEditor* levelEditor, uint32_t index)
{
  levelEditor->toolWidget->labels[levelEditor->currentBrick]->visible = false;

  if(levelEditor->currentBrick == index || index == 0)
  {
    levelEditor->toolWidget->images[5]->visible = false;
    levelEditor->currentBrick = 0;
    levelEditor->toolWidget->labels[0]->visible = true;
    printf("[Level editor] Brick selection: %d\n", 0);

    return;
  }
  
  levelEditor->toolWidget->images[5]->visible = true;
  levelEditor->toolWidget->labels[index]->visible = true;
  levelEditor->currentBrick = index;

  index--;

  int iX = index % 10;
  int iY = index / 10;
  int posX = iX * 50;
  int posY = iY * 24;
  posX += 70;
  posY += 100;
  levelEditor->toolWidget->images[5]->position = (HeroInt2){ posX, posY };

  printf("[Level editor] Brick selection: %d\n", levelEditor->currentBrick);
}

static void newBtnClick(void* arg)
{
  printf("new click\n");
}

static void saveBtnClick(void* arg)
{
  printf("save click\n");
}

static void openBtnClick(void* arg)
{
  printf("open click\n");
}

static void exitBtnClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}