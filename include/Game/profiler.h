#ifndef PROFILER_H_
#define PROFILER_H_

#include"Hero/Hero.h"

#include<stdint.h>

#define BUFFER_SIZE 4096

#define NAMES \
  C(func_gameBricksDraw) \
  C(var_bricksCount) \
  C(func_swapwindow) \
  C(func_batchend) \
  C(func_draw)

#define C(x) x,
enum namesE { NAMES PROFILER_COUNT };
#undef C

typedef struct 
{
  uint32_t length;
  float* currentColumnPtr;
  float buff[BUFFER_SIZE][PROFILER_COUNT];
} ProfilerBuffer;

typedef struct
{
  uint32_t buffersNumber;
  ProfilerBuffer** buffers;
  ProfilerBuffer* currentBufferPtr;
  uint32_t currentBufferIndex;
} Profiler;

Profiler* profilerInit();
void profilerUpdate(void* ptr);
void profilerDestory(void* ptr);

void internalProfilerFuncStart(uint32_t index);
void internalProfilerFuncEnd(uint32_t index);

#define PROFILE_FUNC(name, function) \
  DEBUG_CODE( internalProfilerFuncStart(name); \
  function; \
  internalProfilerFuncEnd(name); )

void internalProfilerVariableSet(uint32_t index, float variable);

#define PROFILE_SETVAR(name, variable) \
  DEBUG_CODE( internalProfilerVariableSet(name, variable); )

#endif