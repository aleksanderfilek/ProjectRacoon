#include"Game/leveleditor.h"
#include"Game/leveleditorWidget.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Hero/H_files.h"
#ifdef DEBUG
extern HeroCore* core;

static void levelEditorclose(void**ptr);
static void update(GameLevelEditor* levelEditor);
static void draw(GameLevelEditor* levelEditor);

static void loadBricksInfo(GameLevelEditor* levelEditor);
static void selectBrick(GameLevelEditor* levelEditor, uint32_t index);

void* gameLevelEditorInit()
{
  GameLevelEditor* levelEditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));
  levelEditor->changed = false;
  levelEditor->currentBrick = 0;
  levelEditor->currentPath = NULL;
  strcat(levelEditor->title, "ProjectRacoon - LevelEditor");
  levelEditor->playing = false;

  levelEditor->bricks = gameBricksCreate();
  memset(levelEditor->bricks->ids, 0, sizeof(levelEditor->bricks->ids));


  levelEditor->input = heroCoreModuleGet(core, "input");

  levelEditor->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  levelEditor->levelEditorSpriteSheet = gameSpriteSheetLoad("assets/debug/LevelEditor.he");
  levelEditor->emptyBrickRect = gameSpriteSheetGetRectByName(levelEditor->levelEditorSpriteSheet, "emptyBrick");
  loadBricksInfo(levelEditor);

  levelEditor->mainWindow = heroCoreModuleGet(core, "window");
  levelEditor->mainSdlWindow = heroWindowGetSdlWindow(levelEditor->mainWindow);
  heroWindowSetBackgroundColor(levelEditor->mainWindow, (HeroColor){0,255,0,255});
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  heroWindowSetBackgroundColor(levelEditor->mainWindow, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  levelEditor->mainSpriteBatch = heroSpriteBatchInit(levelEditor->mainWindow, 100, 32, levelEditor->shader);

  levelEditor->toolWindow = heroWindowInit(levelEditor->title, 640, 480, 0);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  heroWindowSetBackgroundColor(levelEditor->toolWindow, (HeroColor){0x1E,0x1E,0x1E,0xFF});
  heroWindowSetEvent(levelEditor->toolWindow, HERO_WINDOW_CLOSE, levelEditorclose);
  levelEditor->toolSdlWindow = heroWindowGetSdlWindow(levelEditor->toolWindow);
  HeroEvent* event = heroCoreModuleGet(core, "event");
  heroEventAddWindow(event, levelEditor->toolWindow);

  levelEditor->toolSpriteBatch = heroSpriteBatchInit(levelEditor->toolWindow, 100, 32, levelEditor->shader);

  constructToolWidget(levelEditor);
  constructSaveWidget(levelEditor);

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

  for(int i = 1; i <= levelEditor->bricks->spriteSheet->length; i++)
  {
    free(levelEditor->infoText[i]);
  }
  gameBricksDestory(levelEditor->bricks);

  gameSpriteSheetUnload(levelEditor->levelEditorSpriteSheet);

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
    uiWidgetUpdate(levelEditor->saveWidget, levelEditor->input);

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
      if(index > levelEditor->bricks->spriteSheet->length)
      {
        index = 0;
      }

      selectBrick(levelEditor, index);
    }
  }
  else if(levelEditor->currentPath != NULL && heroWindowIsFocused(levelEditor->mainWindow) == true)
  {
    if(levelEditor->playing == true)
    {
      gamePlayUpdate(levelEditor->play);
    }
    else
    {
      int mouseX, mouseY;
      heroInputGetMousePosition(levelEditor->input, &mouseX, &mouseY);
      bool leftMouseClick = heroInputMouseButtonDown(levelEditor->input, HERO_MOUSE_LEFT);
      bool ctrlMouseClick = heroInputKeyPressed(levelEditor->input, HERO_KEYCODE_LCTRL);
      if(leftMouseClick == true && mouseX > 15 && mouseX < 1265 && mouseY > 15 && mouseY < 303)
      {
        int normX = mouseX - 15;
        int normY = mouseY - 15;
        int iX = normX / 50;
        int iY = normY / 24;
        int index = 25 * iY + iX;
        levelEditor->bricks->ids[index] = (ctrlMouseClick == false)? levelEditor->currentBrick : 0;

        levelEditor->changed = true;

        levelEditor->title[strlen(levelEditor->title) - 1] = '*';
        heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);
      }
    }

    if(levelEditor->playing == true && heroInputKeyDown(levelEditor->input, HERO_KEYCODE_ESCAPE))
    {
      gameBtnClick(levelEditor);
    }
  }

  if(levelEditor->changed == true && 
    heroInputKeyPressed(levelEditor->input, HERO_KEYCODE_LCTRL) == true &&
    heroInputKeyDown(levelEditor->input, HERO_KEYCODE_S) == true)
  {
    gameSave(levelEditor);
  }
}

static void draw(GameLevelEditor* levelEditor)
{
  if(heroWindowIsFocused(levelEditor->toolWindow) == true)
  {
    gameDrawTool(levelEditor);
  }
  else if(heroWindowIsFocused(levelEditor->mainWindow) == true)
  {
    gameDrawMain(levelEditor);
  }
}

void gameDrawTool(GameLevelEditor* levelEditor)
{
  heroWindowSetCurrentContext( levelEditor->toolWindow );

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(levelEditor->toolSpriteBatch);

  uiWidgetDraw(levelEditor->toolWidget, levelEditor->toolSpriteBatch);
  uiWidgetDraw(levelEditor->saveWidget, levelEditor->toolSpriteBatch);

  heroSpriteBatchEnd(levelEditor->toolSpriteBatch);

  SDL_GL_SwapWindow(levelEditor->toolSdlWindow);
}

void gameDrawMain(GameLevelEditor* levelEditor)
{
  if(levelEditor->playing == true)
  {
    return;
  }

  heroWindowSetCurrentContext( levelEditor->mainWindow );

  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(levelEditor->mainSpriteBatch);

  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      if(levelEditor->bricks->ids[index] > 0)
      {
        continue;
      }

      HeroInt2 position = { 15 + 50*x, 15 + 24*y };
      heroSpriteBatchDrawTextureEx(levelEditor->mainSpriteBatch, levelEditor->levelEditorSpriteSheet->texture,
        position, (HeroInt2){50, 24}, levelEditor->emptyBrickRect, 0.0f, (HeroColor){0xFF,0xFF,0xFF,0xFF});
    }
  }
  gameBricksDraw(levelEditor->bricks, levelEditor->mainSpriteBatch);

  heroSpriteBatchEnd(levelEditor->mainSpriteBatch);

  SDL_GL_SwapWindow(levelEditor->mainSdlWindow);
}  

static void levelEditorclose(void** ptr)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
}

static void loadBricksInfo(GameLevelEditor* levelEditor)
{
  levelEditor->infoText = (char**)malloc(
    (levelEditor->bricks->spriteSheet->length + 1) * sizeof(char*));
  memset(levelEditor->infoText, 0, (levelEditor->bricks->spriteSheet->length + 1) * sizeof(char*));
  levelEditor->infoText[0] = "None";

  FILE* file = fopen("assets/debug/BricksInfo.txt", "r");
  if(!file)
  {
    printf("Could not load bricks info file!\n");
    exit(-1);
  }

  size_t bufferSize;
  for(int i = 1; i <= levelEditor->bricks->spriteSheet->length; i++)
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

void gameSave(GameLevelEditor* levelEditor)
{
  FILE* file = fopen(levelEditor->currentPath, "wb");

  fwrite(levelEditor->bricks->ids, sizeof(uint8_t), BRICKS_COLUMNS * BRICKS_ROWS, file);

  fclose(file);

  levelEditor->changed = false;
  levelEditor->title[strlen(levelEditor->title) - 1] = ' ';
  heroWindowSetTitle(levelEditor->toolWindow, levelEditor->title);
}
#endif