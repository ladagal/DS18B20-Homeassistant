#include <ESP8266WiFi.h>
#include <ArduinoHA.h>
#include "Secrets.h"

#include <OneWire.h>
#include <DallasTemperature.h>



//-----DS1820-------------------
const int oneWireBus = D7;     
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device, 20);

HASensorNumber ds1820("Teplota_akvarium", HASensorNumber::PrecisionP1);


void setup() {

     Serial.begin(9600);
    Serial.println("Starting...");

    // Unique ID must be set!
    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    // connect to wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500); // waiting for the connection
    }
    Serial.println();
    Serial.println("Connected to the network");

    mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);

    // set device's details (optional)
    device.setName("Modul 1");
    device.setSoftwareVersion("1.0.0");
    device.setManufacturer("Spolecnost");
    device.setModel("NodeMCU");


    ds1820.setIcon("mdi:temperature-celsius");
    ds1820.setName("Teplota");
    ds1820.setUnitOfMeasurement("°C");




}

void loop() {
   mqtt.loop();

//DS1820------------------------------------------------
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);

  ds1820.setValue(temperatureC);    
  delay(5000);
  
}
