#include "esp_wpa2.h"
#include <WiFi.h>

const char* ssid = "IITD_WIFI"; // your ssid
#define EAP_ID "jtm182004"
#define EAP_USERNAME "jtm182004" //removed for obvious reasons
#define EAP_PASSWORD "nigoltatii@63"

void setup() {
Serial.begin(115200);
delay(10);

Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

// WPA2 enterprise magic starts here
WiFi.disconnect(true);
esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ID, strlen(EAP_ID));
esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
esp_wifi_sta_wpa2_ent_enable(&config);
// WPA2 enterprise magic ends here


WiFi.begin(ssid);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

}



/*
* everyhting below this, in loop(), is just a standard request to a webserver and nothing else than an example to show that is works.
*
*/

void loop() {
  
}
