#pragma once

#include "FastLED.h"
#include <ArduinoJson.h>

// f0:9e:9e:b2:88:48
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0x98,0xa3,0x16,0x85,0x7d,0xac}; // 98:a3:16:85:7d:ac Steamer
//uint8_t broadcastAddress2[] = {0xFF, , , , , };
//uint8_t broadcastAddress3[] = {0xFF, , , , , };

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

    /*
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
    */
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


