#include <LiquidCrystal.h>
#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

#define EAP_IDENTITY "jtm182004" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "nigoltatii@63" //your Eduroam password

const char* ssid = "IITD_WIFI"; // Eduroam SSID
const char* host = "10.208.67.44"; //external server domain for HTTP connection after authentification
const uint16_t port = 8090;
int counter = 0;

const int readpin1 = 34,readpin2 = 35;

const int rs = 23, en =22 , d4 = 5, d5 = 18, d6 = 19, d7 = 21;
bool order[] = {false,false,false,false,false,false,false,false};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool order1=false,order2=false,order3=false,order4=false,order5=false,order6=false,order7=false,order8=false;
int ButtonReading1,ButtonReading2;
// the setup routine runs once when you press reset:

WiFiClient client;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  lcd.begin(16, 2);
  //lcd.autoscroll();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Hungry!");
  lcd.setCursor(0,1);
  lcd.print("Please, Wait...");
  
  pinMode(readpin1,INPUT);
  pinMode(readpin2,INPUT);
  
  // set up the LCD's number of columns and rows:
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
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
    Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
  while (!client.connect(host, port)) {
    Serial.println("Connection to host failed");
//    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print("Network Lost.");
//    lcd.setCursor(0,1);
//    lcd.print("Reconnecting...");
//    delay(1000);
//    //return;
  }
  lcd.clear();  
  lcd.setCursor(0,1);
  lcd.print("Ready for order!");




}

// the loop routine runs over and over again forever:
void loop() {
    if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network
      counter = 0; //reset counter

      ButtonReading1 = analogRead(readpin1);
      ButtonReading2 = analogRead(readpin2);
      Serial.print(ButtonReading1);
      Serial.print("\t");
      Serial.print(ButtonReading2);
      Serial.println();
        
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
            client.print(index);
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
    else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
      Serial.println("W-Fi Disconnected!");
      WiFi.begin(ssid);      
    }
  
    while (WiFi.status() != WL_CONNECTED) { //during lost connection, print dots
      delay(500);
      Serial.print(".");
      counter++;
      if(counter>=60){ //30 seconds timeout - reset board
        ESP.restart();
      }
    }
  
}
