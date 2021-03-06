#include"Game/play.h"
#include"Game/state.h"
#include"Game/sharedata.h"
#include"Game/playWidget.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

extern HeroCore* core;

static void dumpPlayData(GamePlay* play);

static void update(GamePlay* play, double deltaTime);
static void draw(GamePlay* play);

void* gamePlayInit()
{
  GamePlay* play = (GamePlay*)malloc(sizeof(GamePlay));

  play->window = heroCoreModuleGet(core, "window");
  play->sdlWindow = heroWindowGetSdlWindow(play->window);
  play->input = heroCoreModuleGet(core, "input");

  play->btnTextures[0] = heroTextureLoad("assets/sprites/playBtn.png", 0);
  play->btnTextures[1] = heroTextureLoad("assets/sprites/quitBtn.png", 0);
  play->btnTextures[2] = heroTextureLoad("assets/sprites/levelBtn.png", 0);

  play->sounds[0] = heroAudioSoundLoad("assets/sounds/beep1.mp3");
  play->sounds[1] = heroAudioSoundLoad("assets/sounds/beep2.mp3");
  play->currentSound = 0;

  play->shader = heroShaderLoad("assets/shaders/shader.vert","assets/shaders/shader.frag");
  play->spriteBatch = heroSpriteBatchInit(play->window, 400, 10, play->shader);

  play->racket = racketCreate();
  play->ball = ballCreate();
  play->bricks = gameBricksCreate();

  GameSharedDataSystem* sharedata = heroCoreModuleGet(core, "data");
  char* path = gameSharedDataGet(sharedata, "level");
  gameBricksLoadLevel(play->bricks, path);

  gamePlayRestart(play);

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  heroWindowSetBackgroundColor(play->window, (HeroColor){0x1E,0x1E,0x1E,0xFF});

  play->currentWidget = PLAY_STATE_GAME;
  play->widgets[0] = conctructPauseWidget(play);
  play->widgets[1] = constructFailedWidget(play);
  play->widgets[2] = constructWinWidget(play);

  play->animationTimer = 0.0f;

  play->effectsManager = effectsManagerCreate();
  
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

  for(int i = 0; i < 3; i++)
  {
    uiWidgetDestroy(play->widgets[i]);
  }

  heroTextureUnload(play->btnTextures[0]);
  heroTextureUnload(play->btnTextures[1]);
  heroTextureUnload(play->btnTextures[2]);

  heroSpriteBatchDestroy(play->spriteBatch);
  heroShaderUnload(play->shader);

  racketDestroy(play->racket);
  ballDestroy(play->ball);
  gameBricksDestory(play->bricks);

  effectsManagerDestroy(play->effectsManager);

  free(ptr);
}

static void update(GamePlay* play, double deltaTime)
{
  // dump data
  if(heroInputKeyDown(play->input, HERO_KEYCODE_Q))
  {
    dumpPlayData(play);
  }

  if(play->currentWidget > PLAY_STATE_GAME) uiWidgetUpdate(play->widgets[play->currentWidget-1], play->input);

  // pause menu
  if(play->currentWidget == PLAY_STATE_GAME)
  {
    if(heroInputKeyDown(play->input, HERO_KEYCODE_ESCAPE))
    {
      setPlayState(play, PLAY_STATE_PAUSE);
    }
  }

  // pause game
  if(play->paused) return;

  play->animationTimer += (float)deltaTime;
  if(play->animationTimer >= ANIMATION_FRAME_TIME)
  {
    play->animationTimer = 0.0f;
    gameBricksAnimation(play->bricks);
  }

  gameBricksEvents(play->bricks, deltaTime);

  // restart
  if(heroInputKeyDown(play->input, HERO_KEYCODE_R))
  {
    gamePlayRestart(play);
  }

  // update racket
  racketUpdate(play->racket, deltaTime, play->input);

  // 
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
    printf("[Play] Game Failed\n");
    setPlayState(play, PLAY_STATE_FAILED);
  }

  bool collided = racketBallBounce(play->racket, play->ball);
  int brickIndex = gameBricksCheckCollisions(play->bricks, play->ball);
  collided |= brickIndex >= 0;

  gameBricksResolveChange(play->bricks, brickIndex);

  effectsManagerUpdate(play->effectsManager, deltaTime, play->racket);

  if(collided == true)
  {
    heroAudioSoundPlay(play->sounds[play->currentSound], false);
    play->currentSound++;
    play->currentSound %= 2;
  }

  if(play->bricks->currentCount <= 0)
  {
    printf("[Play] Game won\n");
    setPlayState(play, PLAY_STATE_WIN);
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
  effectsManagerDraw(play->effectsManager, play->spriteBatch);

  if(play->currentWidget > PLAY_STATE_GAME) uiWidgetDraw(play->widgets[play->currentWidget-1], play->spriteBatch);

  heroSpriteBatchEnd(play->spriteBatch);
  SDL_GL_SwapWindow(play->sdlWindow);
}

void gamePlayRestart(GamePlay* play)
{
  printf("[Play] Game resterted\n");
  gameBricksReset(play->bricks);
  play->started = false;
  play->paused = false;
  play->racket->position = (HeroFloat2){ 604.0f, 695.0f};
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

    play->sounds[0] = heroAudioSoundLoad("assets/sounds/beep1.mp3");
    play->sounds[1] = heroAudioSoundLoad("assets/sounds/beep2.mp3");
    play->currentSound = 0;

    play->animationTimer = 0.0f;


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

  // dump data
  if(heroInputKeyDown(play->input, HERO_KEYCODE_Q))
  {
    dumpPlayData(play);
  }

  play->animationTimer += (float)deltaTime;
  if(play->animationTimer >= ANIMATION_FRAME_TIME)
  {
    play->animationTimer = 0.0f;
    gameBricksAnimation(play->bricks);
  }

  gameBricksEvents(play->bricks, deltaTime);

  // restart
  if(heroInputKeyDown(play->input, HERO_KEYCODE_R))
  {
    gamePlayRestart(play);
  }

  // update racket
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
  int brickIndex = gameBricksCheckCollisions(play->bricks, play->ball);
  collided |= brickIndex >= 0;

  gameBricksResolveChange(play->bricks, brickIndex);

  if(collided == true)
  {
    heroAudioSoundPlay(play->sounds[play->currentSound], false);
    play->currentSound++;
    play->currentSound %= 2;
  }
}
  )

static void dumpPlayData(GamePlay* play)
{
  FILE* output = fopen("dump.txt", "w");

  fprintf(output, "Play\n");
  fprintf(output, "Animation timer: %f\n", play->animationTimer);

  fprintf(output, "\nRacket\n");
  fprintf(output, "\tPosition (%f, %f)\n", play->racket->position.x, play->racket->position.y);
  fprintf(output, "\tSpeed modifier: %f\n", play->racket->speedModifier);

  fprintf(output, "\nBall\n");
  fprintf(output, "\tPosition (%f, %f)\n", play->ball->position.x, play->ball->position.y);
  fprintf(output, "\tSpeed modifier: %f\n", play->ball->speedModifier);
  fprintf(output, "\tVelocity (%f, %f)\n", play->ball->velocity.x, play->ball->velocity.y);

  fprintf(output, "\nBricks\n");
  fprintf(output, "\tCount %d\n", play->bricks->count);
  fprintf(output, "\tCurrent count %d\n", play->bricks->currentCount);
  fprintf(output, "\n\tCurrent ids\n");
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    fprintf(output, "\t");
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int i = BRICKS_COLUMNS * y + x;
      fprintf(output, " %2d", play->bricks->currentIds[i]);
    }
    fprintf(output, "\n");
  }

  fprintf(output, "\n\tIds\n");
  for(int y = 0; y < BRICKS_ROWS; y++)
  {
    fprintf(output, "\t");
    for(int x = 0; x < BRICKS_COLUMNS; x++)
    {
      int i = BRICKS_COLUMNS * y + x;
      fprintf(output, " %2d", play->bricks->ids[i]);
    }
    fprintf(output, "\n");
  }

  fprintf(output, "\nBricks events\n");
  for(int i = 0; i < play->bricks->eventsNumber; i++)
  {
    fprintf(output, "\tid: %d, typeNum: %d, timer: %f\n", play->bricks->events[i].index, 
      play->bricks->events[i].type, play->bricks->events[i].timer);
  }

  fclose(output);
}

void setPlayState(GamePlay* play, PlayStateEnum state)
{
  if(state > PLAY_STATE_GAME) play->paused = true;
  play->currentWidget = state;
}