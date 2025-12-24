#pragma once

#include "FastLED.h"
#include <ArduinoJson.h>
#include "espnowControl.h"

/* If you use more than ~4 characteristics, you need to increase numHandles in this file:
C:\Users\...\.platformio\packages\framework-arduinoespressif32\libraries\BLE\src\BLEServer.h
Setting numHandles = 60 has worked for 7 characteristics.  
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <string>

bool debug = false;
uint8_t dummy = 1;

extern bool powerOn;
//extern bool signalOn;
extern bool trackOn;
extern bool steamerOn;
extern bool stockCarOn;
extern bool lightCarOn;
extern bool cabooseOn;

using namespace fl;

ArduinoJson::JsonDocument sendDoc;
ArduinoJson::JsonDocument receivedJSON;

uint8_t sendVal;
extern void sendESPNOW(uint8_t commandNumber);

//*******************************************************************************
//BLE CONFIGURATION *************************************************************

BLEServer* pServer = NULL;
BLECharacteristic* pButtonCharacteristic = NULL;
BLECharacteristic* pCheckboxCharacteristic = NULL;
//BLECharacteristic* pNumberCharacteristic = NULL;
BLECharacteristic* pStringCharacteristic = NULL;

bool deviceConnected = false;
bool wasConnected = false;

#define SERVICE_UUID                   "19b10000-e8f2-537e-4f6c-d104768a1214"
#define BUTTON_CHARACTERISTIC_UUID     "19b10001-e8f2-537e-4f6c-d104768a1214"
#define CHECKBOX_CHARACTERISTIC_UUID   "19b10002-e8f2-537e-4f6c-d104768a1214"
//#define NUMBER_CHARACTERISTIC_UUID     "19b10003-e8f2-537e-4f6c-d104768a1214"
#define STRING_CHARACTERISTIC_UUID     "19b10004-e8f2-537e-4f6c-d104768a1214"

BLEDescriptor pButtonDescriptor(BLEUUID((uint16_t)0x2902));
BLEDescriptor pCheckboxDescriptor(BLEUUID((uint16_t)0x2902));
//BLEDescriptor pNumberDescriptor(BLEUUID((uint16_t)0x2902));
BLEDescriptor pStringDescriptor(BLEUUID((uint16_t)0x2902));

// UI update functions ***********************************************

void sendReceiptButton(uint8_t receivedValue) {
    pButtonCharacteristic->setValue(String(receivedValue).c_str());
    pButtonCharacteristic->notify();
    if (debug) {
        Serial.print("Button value received: ");
        Serial.println(receivedValue);
    }
}

void sendReceiptCheckbox(String receivedID, bool receivedValue) {
 
      sendDoc.clear();
      sendDoc["id"] = receivedID;
      sendDoc["val"] = receivedValue;

      String jsonString;
      serializeJson(sendDoc, jsonString);

      pCheckboxCharacteristic->setValue(jsonString);
      
      pCheckboxCharacteristic->notify();
      
      if (debug) {
         Serial.print("Sent receipt for ");
         Serial.print(receivedID);
         Serial.print(": ");
         Serial.println(receivedValue);
      }
}

/*
void sendReceiptNumber(String receivedID, float receivedValue) {

      sendDoc.clear();
      sendDoc["id"] = receivedID;
      sendDoc["val"] = receivedValue;

      String jsonString;
      serializeJson(sendDoc, jsonString);

      pNumberCharacteristic->setValue(jsonString);
      
      pNumberCharacteristic->notify();
      
      if (debug) {
         Serial.print("Sent receipt for ");
         Serial.print(receivedID);
         Serial.print(": ");
         Serial.println(receivedValue);
      }
}
*/

void sendReceiptString(String receivedID, String receivedValue) {

   sendDoc.clear();
   sendDoc["id"] = receivedID;
   sendDoc["val"] = receivedValue;

   String jsonString;
   serializeJson(sendDoc, jsonString);

   pStringCharacteristic->setValue(jsonString);

   pStringCharacteristic->notify();
   
   if (debug) {
      Serial.print("Sent receipt for ");
      Serial.print(receivedID);
      Serial.print(": ");
      Serial.println(receivedValue);
   }
}

// Handle UI request functions ***********************************************

std::string convertToStdString(const String& flStr) {
   return std::string(flStr.c_str());
}

void processButton(uint8_t receivedValue) {
    sendReceiptButton(receivedValue);
    if (receivedValue == 10) {powerOn = false;}
    if (receivedValue == 11) {powerOn = true;}
    if (receivedValue == 12) {trackOn=false; steamerOn=false; stockCarOn=false; lightCarOn=false; cabooseOn=false; }
    if (receivedValue == 13) {trackOn=true; steamerOn=true; stockCarOn=true; lightCarOn=true; cabooseOn=true; }
}

void processCheckbox(String receivedID, bool receivedValue ) {
    sendReceiptCheckbox(receivedID, receivedValue);
    if (receivedID == "cx40") {trackOn=receivedValue;}
    if (receivedID == "cx50") {
        steamerOn=receivedValue;
        receivedValue == true ? sendVal = 51 : sendVal = 50;
        sendESPNOW(sendVal);    
    }
    if (receivedID == "cx60") {
        stockCarOn=receivedValue;
        receivedValue == true ? sendVal = 61 : sendVal = 60;
        sendESPNOW(sendVal);    
    }
    if (receivedID == "cx70") {
        lightCarOn=receivedValue;
        receivedValue == true ? sendVal = 71 : sendVal = 70;
        sendESPNOW(sendVal);    
    }
    if (receivedID == "cx90") {
        cabooseOn=receivedValue;
        receivedValue == true ? sendVal = 71 : sendVal = 70;
        sendESPNOW(sendVal);    
    }
}

void processString(String receivedID, String receivedValue ) {
   sendReceiptString(receivedID, receivedValue);
}

//*******************************************************************************
// CALLBACKS ********************************************************************

    class MyServerCallbacks: public BLEServerCallbacks {
        void onConnect(BLEServer* pServer) {
            deviceConnected = true;
            wasConnected = true;
            if (debug) {Serial.println("Device Connected");}
        };
        void onDisconnect(BLEServer* pServer) {
            deviceConnected = false;
            wasConnected = true;
        }
    };

    class ButtonCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *characteristic) {
            String value = characteristic->getValue();
            if (value.length() > 0) {
                uint8_t receivedValue = value[0];
                if (debug) {
                    Serial.print("Button value received: ");
                    Serial.println(receivedValue);
                }
                processButton(receivedValue);
            }
        }
    };

    class CheckboxCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *characteristic) {
            String receivedBuffer = characteristic->getValue();
            if (receivedBuffer.length() > 0) {
                if (debug) {
                    Serial.print("Received buffer: ");
                    Serial.println(receivedBuffer);
                }
                ArduinoJson::deserializeJson(receivedJSON, receivedBuffer);
                String receivedID = receivedJSON["id"] ;
                bool receivedValue = receivedJSON["val"];
                if (debug) {
                    Serial.print(receivedID);
                    Serial.print(": ");
                    Serial.println(receivedValue);
                }
                processCheckbox(receivedID, receivedValue);
            }
        }
    };

/*      class NumberCharacteristicCallbacks : public BLECharacteristicCallbacks {
         void onWrite(BLECharacteristic *characteristic) {
            
            String receivedBuffer = characteristic->getValue();
            
            if (receivedBuffer.length() > 0) {
            
               if (debug) {
                  Serial.print("Received buffer: ");
                  Serial.println(receivedBuffer);
               }
            
               ArduinoJson::deserializeJson(receivedJSON, receivedBuffer);
               String receivedID = receivedJSON["id"] ;
               float receivedValue = receivedJSON["val"];
            
               if (debug) {
                  Serial.print(receivedID);
                  Serial.print(": ");
                  Serial.println(receivedValue);
               }
            
               processNumber(receivedID, receivedValue);
            }
         }
      };
*/
    class StringCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *characteristic) {
            String receivedBuffer = characteristic->getValue();
            if (receivedBuffer.length() > 0) {
                if (debug) {
                    Serial.print("Received buffer: ");
                    Serial.println(receivedBuffer);
                }
                ArduinoJson::deserializeJson(receivedJSON, receivedBuffer);
                String receivedID = receivedJSON["id"] ;
                String receivedValue = receivedJSON["val"];
                if (debug) {
                    Serial.print(receivedID);
                    Serial.print(": ");
                    Serial.println(receivedValue);
                }
                processString(receivedID, receivedValue);
            }
        }
    };

//*******************************************************************************
// BLE SETUP FUNCTION ***********************************************************

void bleSetup() {

    BLEDevice::init("Holiday Train");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pButtonCharacteristic = pService->createCharacteristic(
                    BUTTON_CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_WRITE |
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_NOTIFY
                    );
    pButtonCharacteristic->setCallbacks(new ButtonCharacteristicCallbacks());
    pButtonCharacteristic->setValue(String(dummy).c_str());
    pButtonCharacteristic->addDescriptor(new BLE2902());

    pCheckboxCharacteristic = pService->createCharacteristic(
                    CHECKBOX_CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_WRITE |
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_NOTIFY
                    );
    pCheckboxCharacteristic->setCallbacks(new CheckboxCharacteristicCallbacks());
    pCheckboxCharacteristic->setValue(String(dummy).c_str());
    pCheckboxCharacteristic->addDescriptor(new BLE2902());
    
    /*
    pNumberCharacteristic = pService->createCharacteristic(
                    NUMBER_CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_WRITE |
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_NOTIFY
                    );
    pNumberCharacteristic->setCallbacks(new NumberCharacteristicCallbacks());
    pNumberCharacteristic->setValue(String(dummy).c_str());
    pNumberCharacteristic->addDescriptor(new BLE2902());
    */

    pStringCharacteristic = pService->createCharacteristic(
                    STRING_CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_WRITE |
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_NOTIFY
                    );
    pStringCharacteristic->setCallbacks(new StringCharacteristicCallbacks());
    pStringCharacteristic->setValue(String(dummy).c_str());
    pStringCharacteristic->addDescriptor(new BLE2902());
    
    //**********************************************************

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    if (debug) {Serial.println("Waiting a client connection to notify...");}

}
