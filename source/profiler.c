#include"Game/profiler.h"

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

#define C(x) #x,
static const char * const names[] = { NAMES };

static Profiler* _profiler = NULL;
static clock_t startTimer;

Profiler* profilerInit()
{
  if(_profiler != NULL)
  {
    return _profiler;
  }

  Profiler* profiler = (Profiler*)malloc(sizeof(Profiler));
  _profiler = profiler;

  profiler->buffersNumber = 1;
  profiler->buffers = (ProfilerBuffer**)malloc(sizeof(ProfilerBuffer*));
  profiler->buffers[0] = (ProfilerBuffer*)malloc(sizeof(ProfilerBuffer));
  profiler->currentBufferPtr = profiler->buffers[0];
  memset(profiler->currentBufferPtr, 0, sizeof(ProfilerBuffer));
  profiler->buffers[0]->length = 0;
  profiler->buffers[0]->currentColumnPtr = profiler->buffers[0]->buff[0];

  return profiler;
}

void profilerUpdate(void* ptr)
{
  Profiler* profiler = (Profiler*)ptr;

  profiler->currentBufferPtr->length++;
  profiler->currentBufferPtr->currentColumnPtr += PROFILER_COUNT;
  if(profiler->currentBufferPtr->length >= BUFFER_SIZE)
  {
    profiler->buffersNumber++;
    profiler->buffers = (ProfilerBuffer**)realloc(profiler->buffers, profiler->buffersNumber * sizeof(ProfilerBuffer*));
    profiler->buffers[profiler->buffersNumber - 1] = (ProfilerBuffer*)malloc(sizeof(ProfilerBuffer));
    profiler->currentBufferPtr = profiler->buffers[profiler->buffersNumber - 1];
    memset(profiler->currentBufferPtr, 0, sizeof(ProfilerBuffer));
    profiler->currentBufferPtr->length = 0;
    profiler->currentBufferPtr->currentColumnPtr = profiler->currentBufferPtr->buff[0];
    printf("[Profiler] Number of buffers increased, number = %d, buffer size = %d\n", profiler->buffersNumber, BUFFER_SIZE);
  }
}

void profilerDestory(void* ptr)
{
  Profiler* profiler = (Profiler*)ptr;

  FILE* output = fopen("output.csv", "w");
  if(output == NULL)
  {
    printf("[Profiler] Could not open or create output file\n");
    return;
  }

  fprintf(output, "lp");
  for(int i = 0; i < PROFILER_COUNT; i++)
  {
    fprintf(output, ",%s", names[i]);
  }
  fprintf(output, "\n");

  int counter = 0;
  for(int k = 0; k < profiler->buffersNumber; k++)
  {
    for(int i = 0; i < profiler->buffers[k]->length; i++)
    {
      fprintf(output, "%d", counter);
      counter++;
      for(int j = 0; j < PROFILER_COUNT; j++)
      {
        fprintf(output, ",%f", profiler->buffers[k]->buff[i][j]);
      }
      fprintf(output, "\n");
    }
  }

  fclose(output);

  for(int i = 0; i < profiler->buffersNumber; i++)
  {
    free(profiler->buffers[i]);
  }
  free(profiler->buffers);

  free(profiler);
}

void internalProfilerFuncStart(uint32_t index)
{
  startTimer = clock();
}

void internalProfilerFuncEnd(uint32_t index)
{
  clock_t endTimer = clock();
  double duration = (double)(endTimer - startTimer) / CLOCKS_PER_SEC;
  _profiler->currentBufferPtr->currentColumnPtr[index] = duration * 1000.0;
}

void internalProfilerVariableSet(uint32_t index, float variable)
{
  _profiler->currentBufferPtr->currentColumnPtr[index] = variable;
}