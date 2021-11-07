#include"Game/sharedata.h"

#include<stdlib.h>
#include<string.h>

static void remove(GameShareData* data);

GameSharedDataSystem* gameShareDataInit(uint32_t capacity)
{
  GameSharedDataSystem* system = (GameSharedDataSystem*)malloc(sizeof(GameSharedDataSystem));

  system->capacity = capacity;
  system->datas = (GameShareData*)malloc(capacity * sizeof(GameShareData));
  memset(system->datas, 0, capacity * sizeof(GameShareData));

  return system;
}

void gameShareDataDestroy(void* ptr)
{
  GameSharedDataSystem* system = (GameSharedDataSystem*)ptr;

  for(int i = 0; i < system->capacity; i++)
  {
    remove(&system->datas[i]);
  }
  free(system->datas);

  free(ptr);
}

void gameSharedDataAdd(GameSharedDataSystem* system, const char* name, 
  void* variable, void (*delteFunc)(void*))
{
  int firstEmpty = -1;
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data->name != NULL)
    {
      if(strcmp(data->name, name) == 0)
      {
        data->data = variable;
        data->delteFunc = delteFunc;
        return;
      }
    }
    else if(firstEmpty == -1)
    {
      firstEmpty = i;
    }
  }

  if(firstEmpty >= 0)
  {
    system->datas[firstEmpty].name = strdup(name);
    system->datas[firstEmpty].data = variable;
    system->datas[firstEmpty].delteFunc = delteFunc;
    return;
  }

  int index = system->capacity;
  system->capacity++;
  system->datas = (GameShareData*)realloc(system->datas, system->capacity *sizeof(GameShareData));
  system->datas[firstEmpty].name = strdup(name);
  system->datas[firstEmpty].data = variable;
  system->datas[firstEmpty].delteFunc = delteFunc;  
}

void* gameSharedDataGet(GameSharedDataSystem* system, const char* name)
{
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data != NULL && strcmp(data->name, name) == 0)
    {
      return data->data;
    }
  }

  return NULL;
}

void gameSharedDataRemove(GameSharedDataSystem* system, const char* name)
{
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data != NULL && strcmp(data->name, name) == 0)
    {
      remove(data);
      return;
    }
  }
}

static void remove(GameShareData* data)
{
  if(data->name != NULL)
  {
    free(data->name);
  }

  if(data->data == NULL)
  {
    return;
  }

  if(data->delteFunc == NULL)
  {
    free(data->data);
    return;
  }

  data->delteFunc(data->data);
}