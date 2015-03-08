#ifndef _UPDATE_STATS_H_
#define _UPDATE_STATS_H_

#include <Arduino.h>
#include "LinkedList.h"

enum UpdateType : byte
{
  UT_Time = 0,
  UT_Below = 1,
  UT_Above = 2
};

struct UpdateInfo
{
  UpdateType type;
  int value;
};

struct UpdateStats
{
  byte ExpressionId;
  byte SensorId;
  unsigned long LastUpdateMillis;
  int LastUpdateValue;

  UpdateInfo* UpdateInfos;
  byte UpdateInfoCount;
};

extern void addUpdateStats(LinkedList<UpdateStats*> &list, char* s, byte sensorId);


#endif

