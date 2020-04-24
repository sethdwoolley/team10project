# Automated Gardening Project: Firmware

Firmware for Team 10's automated gardening project for COMP208. Repository is hosted on [GitLab](https://gitlab.com/uol-team-10/firmware).

## Running

You will need the hardware listed in the following section, as well as the
Arduino libraries listed below.

## Hardware

* [Arduino MKR WiFi1010](https://store.arduino.cc/arduino-mkr-wifi-1010)
* [SEN0193 Capacitive Soil Moisture Sensor](https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193)
* [DHT22 Temperature and Humidity Sensor](https://www.adafruit.com/product/385)
* [TEMT6000 Ambient Light Sensor](https://wiki.keyestudio.com/Ks0098_keyestudio_TEMT6000_Ambient_Light_Sensor)
* [OLED Screen 128x32](https://www.adafruit.com/product/931)

<!-- ## Schematic
![Schematic](./img/schematic.png) -->

## Arduino Libraries

* [WiFiNINA](https://www.arduino.cc/en/Reference/WiFiNINA) - Arduino Wifi client for SAMD21 chips
* [ArduinoHttpClient](https://github.com/arduino-libraries/ArduinoHttpClient) - Arduino Http protocol library
* [SPI](https://www.arduino.cc/en/reference/SPI) - Arduino Serial Peripheral Interface library (required by WiFiNINA)
* [Wire](https://www.arduino.cc/en/reference/wire) - Arduino I2C library
* [DHT](https://github.com/adafruit/DHT-sensor-library) - Adafruit library for DHT sensors
* [Scheduler](https://www.arduino.cc/en/reference/scheduler) - Arduino scheduling library
* [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306) - Adafruit library for controlling OLED screens
* [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) - Adafruit core graphics library (required for Adafruit_SSD1306)
* [FlashStorage](https://github.com/cmaglie/FlashStorage) -  A convenient way to store data into Flash memory on the ATSAMD21 processor family

## Tutorials, examples, and forum posts

* WiFiNINA
  * [WiFiWebClient](https://www.arduino.cc/en/Tutorial/WiFiNINAWiFiWebClient) - tutorial on connecting to a WiFi network.
  * [AP_SimpleWebServer](https://www.arduino.cc/en/Tutorial/WiFiNINAAPSimpleWebServer) - tutorial on how to create a WiFi access point using the [WiFiNINA](https://www.arduino.cc/en/Reference/WiFiNINA) library.
* Scheduler
* SSD1306
* DHT

## Tools

* [image2cpp image to byte array converter](https://javl.github.io/image2cpp/)
