#include"Game/filedialog.h"

#ifdef _WIN32
#include <windows.h>
#else

#endif

#include<stdio.h>
#include<string.h>

const char* openFileDialog(HeroWindow* window)
{
#ifdef _WIN32

  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(heroWindowGetSdlWindow(window), &wmInfo);
  HWND hwnd = wmInfo.info.win.window;

  OPENFILENAME ofn = {0}; 
  TCHAR szFile[260]={0};
  // Initialize remaining fields of OPENFILENAME structure
  ofn.lStructSize = sizeof(ofn); 
  ofn.hwndOwner = hwnd; 
  ofn.lpstrFile = szFile; 
  ofn.nMaxFile = sizeof(szFile); 
  ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0"); 
  ofn.nFilterIndex = 1; 
  ofn.lpstrFileTitle = NULL; 
  ofn.nMaxFileTitle = 0; 
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if(GetOpenFileName(&ofn) == TRUE)
  { 
  // use ofn.lpstrFile here

    printf("%s\n",ofn.lpstrFile);
  }
  
#else

#endif
}