#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SparkFun_APDS9960.h>
#include <DHT.h>
#include <RFM69.h>
#include <Adafruit_NeoPixel.h>

#include "Sensor_DHT.h"
#include "Sensor_Gesture.h"
#include "Sensor_Voltage.h"
#include "Sensor_Memory.h"
#include "CommandParser.h"
#include "RadioStream.h"
#include "LinkedList.h"
#include "UpdateStats.h"
#include "Node.h"

#define NODEID          2  //unique for each node on same network
#define NETWORKID       1  //the same on all nodes that talk to each other
#define GATEWAYID       1
#define FREQUENCY       RF69_433MHZ
#define ENCRYPTKEY      "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!

#define NUM_LEDS        1
#define WS2811_PIN      6
#define DHT_PIN         4
#define APDS_INTERRUPT  0    // TODO: interrupt collides with RFM69?
#define APDS_PIN        2
#define VOLTAGE_PIN     A1

Sensor_DHT       dht(DHT_PIN);
Sensor_APDS9960  gesture(APDS_INTERRUPT, APDS_PIN);
Sensor_Voltage   voltage(VOLTAGE_PIN);
Sensor_Memory    memory;
Adafruit_NeoPixel led(NUM_LEDS, WS2811_PIN);
Node node(NODEID);
RadioStream radio;
LinkedList<UpdateStats*> updateStats;

ISensor* sensors[] =
{
    dht.GetTemperatureSensor(1),
    dht.GetHumiditySensor(2),
    gesture.GetGestureSensor(3),
    gesture.GetLightSensor(4),
    gesture.GetProximitySensor(5),
    voltage.GetSensor(6),
    memory.GetFree(7),
    memory.GetHeap(8),
    memory.GetStack(9),
};

void setup()
{
  char s[30];
  Serial.begin(9600);

  Serial.println(GetFreeRam());
  Serial.println(node.GetProcessorInfo(s));
  Serial.println(node.GetCompileInfo(s));

  node.SetSensors(sensors, sizeof(sensors)/sizeof(sensors[0]));
  node.Initialize();

  byte id = 0;
  addUpdateStats(updateStats, "above:100|time:2000&time:25000", id);

  for(int i = 0; i < updateStats.size(); i++)
  {
    UpdateStats* stats = (*updateStats.get(i));
    Serial.print("Expression: "); Serial.println(stats->ExpressionId);
    Serial.print("SensorId: "); Serial.println(stats->SensorId);
    Serial.print("LastUpdateMillis: "); Serial.println(stats->LastUpdateMillis);
    Serial.print("LastUpdateValue: "); Serial.println(stats->LastUpdateValue);

    for(int j = 0; j < stats->UpdateInfoCount; j++)
    {
      sprintf(s, "  %d=%d", (stats->UpdateInfos[j]).type, (stats->UpdateInfos[j]).value);
      Serial.println(s);
    }
  }

  radio.begin();
  led.begin();
}

void loop()
{
  char s[60];
  byte num = node.GetSensorCount();

  if(radio.available() > 0)
  {
    strcpy(s, radio.readString().c_str());
    node.HandleCommand(s, radio);
  }

  for(byte i = 0; i < num; i++)
  {
    SensorMetadata metadata;
    ISensor* sensor = node.GetSensor(i);
    sensor -> GetMetadata(&metadata);

    radio.print(metadata.Id); radio.print(F(": "));
    radio.print(sensor -> GetValue()); radio.print(" ");
    radio.println(getMetricName(s, metadata.ValueMetric));
  }

  node.ShowDateTime(radio, node.GetDateTime());
  radio.println();
  delay(1000);
}

