#include "SevSeg.h"
SevSeg sevseg; 

void setup(){
    byte numDigits = 1;
    byte digitPins[] = {};
    byte segmentPins[] = {3,5,6,9,10,11,12};
    bool resistorsOnSegments = true;

    byte hardwareConfig = COMMON_ANODE; 
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    sevseg.setBrightness(100);
}

void loop(){
//        for(int i=0;i<10;i++){
//          sevseg.setNumber(i);  
//          sevseg.refreshDisplay();
//          delay(2);
//          }
          sevseg.setNumber(5);
          sevseg.refreshDisplay();
          //delay(2);
          //while(1){};
        
        
}
