#pragma once

#include "FastLED.h"
#include <ArduinoJson.h>

// d8:3b:da:75:b8:bc  Main       
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0x98,0xa3,0x16,0x85,0x26,0x80}; //   98:a3:16:85:26:80   Steamer
uint8_t broadcastAddress2[] = {0x94,0xa9,0x90,0x78,0x88,0x80}; //   94:a9:90:78:88:80   Stock Car   
uint8_t broadcastAddress3[] = {0xb4,0x3a,0x45,0x89,0xe4,0x48}; //   b4:3a:45:89:e4:48   Light Car 

// Main        d8:3b:da:75:b8:bc    Pink
// Steamer     98:a3:16:85:26:80    Orange
// Light Car   b4:3a:45:89:e4:48    Purple
// Stock Car   94:a9:90:78:88:80    Teal    

// ================================================================================================

typedef struct message_struct {
  int x;
} message_struct;

message_struct message;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

uint8_t commandNumber;

// =========================================================================

void espnowSetup() {

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
        
    esp_now_register_send_cb(OnDataSent);
    
    // register peer
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // register first peer  
    memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    // register second peer  
    memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    
    /// register third peer
    memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

// =========================================================================

void sendESPNOW(uint8_t commandNumber) {

    message.x = commandNumber;

    esp_err_t result = esp_now_send(0, (uint8_t *) &message, sizeof(message_struct));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
    
}


