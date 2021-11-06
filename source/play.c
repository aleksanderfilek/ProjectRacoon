
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

  play->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  play->spriteBatch = heroSpriteBatchInit(play->window, 128, 10, play->shader);

  play->racket = racketCreate();
  play->ball = ballCreate();
  play->bricks = gameBricksCreate();

  play->started = false;
  play->paused = false;

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
  heroShaderUnload(play->shader);

  racketDestroy(play->racket);
  ballDestroy(play->ball);
  gameBricksDestory(play->bricks);

  free(ptr);
}

static void update(GamePlay* play, double deltaTime)
{
  if(heroInputKeyDown(play->input, HERO_KEYCODE_A))
  {
    play->paused = !play->paused;
  }

  if(play->paused) return;

  if(heroInputKeyDown(play->input, HERO_KEYCODE_R))
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
  gameBricksCheckCollisions(play->bricks, play->ball);
}

static void draw(GamePlay* play)
{
  heroWindowSetCurrentContext(play->window);
  glClear(GL_COLOR_BUFFER_BIT);
  heroSpriteBatchBegin(play->spriteBatch);

  gameBricksDraw(play->bricks, play->spriteBatch);
  ballDraw(play->ball, play->spriteBatch);
  racketDraw(play->racket, play->spriteBatch);

  heroSpriteBatchEnd(play->spriteBatch);
  SDL_GL_SwapWindow(play->sdlWindow);
}

DEBUG_CODE( 
void* gameDebugPlayInit(HeroWindow* window, HeroInput* input, 
  HeroShader* shader, GameBricks* bricks)
  {
    GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

    play->window = window;
    play->sdlWindow = heroWindowGetSdlWindow(play->window);
    play->input = input;

    play->shader = shader;
    heroWindowSetCurrentContext(window);
    play->spriteBatch = heroSpriteBatchInit(play->window, 128, 10, play->shader);

    play->racket = racketCreate();
    play->ball = ballCreate();
    play->ball->position = play->racket->position;
    play->ball->position = heroMathAddF2(play->ball->position, (HeroFloat2){32, -8});
    play->bricks = gameBricksCreate();
    memcpy(play->bricks->ids, bricks->ids, BRICKS_COLUMNS*BRICKS_ROWS*sizeof(uint8_t));

    play->started = false;
    play->paused = false;

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    heroWindowSetBackgroundColor(play->window, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  return play;
} 

void gameDebugPlayDestroy(void* ptr)
{
  GamePlay* play = (GamePlay*)ptr;

  heroSpriteBatchDestroy(play->spriteBatch);

  racketDestroy(play->racket);
  ballDestroy(play->ball);
  gameBricksDestory(play->bricks);

  free(ptr);
}

void gameDebugPlayDraw(GamePlay* play)
{
  draw(play);
}
  )