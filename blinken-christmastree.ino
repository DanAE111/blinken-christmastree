#include <WS2812FX.h>

#define LED_COUNT 121
#define LED_PIN 13

#define TIMER_MS 600000

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

// Variable init
unsigned long lastEffectSwap = 0;
unsigned long now = 0;
byte fx = 0;

// Effects to cycle through
int effectList[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55};
// Removed 1, 29, 26, 27, 28 they are all annoying strobing and flASHING EFFECTS.
byte numEffects = 51; // Total number of effects
byte brightness = 96; // Brightness of LEDs

void setup() {
  // Init serial
  Serial.begin(115200);

  // Seed random number generator
  randomSeed(analogRead(0));

  // Shuffle effects list
  shuffleList(effectList, numEffects);

  // Initialise lights
  ws2812fx.init();
  ws2812fx.setBrightness(brightness);
  ws2812fx.setSpeed(randomSpeed());
  ws2812fx.setColor(randomColour());
  ws2812fx.setMode(effectList[0]);
  ws2812fx.start();
}

void loop() {
  now = millis();
  ws2812fx.service();
  if(now - lastEffectSwap > TIMER_MS) {
    // Cycle to next effect
    if (fx > numEffects) {
      fx = 0;
    }
    ws2812fx.setMode(effectList[fx]);
    fx++;
    // New random colour
    ws2812fx.setColor(randomColour());
    // New random speed
    ws2812fx.setSpeed(randomSpeed());
    lastEffectSwap = now;
    // Print debug text to serial console
    Serial.print("Effect: ");
    Serial.print(ws2812fx.getMode());
    Serial.print(", Colour: ");
    Serial.print(ws2812fx.getColor());
    Serial.print(", Speed: ");
    Serial.println(ws2812fx.getSpeed());
  }
}

unsigned long randomColour() {
  unsigned long colour = 0x0;
  colour = random(0x0, 0xFFFFFF);
  return colour;
}

unsigned int randomSpeed() {
  unsigned int speed = 0;
  speed = random(1, 1001);
  return speed;
}

// Fisher-Yates shuffle
void shuffleList (int * t, int n) {
  while (--n >= 2) {
    int k = random (n);
    int temp = t [n];
    t [n] = t [k];
    t [k] = temp;
  }
}