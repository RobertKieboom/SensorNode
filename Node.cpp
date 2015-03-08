#include "CommandParser.h"
#include "Node.h"

METRIC_LIST(GENERATE_PROGMEM_VARIABLE)
const char* metricNames[] =
{
  METRIC_LIST(GENERATE_PROGMEM_REFERENCE)
};
SENSOR_TYPE_LIST(GENERATE_PROGMEM_VARIABLE)
const char* sensorTypeNames[] =
{
  SENSOR_TYPE_LIST(GENERATE_PROGMEM_REFERENCE)
};
ACTUATOR_TYPE_LIST(GENERATE_PROGMEM_VARIABLE)
const char* actuatorTypeNames[] =
{
  ACTUATOR_TYPE_LIST(GENERATE_PROGMEM_REFERENCE)
};

const char compiled[] PROGMEM = "Compiled:";
const char compile_date[] PROGMEM = __DATE__;
const char compile_time[] PROGMEM = __TIME__;
RTC_Millis dateTime;

Node::Node(byte id)
{
  _sensorCount = 0;
  _actuatorCount = 0;
  _commandCount = 0;
  RTC_Millis::begin(DateTime(__DATE__,__TIME__));
}

void Node::SetSensors(ISensor* sensors[], byte count)
{
  _sensors = sensors;
  _sensorCount = count;
}

void Node::SetActuators(IActuator* actuators[], byte count)
{
  _actuators = actuators;
  _actuatorCount = count;
}

void Node::SetCommands(ICommand* commands[], byte count)
{
  _commands = commands;
  _commandCount = count;
}

void Node::Initialize()
{
  for(int i = 0; i < _sensorCount; i++)
    _sensors[i] -> Initialize();
  delay(2000);
}

ISensor* Node::GetSensor(byte index)
{
  return _sensors[index];
}

DateTime Node::GetDateTime()
{
  return RTC_Millis::now();
}

void addInt(char *s, int value)
{
  int index = strlen(s);
  ltoa(value, s + index, 10);
}

void addChar(char *s, char c)
{
  int index = strlen(s);
  s[index] = c;
  s[index+1] = '\0';
}

void addString(char *s, const char *toAdd)
{
  strcat(s, toAdd);
}

void addComma(char* s)
{
  addChar(s, ',');
}

char* Node::GetSensorLine(byte index, char* line)
{
  SensorMetadata metadata;
  _sensors[index] -> GetMetadata(&metadata);

  char s[15];
  line[0] = '\0';
  addInt(line, metadata.Id);
  addComma(line);
  addString(line, getSensorTypeName(s, metadata.Type));
  addComma(line);
  addString(line, getMetricName(s, metadata.ValueMetric));
  addComma(line);
  addInt(line, metadata.Offset);
  addComma(line);
  addInt(line, metadata.Multiplier);
  addComma(line);
  addInt(line, metadata.Divider);
  addComma(line);
  addInt(line, metadata.Minimum);
  addComma(line);
  addInt(line, metadata.Maximum);
  addComma(line);
  addInt(line, metadata.Precision);
  addComma(line);
  addInt(line, metadata.Delay);
  
  return line;
}

static const char processor[] PROGMEM =
#if defined(__AVR_ATmega168__)
  "ATmega168";
#elif defined(__AVR_ATmega328P__)
  "ATmega328P";
#elif defined(__AVR_ATmega1280__)
  "ATmega1280";
#elif defined(__AVR_ATmega2560__)
  "ATmega2560";
#elif defined(__AVR_ATmega32U4__)
  "ATmega32U4";
#elif defined(__SAM3X8E__)
  "AT91SAM3X8E";
#else
  "Unknown";
#endif

char* Node::GetProcessorInfo(char* info)
{
  strcpy_P(info, processor);
  addChar(info, ' ');
  addInt(info, F_CPU/1000000);
  strcat_P(info, PSTR(" MHz") );
  return info;
}

char* Node::GetCompileInfo(char* info)
{
  strcpy_P(info, compiled);
  strcat_P(info, compile_date);
  strcat(info, "/");
  strcat_P(info, compile_time);
  return info;
}

boolean Node::Connect(char* mac, byte timeOut)
{ // TODO!
}

boolean Node::Ping(DateTime& now, int& newPingTime)
{ // TODO!
}

void Node::HandleCommand(char* s, Stream& io)
{
  CommandInfo info = parseCommand(s);
  switch(info.Cmd)
  {
    case slist:
      handleSlist(io, s);
      break;
    case sget:
      handleSget(io, s, info);
      break;
    case sconf:
      handleSconf(io, s, info);
      break;
    case alist:
      handleAlist(io, s);
      break;
  }
}

void Node::handleSconf(Stream& io, char* storage, CommandInfo& info)
{ // TODO!
  io.println("sconf!");
}

void Node::handleSget(Stream& io, char* storage, CommandInfo& info)
{
  byte id = atoi(info.Parameters[0]);
  ISensor* sensor = getSensorWithId(id);
  if(sensor != NULL)
  {
    DateTime dt = GetDateTime();
    int value = sensor -> GetValue();
    ShowDateTime(io, dt);
    io.print(",");
    io.println(value);
  }
}

ISensor* Node::getSensorWithId(byte id)
{
  for(byte i = 0; i < _sensorCount; i++)
  {
    SensorMetadata metadata;
    _sensors[i] -> GetMetadata(&metadata);
    if(metadata.Id == id)
      return _sensors[i];
  }
  return NULL;
}

void Node::handleSlist(Stream& io, char* storage)
{
  io.print("#"); io.println(_sensorCount);
  for(byte i = 0; i < _sensorCount; i++)
  {
    GetSensorLine(i, storage);
    io.println(storage);
  }
}

char* getActuatorTypeName(char* s, ActuatorType type)
{
  strcpy_P(s, actuatorTypeNames[type]);
  return s;
}

void Node::handleAlist(Stream& io, char* storage)
{
  io.print("#"); io.println(_actuatorCount);
  for(byte i = 0; i < _actuatorCount; i++)
  {
    IActuator* actuator = _actuators[i];
    io.print(actuator -> GetId());
    io.print(",");
    io.print(actuator -> GetName());
    io.print(",");
    io.println(getActuatorTypeName(storage, actuator -> GetType()));
  }
}

char* getSensorTypeName(char* s, SensorType type)
{
  strcpy_P(s, sensorTypeNames[type]);
  return s;
}

char* getMetricName(char *s, Metric metric)
{
  strcpy_P(s, metricNames[metric]);
  return s;
}

void Node::ShowDateTime(Stream &io, const DateTime& dt)
{
  char* slash = "/";
  char* semi = ":";
  io.print(dt.year());
  io.print(slash);
  io.print(dt.month());
  io.print(slash);
  io.print(dt.day());
  io.print(" ");
  io.print(dt.hour());
  io.print(semi);
  io.print(dt.minute());
  io.print(semi);
  io.print(dt.second());
}

