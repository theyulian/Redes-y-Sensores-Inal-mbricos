#include <Wire.h>
#include <SPI.h>
#include <esp_now.h>
#include "WiFi.h"
#define herr 4

uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x33, 0x05, 0x28};

typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

void setup(void) 
{
  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop(void) 
{
  /* Get a new sensor event */ 
  float herradura = digitalRead(herr);

  Serial.println(herradura);
  myData.id=17;
  myData.dato1=herradura;


  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
    
  delay(2000);
}
