
#include "WiFi.h"
#include <esp_now.h>
#include <Wire.h>


typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  
  if(mac_addr[0]==0x84 && mac_addr[1]==0xcc && mac_addr[2]==0xa8 && mac_addr[3]==0x2c && mac_addr[4]==0x99 && mac_addr[5]==0x5c){
    boardsStruct[myData.id-1].dato1 = myData.dato1;
    Serial.printf("%f",boardsStruct[myData.id-1].dato1);
    Serial.println();
  }
    if(mac_addr[0]==0x84 && mac_addr[1]==0xcc && mac_addr[2]==0xa8 && mac_addr[3]==0x7a && mac_addr[4]==0x23 && mac_addr[5]==0x88){
    boardsStruct[myData.id-1].dato1 = myData.dato1;
    Serial.printf("%f",boardsStruct[myData.id-1].dato1);
    Serial.println();
  }
      if(mac_addr[0]==0xac && mac_addr[1]==0x67 && mac_addr[2]==0xb2 && mac_addr[3]==0x38 && mac_addr[4]==0x5a && mac_addr[5]==0x44){
    boardsStruct[myData.id-1].dato1 = myData.dato1;
    Serial.printf("%f",boardsStruct[myData.id-1].dato1);
    Serial.println();
  }
    
}

void setup() {
   Serial.begin(9600);

   WiFi.mode(WIFI_STA);
   Serial.println(WiFi.macAddress());
   
  // put your setup code here, to run once:
  
  if (esp_now_init() != ESP_OK) {
  Serial.println("Error initializing ESP-NOW");
  return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
