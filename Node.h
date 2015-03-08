#ifndef _NODE_H_
#define _NODE_H_

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

#include "ISensor.h"
#include "IActuator.h"
#include "ICommand.h"

class Node
{
  public:
    Node(byte id);
    void SetSensors(ISensor* sensors[], byte count);
    void SetActuators(IActuator* actuators[], byte count);
    void SetCommands(ICommand* commands[], byte count);

    void Initialize();
    byte GetSensorCount() { return _sensorCount; }
    ISensor* GetSensor(byte index);
    char* GetSensorLine(byte index, char* line);
    
    char* GetProcessorInfo(char* info);
    char* GetCompileInfo(char* info);
    
    DateTime GetDateTime();
    void HandleCommand(char* text, Stream &io);
    void ShowDateTime(Stream &io, const DateTime& dt);
    
    boolean Connect(char* mac, byte timeOut);
    boolean Ping(DateTime& now, int& newPingTime);

  private:
    ISensor** _sensors;
    byte _sensorCount;
    IActuator** _actuators;
    byte _actuatorCount;
    ICommand** _commands;
    byte _commandCount;

    ISensor* getSensorWithId(byte id);
    void handleSlist(Stream& io, char* storage);
    void handleSget(Stream& io, char* storage, CommandInfo& info);
    void handleSconf(Stream& io, char* storage, CommandInfo& info);
    void handleAlist(Stream& io, char* storage);
  };

#endif

