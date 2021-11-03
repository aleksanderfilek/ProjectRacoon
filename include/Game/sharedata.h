#ifndef GAME_SHAREDATA_H_
#define GAME_SHAREDATA_H_

#include<stdint.h>

typedef struct
{
  uint32_t capacity;
  void** data;
}GameShareData;

GameShareData* gameShareDataInit(uint32_t capacity);
void gameShareDataDestroy(void* ptr);

#endif