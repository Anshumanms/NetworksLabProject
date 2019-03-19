
const int readpin=33;

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
  Serial.println(sensorValue);
  
  if (sensorValue < 1500 )Serial.println("LED1");
  else if (sensorValue >=1500 and sensorValue <= 2000)Serial.println("LED2");
  else if (sensorValue >=2000 and sensorValue <= 2600)Serial.println("LED3");
  else if (sensorValue >=2600 and sensorValue <= 3200)Serial.println("LED4");
  delay(150);        // delay in between reads for stability
}
