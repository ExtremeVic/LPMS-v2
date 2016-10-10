#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);
void setup()
{
digitalWrite(8,LOW);
  digitalWrite(13,LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
}
void loop(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(4);
 for (int i=0; i< 255; i++)
 {
   display.setCursor(0,0);
   display.print(i);
   display.print(" ");
   display.print((char)i);
   
   display.display();
   delay(250);
   display.clearDisplay();
 }
  }
