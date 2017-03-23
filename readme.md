This project was used as a demo platform for a technical talk on MQTT.  

IMPORTANT: Review platformio.ini build flags to set defines as appropriate.  There are defines for wifi settings and MQTT topic settings

## ESP8266 Resources
 - [Platformio boards explorer](http://platformio.org/boards)
 - [NodeMCU Pinout](https://www.hackster.io/Aritro/getting-started-with-esp-nodemcu-using-arduinoide-aa7267)

## Library Dependencies
 - [ESPHelper](https://github.com/ItKindaWorks/ESPHelper)
 - [Metro](https://github.com/thomasfredericks/Metro-Arduino-Wiring)
 - [pubsubclient](https://github.com/knolleary/pubsubclient)
 - [Debounce](https://github.com/wkoch/Debounce)

## emqtt Broker Information [emqtt.io](http://emqtt.io/)
You can get to the [emqtt dashboard](https://github.com/emqtt/emq-dashboard) by going to `http://localhost:18083/` and using user name: `admin` and password `public` as defaults.
