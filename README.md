![repository-banner.png](https://s3-eu-west-1.amazonaws.com/asaburido/as-readme-banner.png)

# YunSensorMote 

>A Sensor Mote using [Arduino Yun](https://store.arduino.cc/arduino-yun) that comunicates via Wifi through REST calls using the Bridge Library.

This project uses [Platform.io](http://platformio.org/) project structure and features.

## Libraries
* It uses a [ArduinoJson](https://arduinojson.org/) library to return json objects with the raw data.
* Adafruit [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
   
## The Circuit:
  * Arduino A0 connected to Vout signal pin of KY-013 Analog Temperature Sensor from [Keyes](https://tkkrlab.nl/wiki/Arduino_KY-013_Temperature_sensor_module).
  * Arduino D1 connected to Vout signal pin of KY-015 Humidity DHT11 Sensor from [Keyes](https://tkkrlab.nl/wiki/Arduino_KY-015_Temperature_and_humidity_sensor_module).
  * Arduino A2 connected to Vout signal pin of KY-18 LDR Sensor from [Keyes](https://tkkrlab.nl/wiki/Arduino_KY-018_Photo_resistor_module).
  * Arduino A3 connected to a 10K Pot that will simulate any other analog sensor.
  * Arduino D8 connected to KY-019 Relay Module [Keyes](https://tkkrlab.nl/wiki/Arduino_KY-019_5V_relay_module)
  * Arduino I2C SDA/SCL connected to Adafruit's [MPL3115A2](https://www.adafruit.com/product/1893) Sensor Module

# Development

## Data Structure
The sensor mote data is send in JSON format
```JSON
{
    "moteId": 1,
    "groupId": 2,
    "temp": 532,
    "humidity": 39,
    "ldr": 530,
    "adc": 571,
    "relay": 0
}
```

# Support, Questions, or Feedback
> Contact me anytime for anything about this repo or IoT in general

* [Email: Alvaro Saburido](alvaro.saburido@gmail.com)
* [Twitter: @alvarosaburido1](https://twitter.com/alvarosaburido1)

___

# License
 [MIT](/LICENSE)