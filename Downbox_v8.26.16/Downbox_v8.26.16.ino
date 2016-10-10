





/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/


#define CH1 8
#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <gfxfont.h>
#include <Adafruit_SSD1306.h>



//#define OLED_RESET 4
Adafruit_SSD1306 display(4);

//byte incomingByte;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete    
//int hold = 0;
unsigned int result = 0;
unsigned int counter = 0;

void setup()   {    
  
          
  Serial.begin(4800);
  inputString.reserve(100);
  pinMode(CH1,OUTPUT);
  digitalWrite(CH1,LOW);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // logo display for 5 seconds
  //display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(" Firmware: v8.26.16");
  display.display();
  delay(10000); 
}


void loop() {
  
  
   delay(2);

    if (Serial.available()==0) {
      if (hold >= 50 && hold < 650 ) {
    // display error message when no serial data detected
    digitalWrite(A2,HIGH);
    counter = 0;
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(" No Sensor Detected!");
    display.println("");
    display.println("Check all connections");
    display.println("");
    display.println("Voltage must be above");
    display.println("4.7 VDC to function!");
    display.println("");
    display.print("Current voltage: ");
    display.println(readVcc()*0.001);
    display.display();
    delay(10);
      }
      else if (hold >= 650 && hold < 1000) {
        display.clearDisplay();

        display.display();
      }
      else if (hold >= 1000 && hold < 1650) {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(0,0);
        display.println("Notice:");
        display.setTextSize(1);
        display.println("");
        display.println("Sensor requires 30sec");
        display.println("after 1st angle shown");
        display.println("to fully calibrate.");
        display.println("");
        display.println(" www.8thdaysound.com");
        display.display();
      }
      else if (hold >= 1650) {
        hold = 50;
      }
   }

  
    else if (stringComplete) {
      //Serial.println(inputString);
      hold = 0;
      // text display setup
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      // display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.setTextSize(1);
      display.println("   Eighth Day Sound");
      display.println("");
      /*if (readVcc()*0.001 < 4.75){
        counter = 0;
        display.setTextSize(2);
        display.println("Low Voltage");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println("Replace Batteries or swit");
        display.println("");
        display.setTextSize(2);
        display.print("Tilt "); 
        display.println(inputString.toInt()*0.1);
        display.setTextSize(1);
      }
      else if (readVcc()*0.001 >= 4.75){*/
        display.println("");
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.print("Tilt:");
        if (inputString.toInt()*0.1>0) {
        display.print("+");
        }
        else {
          
        }
        display.print(inputString.toInt()*0.1,1);
        display.println(""); 
        display.setTextSize(1);
        
      //} 
      if (counter > 1800) {
        digitalWrite(CH1,HIGH);
        display.println("");
        display.println("   AUTO LASER OFF"); 
        display.println("Cycle Power to Reset!");
        
      }
      else {
        digitalWrite(CH1,LOW);
        display.println("");
        display.print("Laser off in: ");
        display.print((1800-counter)*0.1,1);
        display.println("s");
        display.println(" www.8thdaysound.com");
        counter++;
      }
      
      display.display();
      // clear the string:
        inputString = "";
        stringComplete = false;
        //Serial.flush();
  }
   hold++;
   
   
   
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

