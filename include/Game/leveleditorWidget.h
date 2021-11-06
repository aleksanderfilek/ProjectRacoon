#ifndef LEVELEDITORWIDGET_H_
#define LEVELEDITORWIDGET_H_

#include"Game/leveleditor.h"
#include"Game/play.h"

void constructToolWidget(GameLevelEditor* levelEditor);
void constructSaveWidget(GameLevelEditor* levelEditor);

void newBtnClick(void* arg);
void saveBtnClick(void* arg);
void openBtnClick(void* arg);
void exitBtnClick(void* arg);
void gameBtnClick(void* arg);
void saveYesBtnClick(void* arg);
void saveNoBtnClick(void* arg);

#endif