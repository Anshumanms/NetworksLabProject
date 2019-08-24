#include "SevSeg.h"
SevSeg sevseg; 

void setup(){
    byte numDigits = 6;
    byte digitPins[] = {2,5,4,18,19,23};
    byte segmentPins[] = {15,13,12,14,27,26,25};//A,B,C,D,E,F,G
    bool resistorsOnSegments = true;

    byte hardwareConfig = COMMON_ANODE; 
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    sevseg.setBrightness(100);
}

void loop(){
          sevseg.setNumber(631400);
          sevseg.refreshDisplay();
          //delay(10);
}
