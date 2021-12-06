#include"Game/sharedata.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

static void removeData(GameShareData* data);

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
    removeData(&system->datas[i]);
  }
  free(system->datas);

  free(ptr);
}

void gameSharedDataAdd(GameSharedDataSystem* system, const char* name, 
  void* variable, void (*delteFunc)(void*))
{
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data->name == NULL)
    {
      data->name = strdup(name);
      data->data = variable;
      data->delteFunc = delteFunc;  
      printf("[Share data] Variable %s added to slot %d\n", name, i);
      return;
    }
  }

  int index = system->capacity;
  system->capacity++;
  system->datas = (GameShareData*)realloc(system->datas, system->capacity *sizeof(GameShareData));

  system->datas[index].name = strdup(name);
  system->datas[index].data = variable;
  system->datas[index].delteFunc = delteFunc;  
  printf("[Share data] Variable %s added\n", name);
}

void* gameSharedDataGet(GameSharedDataSystem* system, const char* name)
{
  printf("[Share data] Variable %s ",name);
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data->data != NULL && strcmp(data->name, name) == 0)
    {
      printf("found\n");
      return data->data;
    }
  }

  printf("not found\n");
  return NULL;
}

void gameSharedDataRemove(GameSharedDataSystem* system, const char* name)
{
  for(int i = 0; i < system->capacity; i++)
  {
    GameShareData* data = &system->datas[i];
    if(data != NULL && strcmp(data->name, name) == 0)
    {
      removeData(data);
      return;
    }
  }
}

static void removeData(GameShareData* data)
{
  if(data->name != NULL)
  {
    free(data->name);
    data->name = NULL;
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