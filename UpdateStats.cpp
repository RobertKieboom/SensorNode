#include "UpdateStats.h"

static byte nextId = 0;

UpdateStats* createSingleUpdateStats(char* s, byte sensorId)
{
  char* parts[10];
  byte  partCount = 0;
  char* pipeSeperator = "|";
  char* valueSeperator = ":";
  char* p = strtok(s, pipeSeperator);

  while(p != NULL)
  {
    parts[partCount++] = p;
    p = strtok(NULL, pipeSeperator);
  }

  UpdateStats* stats = new struct UpdateStats();
  stats->SensorId = sensorId;
  stats->ExpressionId = nextId++;
  stats->UpdateInfos = new UpdateInfo[partCount];
  UpdateInfo* infos = stats->UpdateInfos;

  for(byte i = 0; i < partCount; i++)
  {
    char* cmd = strtok(parts[i], valueSeperator);
    char* val = strtok(NULL, valueSeperator);
    infos[i].value = atoi(val);
    if(strcmp_P(cmd, PSTR("time")) == 0)
    {
      infos[i].type = UT_Time;
    }
    else if(strcmp_P(cmd, PSTR("below")) == 0)
    {
      infos[i].type = UT_Below;
    }
    else if(strcmp_P(cmd, PSTR("above")) == 0)
    {
      infos[i].type = UT_Above;
    }
  }

  stats->UpdateInfoCount = partCount;
  stats->LastUpdateValue = 0;
  stats->LastUpdateMillis = 0;

  return stats;
}

void addUpdateStats(LinkedList<UpdateStats*> &list, char* s, byte sensorId)
{
  char* parts[5];
  byte  partCount = 0;
  char* andSeperator = "&";  
  char* p = strtok(s, andSeperator);

  while(p != NULL)
  {
    parts[partCount++] = p;
    p = strtok(NULL, andSeperator);
  }
  
  for(byte i = 0; i < partCount; i++)
  {
    UpdateStats* stats = createSingleUpdateStats(parts[i], sensorId);
    list.add(stats);
  }
}

