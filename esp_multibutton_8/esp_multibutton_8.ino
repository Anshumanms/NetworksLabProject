#include <LiquidCrystal.h>

const int readpin1=39,readpin2=34;
const int rs = 23, en =22 , d4 = 5, d5 = 18, d6 = 19, d7 = 21;
bool order[] = {false,false,false,false,false,false,false,false};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool order1=false,order2=false,order3=false,order4=false,order5=false,order6=false,order7=false,order8=false;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(readpin1,INPUT);
  pinMode(readpin2,INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hungry!");

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  ButtonReading1 = analogRead(readpin1);
  ButtonReading2 = analogRead(readpin2);
    
    if (ButtonReading1 < 1500 ){Serial.println("Button1"); order[0]=true;}
  else if (ButtonReading1 >=1700 and ButtonReading1 <= 2000){Serial.println("Button2");order[1]=true;}
  else if (ButtonReading1 >=2300 and ButtonReading1 <= 2600){Serial.println("Button3");order[2]=true;}
  else if (ButtonReading1 >=2600 and ButtonReading1 <= 3000){Serial.println("Button4");order[3]=true;}
    
    if (ButtonReading2 < 1000 ){Serial.println("Button5");order[4]=true;}
  else if (ButtonReading2 >=1700 and ButtonReading2 <= 2000){Serial.println("Button6");order[5]=true;}
  else if (ButtonReading2 >=2400 and ButtonReading2 <= 2750){Serial.println("Button7");order[6]=true;}
  else if (ButtonReading2 >=2750 and ButtonReading2 <= 3000){Serial.println("Button8");order[7]=true;}
  
  for (int index=0;index<8;index++){
    if (order[index] == true){
        lcd.clear();
        lcd.print("Order : ");
        lcd.print(index);
        delay(100);
      }
    
   }
   for (int index=0;index<8;index++){
      if (order[index]==true){
          order[index]=false;
        }
    }
   
  delay(100);        // delay in between reads for stability
}
