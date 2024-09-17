#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"


Adafruit_SSD1306 display = Adafruit_SSD1306(128,64,&Wire);

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC,0x3c);

}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(0,16);
  display.print("TEXT");

  display.display();


}
