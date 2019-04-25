
//----------------------------Include Libraries-------------------------------------------------------------------------

#include <LiquidCrystal.h>
#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

//----------------------------Define Constants-------------------------------------------------------------------------

#define EAP_IDENTITY "jtm182004" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "nigoltatii@63" //your Eduroam password

//----------------------------Declare Variables-------------------------------------------------------------------------

const char* ssid = "IITD_WIFI"; // Eduroam SSID
const char* host = "10.208.67.44"; //external server domain for HTTP connection after authentification
const uint16_t port = 8090;
int counter = 0;
int count = 0;
const int interval= 500;
int last_sample_at = 0;
const int readpin1 = 39,readpin2 = 34, confirmpin = 35, resetpin =32, billpin = 33 ;
String table_id = "T1", page_id1 = "P1", page_id2 = "P2";
String item_id1 = "I1", item_id2 = "I2";
const int rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool order[] = {false,false,false,false,false,false,false,false};

int Order1S,Order2S,ConfirmS,ResetS, BillS;
WiFiClient client;

//----------------------------setup------------------------------------------------------------------------------------

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  lcd.begin(20, 4);
  //lcd.autoscroll();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Hungry!");
  lcd.setCursor(0,1);
  lcd.print("Please, Wait...");
  
  pinMode(readpin1,INPUT);
  pinMode(readpin2,INPUT);
  pinMode(confirmpin,INPUT);
  pinMode(resetpin,INPUT);
  pinMode(billpin,INPUT);
        
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
    delay(1000);
  }
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Ready for order!");

}
//----------------------------loop---------------------------------------------------------------------------------------------------------

void loop() {
    if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network
        if (client.connected()){
                  Order1S = digitalRead(readpin1);
                  Order2S = digitalRead(readpin2);
                  ConfirmS = digitalRead(confirmpin);
                  ResetS = digitalRead(resetpin);
                  BillS = digitalRead(billpin);      
            if ((millis() - last_sample_at) > interval){
                  last_sample_at = millis();
                  Serial.print(Order1S);
                  Serial.print("\t");
                  Serial.print(Order2S);
                  Serial.print("\t");
                  Serial.print(ConfirmS);
                  Serial.print("\t");
                  Serial.print(ResetS);
                  Serial.print("\t");
                  Serial.print(BillS);
                  Serial.println();
                  if (Order1S ==1){
                    lcd.setCursor(0,2);
                    lcd.print("ITEM 1              ");
                    client.print(table_id+","+page_id1+","+item_id1);
                  }
                  if (Order2S == 1){
                    lcd.setCursor(0,2);
                    lcd.print("ITEM 2              ");
                    client.print(table_id+","+page_id2+","+item_id1);
                  }
                  if (ConfirmS == 1){
                    lcd.clear();
                    lcd.setCursor(0,2);
                    lcd.print("Confirm             ");
                    client.print("confirm");
                  }
                  if (ResetS == 1){
                    lcd.setCursor(0,2);
                    lcd.print("Reset               ");
                    client.print("reset");
                  }
                  if (BillS == 1){
                    lcd.setCursor(0,2);
                    lcd.print("Bill Request        ");
                    client.print(table_id+","+"bill");
                  }
            }
        }
        else {
            while (!client.connect(host, port)) {
              Serial.println("Connection to host failed");
              delay(1000);
            }       
        }
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
  
    delay(50);
}
