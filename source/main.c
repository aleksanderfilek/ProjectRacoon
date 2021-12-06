#include"Hero/Hero.h"

#include"Game/sharedata.h"
#include"Game/menu.h"

HeroCore* core = NULL;

void gameClose(void**);

int main(int argc, char *argv[])
{
  core = heroCoreInit();
  {
    void* window = heroWindowInit("ProjectRacoon", 1280, 720, 0);
    heroWindowSetEvent((HeroWindow*)window, HERO_WINDOW_CLOSE, gameClose);
    heroCoreModuleAdd(core, "window", window, NULL, heroWindowDestroy);

    void* input = heroInputInit();
    heroCoreModuleAdd(core, "input", input, heroInputUpdate, heroInputDestroy);

    void* event = heroEventInit();
    heroEventAddWindow(event, (HeroWindow*)window);
    heroCoreModuleAdd(core, "event", event, heroEventUpdate, heroEventDestroy);

    void* audio = heroAudioInit(HERO_AUDIO_FREQUENCY_44100, HERO_AUDIO_CHANNEL_MONO, 
      MIX_DEFAULT_FORMAT, 2048, HERO_AUDIO_FORMAT_MP3);
    heroCoreModuleAdd(core, "audio", audio, NULL, heroAudioDestroy);

    void* data = gameShareDataInit(1);
    heroCoreModuleAdd(core, "data", data, NULL, gameShareDataDestroy);

    void* state = menuInit();
    heroCoreModuleAdd(core, "state", state, menuUpdate, menuDestroy);
  }

  heroCoreStart(core);

  return 0;
}

void gameClose(void** data)
{
  heroCoreClose(core);
}
