#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ArduinoHA.h>

#include "arduino_secrets.h"

const uint8_t NEO_PIXEL_STRIP_PIN = 0;
const uint16_t NEO_PIXEL_STRIP_LENGTH = 7;

Adafruit_NeoPixel neoPixelStrip(NEO_PIXEL_STRIP_LENGTH, NEO_PIXEL_STRIP_PIN, NEO_GRB + NEO_KHZ800);
elapsedMillis timeSinceUpdate;
elapsedMillis timeSinceActive;
uint32_t timeBetweenUpdates = 0;
bool isOn = false;
bool isActive = false;

#define BROKER_ADDR IPAddress(192,168,178,179)
byte macAddress[6];

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
HASwitch thunderSwitch("thunderSwitch");

void onSwitchCommand(bool in_state, HASwitch* out_sender)
{
    isActive = in_state;
    out_sender->setState(in_state); // report state back to Home Assistant
}

void setup()
{
  Serial.begin(9600);

  neoPixelStrip.begin();
  neoPixelStrip.fill(0x000000, 0, NEO_PIXEL_STRIP_LENGTH);
  neoPixelStrip.show();

  randomSeed(analogRead(A0));
  timeBetweenUpdates = random(0, 1000);
  timeSinceUpdate = 0;

  Serial.println("SETUP - BEGIN");

  WiFi.macAddress(macAddress);
  device.setUniqueId(macAddress, sizeof(macAddress));
  device.setName("neoThunderstorm");
  device.setSoftwareVersion("0.1.1");

  thunderSwitch.setName("Thunder Switch");
  thunderSwitch.onCommand(onSwitchCommand);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");

  mqtt.begin(BROKER_ADDR, 1883, "homeassistant", MQTT_PASSWORD);

  Serial.println("SETUP - END");
}

void loop()
{
  mqtt.loop();

  if (isActive && timeSinceUpdate >= timeBetweenUpdates)
  {
    if (isOn)
    {
      neoPixelStrip.fill(0x000000, 0, NEO_PIXEL_STRIP_LENGTH);
      neoPixelStrip.show();
      timeBetweenUpdates = random(0, 1000);
    }
    else
    {
      neoPixelStrip.fill(0xffffff, 0, NEO_PIXEL_STRIP_LENGTH);
      neoPixelStrip.show();
      timeBetweenUpdates = random(10, 100);
    }

    isOn = not isOn;
    timeSinceUpdate = 0;
  }
  else if (not isActive && isOn)
  {
    neoPixelStrip.fill(0x000000, 0, NEO_PIXEL_STRIP_LENGTH);
    neoPixelStrip.show();

    isOn = false;
  }
}
