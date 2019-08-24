
//----------------------------Include Libraries-------------------------------------------------------------------------
#include "SevSeg.h"
#include <LiquidCrystal_I2C.h>
#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <elapsedMillis.h>




//----------------------------Define Constants-------------------------------------------------------------------------

#define EAP_IDENTITY "jtm182004" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "nigoltatii@63" //your Eduroam password

//---------------------------------------------------------------------------------------------------------------------
float interval = 0.3;
bool foundFlag = false;

byte numDigits = 6;
byte digitPins[] = {2,5,4,18,19,23};
byte segmentPins[] = {15,13,12,14,27,26,25};//A,B,C,D,E,F,G
byte hardwareConfig = COMMON_ANODE; 

const int readpin1 = 39,readpin2 = 34;// readpin3 = 35;//, resetpin =32, billpin = 33 ;
const int sensorPin = 36, LDRPin1 = 35, LDRPin2 = 33, LDRPin3 = 32;
const char* ssid = "IITD_WIFI"; // Eduroam SSID
const char* host = "10.208.67.44"; //external server domain for HTTP connection after authentification
const uint16_t port = 8090;
const int sample_interval= 500,status_interval = 8000;

bool order[] = {false,false,false,false,false,false,false,false,false,false};
bool resistorsOnSegments = true;
int quant_items[] = {0,0,0,0,0,0,0};
int ButtonReading1,ButtonReading2,ButtonReading3;
int counter = 0, page = 1,count = 0;
int last_sample_at = 0, total_items_now = 0;
int lcdColumns = 20;
int lcdRows = 4;

float sensorValue1 = 0.0,sensorValue2 = 0.0,sensorValue3 = 0.0,referValue = 0, reference=0.0, reading = 0.0, upperRange, lowerRange; // variable to store the value coming from the sensor
String table_id = "T1";
String empty = "                    ";
String sevOut = "000000";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
SevSeg sevseg;
WiFiClient client;
elapsedMillis timeElapsed;

//----------------------------setup------------------------------------------------------------------------------------

void setup() {
  
  //Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hungry!Please, Wait.");
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(100);

  
  pinMode(readpin1,INPUT);
  pinMode(readpin2,INPUT);
  //pinMode(readpin3,INPUT);
          
  // set up the LCD's number of columns and rows:
  //Serial.println();
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  //Serial.print("Connecting to network: ");
  //Serial.println(ssid);
  
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  
  WiFi.begin(ssid); //connect to wifi
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  
  //Serial.println("");
  //lcd.setCursor(0,1);
  //lcd.print("                    ");
  //lcd.setCursor(0,1);
  //lcd.print("WiFi connected!");
  //Serial.println("WiFi connected");
  //Serial.println("IP address set: "); 
  //Serial.println(WiFi.localIP()); //print LAN IP
  while (!client.connect(host, port)) {
    //Serial.println("Connection to host failed");
    delay(1000);
  }
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Ready for order!");
  delay(2000);
  timeElapsed = 0;
  //client.stop();

}
//----------------------------loop---------------------------------------------------------------------------------------------------------
int page_number(){
    sensorValue1 = float(analogRead(LDRPin1))/1023; // read the value from the sensor
    sensorValue2 = float(analogRead(LDRPin2))/1023;
    sensorValue3 = float(analogRead(LDRPin3))/1023;
    reference = float(analogRead(sensorPin))/1023;
    upperRange = reference + interval;
    lowerRange = reference - interval;
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
void seven_segment_update(){
  
     String sevOut = "";
     for (int i=1;i<=6;i++){
      sevOut = sevOut + quant_items[i];
      }
     sevseg.setNumber(sevOut.toInt());
     sevseg.refreshDisplay();
  
  }
void loop() {
     page = 1;
     page = page_number();
     seven_segment_update();

    if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network

        if (client.connected()){
                  sevseg.setNumber(931400);
                  sevseg.refreshDisplay();

                  if(timeElapsed > status_interval){
                    client.print(table_id+",getstatus");
                    timeElapsed = 0;
                  }
                  while(client.available()) {

                      String line = client.readStringUntil('\f');
                      
                      if (line.indexOf("TI:")>=0){
                          lcd.setCursor(0,3);
                          lcd.print(line);
                        }
                      else if (line.indexOf("Bill Amount")>=0){
                        lcd.setCursor(0,1);
                        lcd.print(empty);
                        lcd.setCursor(0,1);
                        lcd.print(line);
                        }
                      else{
                          ////Serial.println("in else");
                          lcd.setCursor(0,0);
                          lcd.print(empty);
                          lcd.setCursor(0,0);
                          lcd.print(line);
                          delay(1000);
                          lcd.setCursor(0,0);
                          lcd.print(empty);
                          //Serial.println(line);
                        }
                  }
                  lcd.setCursor(17,3);
                  lcd.print("P:");
                  lcd.print(page);
                  //page = 1;
            if ((millis() - last_sample_at) > sample_interval){

                  last_sample_at = millis();
                  ButtonReading1 = analogRead(readpin1);
                  ButtonReading2 = analogRead(readpin2);
                  //Serial.print(ButtonReading1);
                  //Serial.print("\t");
                  //Serial.println(ButtonReading2);
                  
                  //ButtonReading3 = analogRead(readpin3);
                       if (ButtonReading1 < 1500 ){Serial.println("Button1"); order[1]=true;}
                  else if (ButtonReading1 >=1700 and ButtonReading1 <= 2000){Serial.println("Button2");order[2]=true;}
                  else if (ButtonReading1 >=2300 and ButtonReading1 <= 2600){Serial.println("Button3");order[3]=true;}
                  else if (ButtonReading1 >=2600 and ButtonReading1 <= 3000){Serial.println("Button4");order[4]=true;}
                    
                       if (ButtonReading2 < 1000 ){Serial.println("Button5");order[5]=true;}
                  else if (ButtonReading2 >=1700 and ButtonReading2 <= 2000){Serial.println("Button6");order[6]=true;}
                  else if (ButtonReading2 >=2450 and ButtonReading2 <= 2750){Serial.println("Button7");order[7]=true;}
                  else if (ButtonReading2 >2750 and ButtonReading2 <= 2900){Serial.println("Button8");order[8]=true;}
                  else if (ButtonReading2 >2900 and ButtonReading2 <= 3200){Serial.println("Button9");order[9]=true;}
                  
                  for(int i=1; i <= 9; i++){
                    
                    if (order[i] == true){
                          order[i] = false;
                          if(i>=1 and i<=6){
                              quant_items[i] = quant_items[i] + 1;
                              client.print(table_id+","+"P"+page+","+"N"+i);
                              lcd.setCursor(0,2);
                              lcd.print("ITEM ");
                              lcd.print(i);
                              lcd.print(" picked.");
                              delay(1000);
                              lcd.setCursor(0,2);
                              lcd.print("                   ");
                          
                          }
                           if (i==7){ //confirm case
                              for (int i=1;i <=6; i ++){
                                  quant_items[i] = 0;
                               }
                              client.print(table_id+",reset");
                              lcd.setCursor(0,1);
                              lcd.print(empty);
                              lcd.setCursor(0,2);
                              lcd.print("Reset               ");
                              delay(1000);
                              lcd.setCursor(0,2);
                              lcd.print("                   ");
                            
                            }
                            if (i==8){ //bill case
                              client.print(table_id+",confirm");
                              lcd.setCursor(0,2);
                              lcd.print("Confirm             ");
                              delay(1000);
                              lcd.setCursor(0,2);
                              lcd.print("                   ");
    
                              
                            }
                            if (i==9){ //reset case
                              for (int i=1;i <=6; i ++){
                                quant_items[i] = 0;
                                }
                              client.print(table_id+",bill");
                              lcd.setCursor(0,1);
                              lcd.print(empty);
                              lcd.setCursor(0,2);
                              lcd.print("Bill                ");
                              delay(1000);
                              lcd.setCursor(0,2);
                              lcd.print("                   ");
                            }
                     }
                    
                    }
//                   for (int index=0;index<9;index++){
//                      if (order[index]==true){
//                        order[index]=false;
//                      }
//                   }

                  

              }
            //client.stop();
        }
        else {
            //client.stop();
            while (!client.connect(host, port)) {
              lcd.setCursor(0,0);
              lcd.print("Connection failed. ");
              delay(2000);
              lcd.setCursor(0,0);
              lcd.print(empty);
              //Serial.println("Connection to host failed");
              delay(1000);
            }       
        }
    }
    
    else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
        //lcd.setCursor(0,0);
        //lcd.print("Wi-Fi Disconnected!");
        //Serial.println("Wi-Fi Disconnected!");
        WiFi.begin(ssid);      
    }
}
