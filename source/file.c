#include"Game/file.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SLASH "/"

char* gameFileGetName(const char* path)
{
  char* pch = strdup(path);

  pch = strtok (pch, SLASH);

  char* name = NULL;
  while (pch != NULL)
  {
    name = pch;
    pch = strtok (NULL, SLASH);
  }

  name = strtok (name, ".");
  name = strdup(name);
  free(pch);

  return name;
}

char** gameFileGetInDirectory(const char* directoryPath, uint32_t* length)
{
  #ifdef _WIN32
  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;

  char sPath[2048];
  char** files = (char**)malloc(sizeof(char*));
  files[0] = NULL;
  int fileNumber = 0;

  //Specify a file mask. *.* = We want everything!
  sprintf(sPath, "%s/*.*", directoryPath);

  if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
  {
      printf("Path not found: [%s]\n", directoryPath);
      free(files);
      return NULL;
  }

  do
  {
      //Find first file will always return "."
      //    and ".." as the first two directories.
      if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
      {
          //Build up our file path using the passed in
          //  [directoryPath] and the file/foldername we just found:
          sprintf(sPath, "%s/%s", directoryPath, fdFile.cFileName);
          //Is the entity a File or Folder?
          if(!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
          {
            files[fileNumber] = strdup(sPath);
            fileNumber++;
            files = (char**)realloc(files, (fileNumber + 1) * sizeof(char*));
            files[fileNumber] = NULL;
          }
      }
  }
  while(FindNextFile(hFind, &fdFile)); //Find the next file.

  FindClose(hFind); //Always, Always, clean things up!

  if(length)
  {
    *length = fileNumber;
  }

  return files;
  #else
  char sPath[2048];
  char** files = (char**)malloc(sizeof(char*));
  files[0] = NULL;
  int fileNumber = 0;

  DIR *d;
  struct dirent *dir;
  d = opendir(directoryPath);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
      {
        sprintf(sPath, "%s/%s", directoryPath, dir->d_name);
        files[fileNumber] = strdup(sPath);
        fileNumber++;
        files = (char**)realloc(files, (fileNumber + 1) * sizeof(char*));
        files[fileNumber] = NULL;
      }
    }
    closedir(d);
  }

  if(length)
  {
    *length = fileNumber;
  }

  return files;
  #endif
}