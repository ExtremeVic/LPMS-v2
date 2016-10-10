#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4);


//byte incomingByte;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int counter = 0;


void setup()   {
  Serial.begin(4800);
  inputString.reserve(200);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(13,LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(F("Notice:"));
  display.setTextSize(1);
  display.println("");
  display.println(F("Sensor requires 30sec"));
  display.println(F("after 1st angle shown"));
  display.println(F("to fully calibrate."));
  display.println(F("v10.3.16 VA"));
  display.display();
  delay(2500); 
}


void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    digitalWrite(A2,HIGH);   //PinA2 to high
    Serial.println(inputString);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("  ANGLE "); 
    Serial.println(inputString.toInt() * 0.1 > 0);
    if (inputString.toInt() * 0.1 > 0) {
      display.print("+");    
    }
    display.setTextSize(3);
    display.print(" "); 
    display.print(inputString.toInt()*0.1);
     
    display.display();
    //delay(100);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  if (counter > 1800) {
    digitalWrite(8,HIGH);
  }
   
  else if (Serial.available()==0) {
    //ERROR WARNING
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("  CHECK");
  display.println("CONNECTION");
  display.display();
  }


 display.println("");
 display.print("Laser off in: ");
 display.print((1800-counter)*0.1,1);
 display.println("s");

 counter++;

  
}

/*
 SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent() {
  inputString = "";
  while (Serial.available()>= 5) {
    
    char inChar = (char)Serial.read(); // get the new byte:
    inputString += inChar;  // add it to the inputString:
    
    // if the incoming character is a newline, set a flag
    if (inChar == '\n'  ){ 
      stringComplete = true;
      
    }

   }
}


