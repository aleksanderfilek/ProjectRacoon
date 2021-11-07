#ifndef GAME_SHAREDATA_H_
#define GAME_SHAREDATA_H_

#include<stdint.h>

typedef struct
{
  void* data;
  char* name;
  void (*delteFunc)(void*);
} GameShareData;

typedef struct
{
  uint32_t capacity;
  GameShareData* datas;
}GameSharedDataSystem;

GameSharedDataSystem* gameShareDataInit(uint32_t capacity);
void gameShareDataDestroy(void* ptr);
void gameSharedDataAdd(GameSharedDataSystem* system, const char* name, void* variable, void (*delteFunc)(void*));
void* gameSharedDataGet(GameSharedDataSystem* system, const char* name);
void gameSharedDataRemove(GameSharedDataSystem* system, const char* name);

#endif