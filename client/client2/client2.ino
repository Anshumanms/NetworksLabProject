#include <WiFi.h>
 
const char* ssid = "anshuman";
const char* password =  "12345678";
 
const uint16_t port = 8090;
const char * host = "10.194.34.84";
int count = 0;
WiFiClient client;

void setup()
{
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
 
 while (!client.connect(host, port)) {
 
      Serial.println("Connection to host failed");
      delay(1000);
      //return;
    }
  
  Serial.println("Connected to server successful!");
}
 
void loop() {

    client.print("Hello from ESP32!");
    client.print("This looks good!");
    //Serial.println("Disconnecting...");
    //client.stop();
 
    delay(10000);
}
