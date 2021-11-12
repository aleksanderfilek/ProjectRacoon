#ifndef GAME_FILE_H_
#define GAME_FILE_H_

#include<stdint.h>

#ifdef _WIN32
#include<windows.h>
#else
#include <dirent.h> 
#endif

char* gameFileGetName(const char* path);
char** gameFileGetInDirectory(const char* directoryPath, uint32_t* length);

#endif