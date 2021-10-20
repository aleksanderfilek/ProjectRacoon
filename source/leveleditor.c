#include"Game/leveleditor.h"

#include<stdlib.h>

void* gameLevelEditorInit()
{
  GameLevelEditor* leveleditor = (GameLevelEditor*)malloc(sizeof(GameLevelEditor));

  return leveleditor;
}

void gameLevelEditorUpdate(void* ptr)
{

}

void gameLevelEditorDestroy(void* ptr)
{
  free(ptr);
}