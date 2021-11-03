#include"Game/sharedata.h"

#include<stdlib.h>

GameShareData* gameShareDataInit(uint32_t capacity)
{
  GameShareData* shareData = (GameShareData*)malloc(sizeof(GameShareData));

  return shareData;
}

void gameShareDataDestroy(void* ptr)
{
  GameShareData* shareData = (GameShareData*)ptr;

  for(int i = 0; i < shareData->capacity; i++)
  {
    if(shareData->data[i] == NULL)
    {
      continue;
    }

    free(shareData->data[i]);
  }
  free(shareData->data);

  free(ptr);
}