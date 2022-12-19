
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <string.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define helloCHARACTERISTIC_UUID "280a8262-719c-11ed-a1eb-0242ac120002"
#define passCHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define ssidCHARACTERISTIC_UUID "b8e3c77e-7199-11ed-a1eb-0242ac120002"
 BLECharacteristic *passCharacteristic;
 BLECharacteristic *ssidCharacteristic;
 BLECharacteristic *helloCharacteristic;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("your device"); // modifier ce nom de telle sorte à ne pas avoir plusieurs devices publiant le même ID.
  BLEServer *helloServer = BLEDevice::createServer();
  BLEService *helloService = helloServer->createService(SERVICE_UUID);
 passCharacteristic = helloService->createCharacteristic(
                                         passCHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ|
                                          BLECharacteristic::PROPERTY_WRITE
                                       );
 ssidCharacteristic = helloService->createCharacteristic(
                                         ssidCHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  helloCharacteristic = helloService->createCharacteristic(
                                         helloCHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ | 
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  helloCharacteristic->setValue("");
  ssidCharacteristic->setValue("Here get me your ssid");
  passCharacteristic->setValue("Here get me your password");
  helloService->start();
  // BLEAdvertising *pAdvertising = helloServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println(F("Characteristic defined! Now you can read it in your phone!"));
}

void loop() {
  
  std::string value1 = passCharacteristic->getValue();
  std::string value2 = ssidCharacteristic->getValue();
  std::string value3= helloCharacteristic->getValue();
  String message3=value3.c_str();
  if(message3=="configure"){
  String message1=value1.c_str();
  String message2=value2.c_str();
  char str1[message1.length()+1];
  char str2[message2.length()+1];
  message1.toCharArray(str1,message1.length()+1);
  message2.toCharArray(str2,message2.length()+1);
 
  Serial.print(F("The ssid is: "));
  Serial.println(str2);
  Serial.print(F("The password is: "));
  // put your main code here, to run repeatedly:
  Serial.println(str1);

WiFi.begin(str2, str1);
int i=10;
  while ((WiFi.status() != WL_CONNECTED)&(i!=0)) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    i=i-1;
  }
  // Print ESP Local IP Address
  if (WiFi.status()==WL_CONNECTED){
  Serial.println(WiFi.localIP());
  }
  helloCharacteristic->setValue("");
  }     
  delay(2000);

  }  
