#include <Arduino.h>
#include <LD2450.h>
#include <SoftwareSerial.h>

#define RADAR_RX 10
#define RADAR_TX 11

SoftwareSerial radarSerial(RADAR_RX, RADAR_TX);
LD2450 ld2450;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  radarSerial.begin(256000);

  ld2450.begin(radarSerial);

  Serial.println("Initialized LD2450 Radar Sensor");
}

void loop()
{
  const int found_targets = ld2450.read();

  if (found_targets > 0)
  {
    Serial.print("Found ");
    Serial.print(found_targets);
    Serial.println(" targets:");
    for (int i = 0; i < found_targets; i++)
    {
      const LD2450::RadarTarget target = ld2450.getTarget(i);
      if (target.valid)
      {
        Serial.print("Target ");
        Serial.print(i);
        Serial.print(" - Distance: ");
        Serial.print(target.distance);
        Serial.println(" mm");
      }
    }
  }
  else
  {
    Serial.print(ld2450.getLastTargetMessage());
    static unsigned long lastMillis = 0;
    if (millis() - lastMillis > 2000)
    {
      Serial.println("No target found...");
      lastMillis = millis();
    }
  }

  delay(100);
}