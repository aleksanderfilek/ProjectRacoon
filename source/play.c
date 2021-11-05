
#include"Game/play.h"
#include"Game/state.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

extern HeroCore* core;

static void update(GamePlay* play, double deltaTime);
static void draw(GamePlay* play);

void* gamePlayInit()
{
  GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

  play->window = heroCoreModuleGet(core, "window");
  play->sdlWindow = heroWindowGetSdlWindow(play->window);
  play->input = heroCoreModuleGet(core, "input");

  play->brickSpriteSheet = gameSpriteSheetLoad("assets/sprites/Bricks.he");
  play->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  play->spriteBatch = heroSpriteBatchInit(play->window, 100, 16, play->shader);

  play->racket = racketCreate();
  play->ball = ballCreate();

  play->started = false;
  play->paused = false;

  memset(play->bricks, 1, BRICKS_COLUMNS*BRICKS_ROWS*sizeof(uint8_t));

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  heroWindowSetBackgroundColor(play->window, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  return play;
}

void gamePlayUpdate(void* ptr)
{
  GamePlay* play = (GamePlay*)ptr;

  double deltaTime = heroCoreGetDeltaTime(core);

  update(play, deltaTime);

  draw(play);
}

void gamePlayDestroy(void* ptr)
{
  GamePlay* play = (GamePlay*)ptr;

  heroSpriteBatchDestroy(play->spriteBatch);

  gameSpriteSheetUnload(play->brickSpriteSheet);

  racketDestroy(play->racket);
  ballDestroy(play->ball);

  free(ptr);
}

static void update(GamePlay* play, double deltaTime)
{
  if(heroInputKeyDown(play->input, HERO_KEYCODE_A))
  {
    play->paused = !play->paused;
  }

  if(play->paused) return;

  if(heroInputKeyDown(play->input, HERO_KEYCODE_ESCAPE))
  {
    printf("[Play] Game resterted\n");
    GameState* state = heroCoreModuleGet(core, "state");
    gameStateChange(state, GAMESTATE_PLAY);
  }

  racketUpdate(play->racket, deltaTime, play->input);

  if(play->started == false)
  {
    racketPositioning(play->racket, play->ball);
    if(heroInputKeyDown(play->input, HERO_KEYCODE_SPACE))
    {
      play->started = true;
    }
    return;
  }
  
  if(!ballUpdate(play->ball, deltaTime))
  {
    printf("[Play] Ball under racket\n");
    GameState* state = heroCoreModuleGet(core, "state");
    gameStateChange(state, GAMESTATE_PLAY);
  }

  racketBallBounce(play->racket, play->ball);
}

static void draw(GamePlay* play)
{
  glClear(GL_COLOR_BUFFER_BIT);
  heroSpriteBatchBegin(play->spriteBatch);

  gamePlayBricksDraw(play);

  ballDraw(play->ball, play->spriteBatch);
  racketDraw(play->racket, play->spriteBatch);

  heroSpriteBatchEnd(play->spriteBatch);
  SDL_GL_SwapWindow(play->sdlWindow);
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