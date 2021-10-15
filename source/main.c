#include"Hero/Hero.h"
#include"Game/menu.h"

HeroCore* core = NULL;

void gameClose(void*);

int main(int argc, char *argv[])
{
  core = heroCoreInit();

  {
    void* window = heroWindowInit("window", 1280, 720, 0);
    heroWindowSetEvent((HeroWindow*)window, HERO_WINDOW_CLOSE, gameClose);
    heroCoreModuleAdd(core, "window", window, NULL, heroWindowDestroy);

    void* input = heroInputInit();
    heroCoreModuleAdd(core, "input", input, heroInputUpdate, heroInputDestroy);

    void* event = heroEventInit();
    heroEventAddWindow(event, (HeroWindow*)window);
    heroCoreModuleAdd(core, "event", event, heroEventUpdate, heroEventDestroy);

    void* menu = gameMenuInit();
    heroCoreModuleAdd(core, "menu", menu, gameMenuUpdate, gameMenuDestroy);
  }

  heroCoreStart(core);

  return 0;
}

void gameClose(void* data)
{
  heroCoreClose(core);
}