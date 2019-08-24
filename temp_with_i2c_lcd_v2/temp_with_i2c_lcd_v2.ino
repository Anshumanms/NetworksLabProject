
//----------------------------Include Libraries-------------------------------------------------------------------------

#include <LiquidCrystal_I2C.h>
#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <elapsedMillis.h>
elapsedMillis timeElapsed;

//----------------------------Define Constants-------------------------------------------------------------------------

#define EAP_IDENTITY "jtm182004" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "nigoltatii@63" //your Eduroam password

//----------------------------Declare Variables-------------------------------------------------------------------------
// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

const char* ssid = "IITD_WIFI"; // Eduroam SSID
const char* host = "10.208.67.44"; //external server domain for HTTP connection after authentification
const uint16_t port = 8090;
int counter = 0, page = 1,count = 0;
const int sample_interval= 500,status_interval = 10000;
int last_sample_at = 0, total_items_now = 0;
const int readpin1 = 39,readpin2 = 34, confirmpin = 35, resetpin =32, billpin = 33 ;

String table_id = "T1", page_id1 = "P1", page_id2 = "P2";
String item_no1 = "N1", item_no2 = "N2",item_no3 = "N3";

const int rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;
int total_amount=0;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
bool order[] = {false,false,false,false,false,false,false,false};
String empty = "                    ";
int Order1S,Order2S,ConfirmS,ResetS, BillS;
WiFiClient client;

//----------------------------setup------------------------------------------------------------------------------------

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  //lcd.autoscroll();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Hungry!Please, Wait.");
  pinMode(readpin1,INPUT);
  pinMode(readpin2,INPUT);
  pinMode(confirmpin,INPUT);
  pinMode(resetpin,INPUT);
  pinMode(billpin,INPUT);
        
  // set up the LCD's number of columns and rows:
  Serial.println();
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
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
  //lcd.setCursor(0,1);
  //lcd.print("                    ");
  //lcd.setCursor(0,1);
  //lcd.print("WiFi connected!");
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
  delay(2000);
  timeElapsed = 0;
  //client.stop();

}
//----------------------------loop---------------------------------------------------------------------------------------------------------

void loop() {
    if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network
        if (client.connected()){
                  if(timeElapsed > status_interval){
                    client.print(table_id+",getstatus");
                    timeElapsed = 0;
                  }
                  lcd.setCursor(0,0);
                  lcd.print(empty);
                  
//                  lcd.setCursor(0,3);
//                  lcd.print("TI:");
//                  lcd.print(total_items_now);
//                  lcd.setCursor(9,3);
//                  lcd.print("TA:");
//                  lcd.print(total_amount);
                  
                  lcd.setCursor(17,3);
                  lcd.print("P:");
                  lcd.print(page);
                  Order1S = digitalRead(readpin1);
                  Order2S = digitalRead(readpin2);
                  ConfirmS = digitalRead(confirmpin);
                  ResetS = digitalRead(resetpin);
                  BillS = digitalRead(billpin);      
            if ((millis() - last_sample_at) > sample_interval){
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
                    total_items_now = total_items_now + 1;  
                    client.print(table_id+","+page_id1+","+item_no1);
                    lcd.setCursor(0,2);
                    lcd.print("ITEM 1 Picked.     ");
                    delay(1000);
                    lcd.setCursor(0,2);
                    lcd.print("                   ");
                  }
                  if (Order2S == 1){
                    total_items_now = total_items_now + 1;
                    client.print(table_id+","+page_id2+","+item_no1);
                    lcd.setCursor(0,2);
                    lcd.print("ITEM 2 Picked.      ");
                    delay(1000);
                    lcd.setCursor(0,2);
                    lcd.print("                   ");
                    
                  }
                  if (ConfirmS == 1){
                    total_items_now = 0;
                    client.print(table_id+",confirm");
                    //lcd.clear();
                    lcd.setCursor(0,2);
                    lcd.print("Confirm             ");
                    delay(1000);
                    lcd.setCursor(0,2);
                    lcd.print("                   ");
                    
                  }
                  if (ResetS == 1){
                    total_items_now = 0;
                    client.print(table_id+",reset");
                    lcd.setCursor(0,1);
                    lcd.print(empty);
                    lcd.setCursor(0,2);
                    lcd.print("Reset               ");
                    delay(1000);
                    lcd.setCursor(0,2);
                    lcd.print("                   ");
                  }
                  if (BillS == 1){
                    client.print(table_id+",bill");
                    lcd.setCursor(0,2);
                    lcd.print("Bill                ");
                    delay(1000);
                    lcd.setCursor(0,2);
                    lcd.print("                   ");
                    
                  }
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
                          //Serial.println("in else");
                          lcd.setCursor(0,0);
                          lcd.print(empty);
                          lcd.setCursor(0,0);
                          lcd.print(line);
                          delay(1000);
                          Serial.println(line);
                        }
                  }
            client.stop();
        }
        else {
            //client.stop();
            while (!client.connect(host, port)) {
              lcd.setCursor(0,0);
              lcd.print("Connection failed. ");
              Serial.println("Connection to host failed");
              delay(1000);
            }       
        }
    }
    
    else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
        lcd.setCursor(0,0);
        lcd.print("Wi-Fi Disconnected!");
        Serial.println("Wi-Fi Disconnected!");
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
  
    //delay(50);
}
