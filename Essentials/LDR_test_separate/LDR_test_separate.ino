int sensorPin = A0; // select the input pin for LDR
int LDRPin1 = A3;
int LDRPin2 = A2;
int LDRPin3 = A1;

float sensorValue1 = 0.0,sensorValue2 = 0.0,sensorValue3 = 0.0,referValue = 0, reference=0.0, reading = 0.0, upperRange, lowerRange; // variable to store the value coming from the sensor
int iterations = 200, page = 1;
float interval = 0.1;
bool foundFlag = false;
void setup() {
    
    Serial.begin(9600); //sets serial port for communication
}
void loop() {
    //int here = millis();
    sensorValue1 = float(analogRead(LDRPin1))/1023; // read the value from the sensor
    sensorValue2 = float(analogRead(LDRPin2))/1023;
    sensorValue3 = float(analogRead(LDRPin3))/1023;
    reference = float(analogRead(sensorPin))/1023;
    upperRange = reference + interval;
    lowerRange = reference - interval;
//    Serial.print(sensorValue1); //prints the values coming from the sensor on the screen
//    Serial.print("\t");
//    Serial.print(sensorValue2); //prints the values coming from the sensor on the screen
//    Serial.print("\t");
//    Serial.print(sensorValue3); //prints the values coming from the sensor on the screen
//    Serial.print("\t");
//    Serial.print("Reference : ");
//    Serial.println(reference);
    
    if (sensorValue1 > lowerRange and sensorValue1 < upperRange){
      page = 2;
      foundFlag = true;
     }
    if (sensorValue2 > lowerRange and sensorValue2 < upperRange){
      //Serial.println("Page 2");
      page = 3;
      foundFlag = true;
      }
    if ( sensorValue3 > lowerRange and sensorValue3 < upperRange){
      //Serial.println("Page 3");
      page = 4;
      foundFlag = true;
    }
    
      
      
    //Serial.println(foundFlag);  
    if (foundFlag == true){
        Serial.print("Page : ");
        Serial.println(page);
        //Serial.print("");
        //Serial.println("");

        page = 1;
      }
    else {
        Serial.print("Page : ");
        Serial.println(page);
        //Serial.print("");
        //Serial.println("");
      }
    foundFlag = false;
    //Serial.println(millis()-here);
    //delay(500);

}
