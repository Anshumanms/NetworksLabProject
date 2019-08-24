int sensorPin = 36; // select the input pin for LDR
int LDRPin1 = 35;
int LDRPin2 = 33;
int LDRPin3 = 32;

float sensorValue1 = 0.0,sensorValue2 = 0.0,sensorValue3 = 0.0,referValue = 0, reference=0.0, reading = 0.0, upperRange, lowerRange; // variable to store the value coming from the sensor
int iterations = 200, page = 1;
float interval = 0.1;
bool foundFlag = false;
void setup() {
    
    Serial.begin(115200); //sets serial port for communication
}
int page_number(){
    sensorValue1 = float(analogRead(LDRPin1))/4095; // read the value from the sensor
    sensorValue2 = float(analogRead(LDRPin2))/4095;
    sensorValue3 = float(analogRead(LDRPin3))/4095;
    reference = float(analogRead(sensorPin))/4095;
    upperRange = reference + interval;
    lowerRange = reference - interval;
    Serial.print(sensorValue1); //prints the values coming from the sensor on the screen
    Serial.print("\t");
    Serial.print(sensorValue2); //prints the values coming from the sensor on the screen
    Serial.print("\t");
    Serial.print(sensorValue3); //prints the values coming from the sensor on the screen
    Serial.print("\t");
    Serial.print("Reference : ");
    Serial.println(reference);

    
    
    if (sensorValue1 > lowerRange and sensorValue1 < upperRange){
      page = 2;
      foundFlag = true;
     }
    if (sensorValue2 > lowerRange and sensorValue2 < upperRange){
      page = 3;
      foundFlag = true;
      }
    if ( sensorValue3 > lowerRange and sensorValue3 < upperRange){

      page = 4;
      foundFlag = true;
    }
    foundFlag = false;

    return page;
  
}

void loop() {
    page  = page_number();
    Serial.print("Page : ");
    Serial.println(page);
    page = 1;
    delay(50);

}
