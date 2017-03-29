This project was used as a demo platform for a technical talk on MQTT and is dependant on the following related resources:

 - [IoT Web Dashboard](https://github.com/pprimm/iot-led-dashboard)
 - [Node-RED Gist for Devices and Display tabs](https://gist.github.com/pprimm/bc381150fbac1f58bdf1cca14ba9f6e1)

This repository serves as the primary readme for the talk.

## Tools and Dependencies
Please refer to the following resources

### ESP8266 Resources
 - [Platformio](http://platformio.org)
 - [Platformio boards explorer](http://platformio.org/boards)
 - [NodeMCU Pinout](https://www.hackster.io/Aritro/getting-started-with-esp-nodemcu-using-arduinoide-aa7267)

### ESP8266 Library Dependencies
 - [ESPHelper](https://github.com/ItKindaWorks/ESPHelper)
 - [Metro](https://github.com/thomasfredericks/Metro-Arduino-Wiring)
 - [pubsubclient](https://github.com/knolleary/pubsubclient)
 - [Debounce](https://github.com/wkoch/Debounce)

### emqtt Broker [emqtt.io](http://emqtt.io/)
Once emqtt is downloaded and launched, following the directions on the site, you can get to the [emqtt dashboard](https://github.com/emqtt/emq-dashboard) by going to `http://localhost:18083/` and entering user name: `admin` and password `public` as defaults.

### mqtt-spy
 - [mqtt-spy Downloads](https://github.com/eclipse/paho.mqtt-spy/wiki/Downloads)
 - [mqtt-spy Getting Started](https://github.com/eclipse/paho.mqtt-spy/wiki/GettingStarted)

### IoT Web Dashboard Dependencies
Please refer to the [dashboard repository iot-led-dashboard](https://github.com/pprimm/iot-led-dashboard) for more information.

## ESP8266 Code Description

IMPORTANT: This project uses Platformio's ability to "inject" defines based on the `[env:...]` build-flags argument for modifying the `main.ino` for multiple devices; including uploads.  So, if you're a little unfamiliar with this, you'll see what appears like a constant (ie `SET_LED_TOPIC_DEF`) used in `main.ino` that doesn't seem to be defined.  Just review the `platformio.ini` for its definition based on a particular `[env:{device name}]` set of defines.  So, for your own purposes, modify the `platformio.ini` build flags to set defines as appropriate.  There are defines for wifi settings, MQTT broker settings, and MQTT topics.

This program is designed to:

1. connect to a predefined/hard-coded wifi SSID as defined in the `platformio.ini` file using the ESPHepler library
2. connect to a predefined/hard-coded MQTT broker s defined in the `platformio.ini` file; also using the ESPHepler library
3. Detect a button press to ramp a pot (short for potentiometer) value up and down; cycling as long as the button is pressed
4. Listen for pot and led value set from predefined/hard-coded topics as defined in the `platformio.ini` file and update the local system as appropriate
5. Send the pot value to a predefined/hard-coded topic as defined in the `platformio.ini` file when the value changes either by button press(es) or MQTT topic set(s)
6. Send the led value to a predefined/hard-coded topic as defined in the `platformio.ini` file when the value changes by MQTT topic set(s)

