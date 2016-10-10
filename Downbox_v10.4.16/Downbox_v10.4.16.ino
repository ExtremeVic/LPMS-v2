#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
char messageFromPC2[numChars] = {0};
boolean newData = false;    // New Data FLag
int counter = 0;  // Counter to turn off laser circuit


void setup()   {
  Serial.begin(4800);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(13,LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(F("  Notice"));
  display.setTextSize(1);
  display.println("");
  display.println(F("Sensor needs ~15sec"));
  display.println(F("to fully calibrate."));
  display.println("");
  display.println("");
  display.println(F("   F/W v10.4.16"));
  display.display();
  delay(2500); 
}

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
        
    }

    /*else if (newData == false) {
              display.clearDisplay();
              display.setTextColor(WHITE);
              display.setTextSize(2);
              display.setCursor(0,0);
              display.println("  CHECK");
              display.println("CONNECTION");
              display.setTextSize(1);
              display.println("");
              display.println("");
              display.println(F("   F/W v10.4.16"));
              display.display();    
    }*/
    

  if (counter > 1800) {
     digitalWrite(8,HIGH);
     display.println("");
     display.print("AUTO LASER OFF");

  }
    
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }

  while (Serial.available() == 0) {
              delay(100); // pause
              if (Serial.available() == 0) { // Check Again, get rid of millisecond drops
                display.clearDisplay();
                display.setTextColor(WHITE);
                display.setTextSize(2);
                display.setCursor(0,0);
                display.println("  CHECK");
                display.println("CONNECTION");
                display.setTextSize(1);
                display.println("");
                display.println("");
                display.println(F("   F/W v10.4.16"));
                display.display();    
              }

  }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    strcpy(messageFromPC2, strtokIndx); // copy it to messageFromPC

}

//============

void showParsedData() {

    
    if (strcmp(messageFromPC,messageFromPC2) == 0){
    //Serial Readout    
    /*Serial.print("Equal ");
    Serial.print(messageFromPC);
    Serial.print(" / ");
    Serial.print(messageFromPC2);
    Serial.println();
    */


    //Display Readout
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("   ANGLE "); 
    display.setTextSize(3);
    display.print(" "); 
    if (messageFromPC[0] != '-') {
      display.print("+");  
      }  
    display.print(messageFromPC);
    display.println("");
    display.setTextSize(1);
    display.println("");
    display.println(" Laser off in 2 mins");
    display.display();
    
    }

    else{
    //Serial Readout    
    /*Serial.print("UnEqual ");
    Serial.print(messageFromPC);
    Serial.print(" / ");
    Serial.print(messageFromPC2);
    Serial.println();
    */
    }


}
