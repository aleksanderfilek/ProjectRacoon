#include"Game/menu.h"
#include"Game/sharedata.h"
#include"Game/standard.h"
#include"Game/menuWidget.h"

extern HeroCore* core;

void* menuInit()
{
    Menu* menu = allocate(Menu, 1);
    memset(menu, 0, sizeof(menu));

    menu->window = heroCoreModuleGet(core, "window");
    heroWindowSetBackgroundColor(menu->window, (HeroColor){0x00,0x00,0x00,0xFF});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    menu->input = heroCoreModuleGet(core, "input");

    GameSharedDataSystem* data = heroCoreModuleGet(core, "data");

    HeroShader* shader = gameSharedDataGet(data, "shader");
    // if shader not esists create one and add to system
    if(!shader)
    {
        shader = heroShaderLoad("assets/shaders/shader.vert", "assets/shaders/shader.frag");
        gameSharedDataAdd(data, "shader", shader, heroShaderUnload);
    }

    menu->spriteBatch = heroSpriteBatchInit(menu->window, 32, 32, shader);

    // if levels not esists load and add to system
    Levels* levels = gameSharedDataGet(data, "level");
    if(!levels)
    {
        gameSharedDataAdd(data, "level", NULL, NULL);
    }

    // if font not esists load and add to system
    HeroFont* font = gameSharedDataGet(data, "font");
    if(!font)
    {
        font = heroFontLoad("assets/fonts/arial.ttf", 28);
        gameSharedDataAdd(data, "font", font, heroFontUnload);
    }

    // construct widgets
    widgetMenuMainConstruct(menu, font);
    widgetMenuLevelsConstruct(menu, font);
    widgetMenuSettingsConstruct(menu, font);
    menu->currentState = MENUSTATE_MAIN;
    menu->stateWidgets[0]->visible = true;
    menu->activeOptionIndex = 0;

    // first menu draw
    menuDraw(menu);

    return menu;
}

void menuUpdate(void* ptr)
{
    Menu* menu = (Menu*)ptr;

    // handle widget events
    optionSwitch(menu);
    optionClick(menu);
}

void menuDestroy(void* ptr)
{
    Menu* menu = (Menu*)ptr;

    // free wdigets
    for(int i = 0; i < MENUSTATE_COUNT; i++)
    {
        uiWidgetDestroy(menu->stateWidgets[i]);
    }

    free(menu);
}

void menuDraw(Menu* menu)
{
    glClear(GL_COLOR_BUFFER_BIT);

    heroSpriteBatchBegin(menu->spriteBatch);
    uiWidgetDraw(menu->stateWidgets[menu->currentState], menu->spriteBatch);
    heroSpriteBatchEnd(menu->spriteBatch);

    heroWindowRender(menu->window);
}