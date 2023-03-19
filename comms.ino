#include <esp_now.h>
#include <WiFi.h>

uint8_t controllerAddr[] = {0xC8, 0xF0, 0x9E, 0x9F, 0x57, 0x70};

String commStatus;



typedef struct comm_data_out {
  float alt;
  float temp;
  float pres;
  float ax;
  float ay;
  float az;
  float rx;
  float ry;
  float rz;
} comm_data_out;

comm_data_out dataBuffer;

esp_now_peer_info_t peerInfo;
 
bool initComms() {
  Serial.print("Initializing comms...");
  WiFi.mode(WIFI_MODE_STA);
//  Serial.println(WiFi.macAddress());
  WiFi.disconnect();
//  delay(5000);

  // Init ESP-NOWÂ
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return false;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, controllerAddr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return false;
  }  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  return true;
  Serial.println("success");
}
 
void commLoop() {
//  getReadings();
 
//  esp_now_send(broadcastAddress, (uint8_t *) &DHTReadings, sizeof(DHTReadings));
  delay(10000);
}
