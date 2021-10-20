#ifndef GAME_LEVELEDITOR_H_
#define GAME_LEVELEDITOR_H_

#include<stdbool.h>

typedef struct
{
  bool active;
} GameLevelEditor;

void* gameLevelEditorInit();
void gameLevelEditorUpdate(void* ptr);
void gameLevelEditorDestroy(void* ptr);

#endif