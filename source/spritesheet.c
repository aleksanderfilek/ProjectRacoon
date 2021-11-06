#include"Game/spritesheet.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

GameSpriteSheet* gameSpriteSheetLoad(const char* path)
{
  GameSpriteSheet* spriteSheet = (GameSpriteSheet*)malloc(sizeof(GameSpriteSheet));

  FILE* srcFile = fopen(path, "rb");
  if(srcFile == NULL)
  {
    printf("[Sprite sheet] Could not load file, path %s\n", path);
    return NULL;
  }

  uint32_t size;
  fread(&size, sizeof(uint32_t), 1, srcFile); // get name length
  char* texturePath = (char*)malloc(size* sizeof(char)); // may cause memory leak, if hero texture not free memory
  fread(texturePath, sizeof(char), size, srcFile);
  fread(&spriteSheet->length, sizeof(uint32_t), 1, srcFile); // get tiles count

  spriteSheet->names = (char**)malloc(spriteSheet->length * sizeof(char*));
  spriteSheet->rects = (HeroInt4*)malloc(spriteSheet->length * sizeof(HeroInt4));
  for(int i = 0; i < spriteSheet->length; i++)
  {
    fread(&size, sizeof(uint32_t), 1, srcFile);
    spriteSheet->names[i] = (char*)malloc(size * sizeof(char));
    fread(spriteSheet->names[i], sizeof(char), size, srcFile);
    int rect[4];
    fread(rect, sizeof(int), 4, srcFile);
    spriteSheet->rects[i] = (HeroInt4){ rect[0],rect[1],rect[2],rect[3]};
  }

  fclose(srcFile);

  spriteSheet->texture = heroTextureLoad(texturePath, 0);

  return spriteSheet;
}

void gameSpriteSheetUnload(GameSpriteSheet* spriteSheet)
{
  heroTextureUnload(spriteSheet->texture);
  free(spriteSheet->names);
  free(spriteSheet->rects);
  free(spriteSheet);
}

GameSpriteSheetIndex gameSpriteSheetGet(GameSpriteSheet* spriteSheet, const char* name)
{
  for(int i = 0; i < spriteSheet->length; i++)
  {
    if(strcmp(name, spriteSheet->names[i]) == 0)
    {
      return i;
    }
  }
  return 0;
}

HeroInt4 gameSpriteSheetGetRect(GameSpriteSheet* spriteSheet, GameSpriteSheetIndex index)
{
  return spriteSheet->rects[index];
}

HeroInt4 gameSpriteSheetGetRectByName(GameSpriteSheet* spriteSheet, const char* name)
{
  GameSpriteSheetIndex index = gameSpriteSheetGet(spriteSheet, name);
  return gameSpriteSheetGetRect(spriteSheet, index);
}