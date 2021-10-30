
#include"Game/play.h"

#include<stdlib.h>
#include<stdio.h>

extern HeroCore* core;

static void update(GamePlay* gamePlay);
static void draw(GamePlay* gamePlay);

void* gamePlayInit()
{
  GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

  play->window = heroCoreModuleGet(core, "window");
  play->sdlWindow = heroWindowGetSdlWindow(play->window);
  play->input = heroCoreModuleGet(core, "input");

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  heroWindowSetBackgroundColor(play->window, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  return play;
}

void gamePlayUpdate(void* ptr)
{
  GamePlay* play = (GamePlay*)ptr;

  update(play);

  draw(play);
}

void gamePlayDestroy(void* ptr)
{
  free(ptr);
}

static void update(GamePlay* gamePlay)
{

}

static void draw(GamePlay* gamePlay)
{
  glClear(GL_COLOR_BUFFER_BIT);

  heroSpriteBatchBegin(gamePlay->spriteBatch);

  gamePlayBricksDraw(gamePlay);

  heroSpriteBatchEnd(gamePlay->spriteBatch);

  SDL_GL_SwapWindow(gamePlay->sdlWindow);
}

void gamePlayBricksDraw(GamePlay* play)
{
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int index = BRICKS_COLUMNS*y + x;
      if(play->bricks[index] == 0)
      {
        continue;
      }

      HeroInt2 position = { 15 + 50*x, 15 + 24*y };
      HeroInt4 rect = gameSpriteSheetGetRect(play->brickSpriteSheet, play->bricks[index]-1);
      heroSpriteBatchDrawTextureEx(play->spriteBatch, play->brickSpriteSheet->texture,
        position, (HeroInt2){50, 24}, rect, 0.0f, (HeroColor){0xFF,0xFF,0xFF,0xFF});
    }
  }
}