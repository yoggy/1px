//
// ILI9325_1px.ino - for ILI9325 LCD display module.
//
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);

int frame_count = 0;
uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0x0000};

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("1px - ILI9325 version"));

  tft.reset();

  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  Serial.print("identifier=");
  Serial.println(identifier, HEX);

  tft.setRotation(3); // 0-3
  tft.fillScreen(0xFFFF);
  tft.fillRect(0, 0, 200, 200, 0x0000);
}

void loop(void) {
  tft.drawPixel(100, 100, colors[frame_count % 4]);
  delay(250);
  frame_count ++;
}

