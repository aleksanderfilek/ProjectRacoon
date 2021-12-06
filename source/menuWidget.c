#include"Game/menuWidget.h"
#include"Game/ui.h"

#include<stdint.h>

static uint8_t optionsCount[] = {3, 1, 1};

extern HeroCore* core;

void widgetMenuMainConstruct(Menu* menu, HeroFont* font)
{
    UIWidget* widget = uiWidgetCreate();
    menu->stateWidgets[0] = widget;
    widget->visible = false;

    uiWidgetLabelsNumber(menu->stateWidgets[0], 6);
    widget->labels[0] = uiLabelCreate("Play", font, (HeroColor){0xFF,0xFF,0xFF,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){100,100}, (HeroInt2){200,30});
    widget->labels[1] = uiLabelCreate("Play", font, (HeroColor){0xFF,0x00,0x00,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){101,99}, (HeroInt2){200,30});
    widget->labels[1]->visible = true;

    widget->labels[2] = uiLabelCreate("Settings", font, (HeroColor){0xFF,0xFF,0xFF,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){100,140}, (HeroInt2){200,30});
    widget->labels[3] = uiLabelCreate("Settings", font, (HeroColor){0xFF,0x00,0x00,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){101,139}, (HeroInt2){200,30});
    widget->labels[3]->visible = false;

    widget->labels[4] = uiLabelCreate("Quit", font, (HeroColor){0xFF,0xFF,0xFF,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){100,180}, (HeroInt2){200,30});
    widget->labels[5] = uiLabelCreate("Quit", font, (HeroColor){0xFF,0x00,0x00,0xFF}, 
        UIALLIGMENT_TOPLEFT, (HeroInt2){101,179}, (HeroInt2){200,30});
    widget->labels[5]->visible = false;

    // menu->uiEvents[2] = exitClick;
}

void widgetMenuLevelsConstruct(Menu* menu, HeroFont* font)
{
    menu->stateWidgets[1] = uiWidgetCreate();
}

void widgetMenuSettingsConstruct(Menu* menu, HeroFont* font)
{
    menu->stateWidgets[2] = uiWidgetCreate();
}

void optionSwitch(Menu* menu)
{
    if(heroInputKeyDown(menu->input, HERO_KEYCODE_DOWN))
    {
        menu->stateWidgets[menu->currentState]->labels[2*menu->activeOptionIndex + 1]->visible = false;
        menu->activeOptionIndex++;
        menu->activeOptionIndex %= optionsCount[menu->currentState];
        menu->stateWidgets[menu->currentState]->labels[2*menu->activeOptionIndex + 1]->visible = true;
        menuDraw(menu);
    }
    else if(heroInputKeyDown(menu->input, HERO_KEYCODE_UP))
    {
        menu->stateWidgets[menu->currentState]->labels[2*menu->activeOptionIndex + 1]->visible = false;
        menu->activeOptionIndex--;
        if(menu->activeOptionIndex < 0)
            menu->activeOptionIndex = optionsCount[menu->currentState] - 1;
        menu->stateWidgets[menu->currentState]->labels[2*menu->activeOptionIndex + 1]->visible = true;
        menuDraw(menu);
    }
}

void optionClick(Menu* menu)
{
    if(heroInputKeyDown(menu->input, HERO_KEYCODE_RETURN))
    {
        // menu->uiEvents[menu->activeOptionIndex](NULL);
    }
}

void exitClick(void* arg)
{
    heroCoreClose(core);
}