#ifndef GAME_SPRITESHEET_H_
#define GAME_SPRITESHEET_H_

#include"Hero/Hero.h"

#include<stdint.h>

typedef struct
{
  HeroTexture* texture;
  char** names;
  HeroInt4* rects;
  uint32_t length; 
} GameSpriteSheet;

typedef uint32_t GameSpriteSheetIndex;

GameSpriteSheet* gameSpriteSheetLoad(const char* path);
void gameSpriteSheetUnload(GameSpriteSheet* spriteSheet);
GameSpriteSheetIndex gameSpriteSheetGet(GameSpriteSheet* spriteSheet, const char* name);
HeroInt4 gameSpriteSheetGetRect(GameSpriteSheet* spriteSheet, GameSpriteSheetIndex index);
HeroInt4 gameSpriteSheetGetRectByName(GameSpriteSheet* spriteSheet, const char* name);

#endif