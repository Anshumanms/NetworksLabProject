
const int input= 3,readpin=33;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(readpin,INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(readpin);
  // print out the value you read:
  //Serial.println(sensorValue);
  if (input == 5){
  if (sensorValue < 400 )Serial.println("LED1");
  else if (sensorValue >=500 and sensorValue <= 600)Serial.println("LED2");
  else if (sensorValue >=600 and sensorValue <= 700)Serial.println("LED3");
  else if (sensorValue >=700 and sensorValue <= 800)Serial.println("LED4");
  }
  else {
  if (sensorValue < 300 )Serial.println("LED1");
  else if (sensorValue >=300 and sensorValue <= 400)Serial.println("LED2");
  else if (sensorValue >=400 and sensorValue <= 480)Serial.println("LED3");
  else if (sensorValue >=480 and sensorValue <= 550)Serial.println("LED4");
  }
  
  delay(150);        // delay in between reads for stability
}
