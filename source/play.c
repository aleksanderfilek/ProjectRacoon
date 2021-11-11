#include"Game/play.h"
#include"Game/state.h"
#include"Game/sharedata.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

extern HeroCore* core;

static void update(GamePlay* play, double deltaTime);
static void draw(GamePlay* play);

static void conctructPauseWidget(GamePlay* play);
static void playBtnClick(void* arg);
static void quitBtnClick(void* arg);

void* gamePlayInit()
{
  GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

  play->window = heroCoreModuleGet(core, "window");
  play->sdlWindow = heroWindowGetSdlWindow(play->window);
  play->input = heroCoreModuleGet(core, "input");

  play->pauseTextures[0] = heroTextureLoad("assets/sprites/playBtn.png", 0);
  play->pauseTextures[1] = heroTextureLoad("assets/sprites/quitBtn.png", 0);

  play->sounds[0] = heroAudioSoundLoad("assets/sounds/beep1.mp3");
  play->sounds[1] = heroAudioSoundLoad("assets/sounds/beep2.mp3");
  play->currentSound = 0;

  play->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  play->spriteBatch = heroSpriteBatchInit(play->window, 128, 10, play->shader);

  play->racket = racketCreate();
  play->ball = ballCreate();
  play->bricks = gameBricksCreate();

  GameSharedDataSystem* sharedata = heroCoreModuleGet(core, "data");
  const char* path = gameSharedDataGet(sharedata, "level");
  gameBricksLoadLevel(play->bricks, path);

  gamePlayRestart(play);

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  heroWindowSetBackgroundColor(play->window, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  conctructPauseWidget(play);

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

  heroAudioSoundUnload(play->sounds[0]);
  heroAudioSoundUnload(play->sounds[1]);

  uiWidgetDestroy(play->pauseWidget);
  heroTextureUnload(play->pauseTextures[0]);
  heroTextureUnload(play->pauseTextures[1]);

  heroSpriteBatchDestroy(play->spriteBatch);
  heroShaderUnload(play->shader);

  racketDestroy(play->racket);
  ballDestroy(play->ball);
  gameBricksDestory(play->bricks);

  free(ptr);
}

static void update(GamePlay* play, double deltaTime)
{
  
  if(play->pauseWidget->visible == false)
  {
    if(heroInputKeyDown(play->input, HERO_KEYCODE_ESCAPE))
    {
      playBtnClick(play);
    }
  }
  else
  {
    uiWidgetUpdate(play->pauseWidget, play->input);
  }

  if(play->paused) return;

  if(heroInputKeyDown(play->input, HERO_KEYCODE_R))
  {
    gamePlayRestart(play);
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
    gamePlayRestart(play);
  }

  bool collided = racketBallBounce(play->racket, play->ball);
  collided |= gameBricksCheckCollisions(play->bricks, play->ball);
  if(collided == true)
  {
    heroAudioSoundPlay(play->sounds[play->currentSound], false);
    play->currentSound++;
    play->currentSound %= 2;
  }
}

static void draw(GamePlay* play)
{
  heroWindowSetCurrentContext(play->window);
  glClear(GL_COLOR_BUFFER_BIT);
  heroSpriteBatchBegin(play->spriteBatch);

  gameBricksDraw(play->bricks, play->spriteBatch);
  ballDraw(play->ball, play->spriteBatch);
  racketDraw(play->racket, play->spriteBatch);
  uiWidgetDraw(play->pauseWidget, play->spriteBatch);

  heroSpriteBatchEnd(play->spriteBatch);
  SDL_GL_SwapWindow(play->sdlWindow);
}

void gamePlayRestart(GamePlay* play)
{
  printf("[Play] Game resterted\n");
  memcpy(play->bricks->currentIds, play->bricks->ids, BRICKS_COLUMNS*BRICKS_ROWS*sizeof(uint8_t));
  play->bricks->currentCount = play->bricks->count;
  play->started = false;
  play->paused = false;
  play->racket->position = (HeroFloat2){ 604.0f, 695.0f};
}

static void conctructPauseWidget(GamePlay* play)
{
  play->pauseWidget = uiWidgetCreate();
  play->pauseWidget->visible = false;
  play->pauseWidget->buttonNumber = 2;
  play->pauseWidget->buttons = (UIButton**)malloc(play->pauseWidget->buttonNumber * sizeof(UIButton*));
  play->pauseWidget->buttons[0] = uiButtonCreate(play->pauseTextures[0], (HeroInt2){420,286},(HeroInt2){386,64});
  uiButtonSetClickFunc(play->pauseWidget->buttons[0], playBtnClick, play);
  play->pauseWidget->buttons[1] = uiButtonCreate(play->pauseTextures[1], (HeroInt2){420,370},(HeroInt2){386,64});
  uiButtonSetClickFunc(play->pauseWidget->buttons[1], quitBtnClick, play);
}

static void playBtnClick(void* arg)
{
  GamePlay* play = (GamePlay*)arg;
  play->paused = !play->paused;
  play->pauseWidget->visible = play->paused;
}

static void quitBtnClick(void* arg)
{
  GameState* state = heroCoreModuleGet(core, "state");
  gameStateChange(state, GAMESTATE_MENU);
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
    gamePlayRestart(play);

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
  heroWindowSetCurrentContext(play->window);
  glClear(GL_COLOR_BUFFER_BIT);
  heroSpriteBatchBegin(play->spriteBatch);

  gameBricksDraw(play->bricks, play->spriteBatch);
  ballDraw(play->ball, play->spriteBatch);
  racketDraw(play->racket, play->spriteBatch);

  heroSpriteBatchEnd(play->spriteBatch);
  SDL_GL_SwapWindow(play->sdlWindow);
}

void gameDebugPlayUpdate(GamePlay* play)
{
  double deltaTime = heroCoreGetDeltaTime(core);
  if(heroInputKeyDown(play->input, HERO_KEYCODE_R))
  {
    gamePlayRestart(play);
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
    gamePlayRestart(play);
  }

  racketBallBounce(play->racket, play->ball);
  gameBricksCheckCollisions(play->bricks, play->ball);
}
  )