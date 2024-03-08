#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>

const uint8_t NEO_PIXEL_STRIP_PIN = 0;
const uint16_t NEO_PIXEL_STRIP_LENGTH = 7;

Adafruit_NeoPixel neoPixelStrip(NEO_PIXEL_STRIP_LENGTH, NEO_PIXEL_STRIP_PIN, NEO_GRB + NEO_KHZ800);
elapsedMillis timeSinceUpdate;
uint32_t timeBetweenUpdates = 0;
bool isOn = false;

void setup()
{
  neoPixelStrip.begin();
  neoPixelStrip.fill(0x000000, 0, NEO_PIXEL_STRIP_LENGTH);
  neoPixelStrip.show();

  randomSeed(analogRead(A0));
  timeBetweenUpdates = random(0, 1000);
  timeSinceUpdate = 0;
}

void loop()
{
  if (timeSinceUpdate >= timeBetweenUpdates)
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
}
