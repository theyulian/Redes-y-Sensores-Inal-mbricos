#include <Wire.h>
#include <SPI.h>
#include <esp_now.h>
#include "WiFi.h"
#define herr 4

uint8_t broadcastAddress2[] = {0x84, 0xcc, 0xa8, 0x7a, 0x23, 0x88};
uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x33, 0x05, 0x28};

typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
    float flagtosend;
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
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  peerInfo.channel = 1;
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

  float tiempo= micros();
  // Send message via ESP-NOW
  myData.flagtosend=0;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    tiempo=micros() - tiempo;
    Serial.print("tiempo1: ");
    Serial.println( tiempo);
    
  }
  else{
    tiempo=100000;
  }

  float tiempo2= micros();
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
  if (result2 == ESP_OK) {
    
    tiempo2=micros() - tiempo2;
    Serial.print("tiempo_2: ");
    Serial.println(tiempo2);

  }
  else{
    tiempo2=100000;
  }
  myData.flagtosend=1;
  if(tiempo>=tiempo2){
    esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
      if (result == ESP_OK) {
         Serial.println("Sent with success");
    
      }
      else {
        Serial.println("Error sending the data");
      }
  }
  else{
      esp_err_t result2 = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      if (result2 == ESP_OK) {
        Serial.println("5");
        Serial.println("Sent with success");
      }
      else {
        Serial.println("Error sending the data");
      }
  }
  tiempo=0;
  tiempo2=0;
  delay(1000);
}
