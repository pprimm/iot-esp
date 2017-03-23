/*
    Copyright (c) 2017 PrimmTech Corp. All rights reserved.


  This program uses ESPHelper
    Copyright (c) 2016 ItKindaWorks All right reserved.
    github.com/ItKindaWorks

    IMPORTANT
    Review platformio.ini build flags to set defines as appropriate
*/
#include "ESPHelper.h"
#include "Debounce.h"
#include "Metro.h"

/** ESPHelper stuff **/
netInfo homeNet = {.name = NET_NAME, .mqtt = MQTT_HOST, .ssid = WIFI_SSID, .pass = WIFI_PASS};
int espStatus = NO_CONNECTION;
ESPHelper myESP(&homeNet);
char const* OTA_PASSWORD = "password";
char const* OTA_HOSTNAME = "gaspra";


/* The topic names below need to be changed to name the device on the
   MQTT topic structure.
**/
char const* SET_LED_TOPIC = SET_LED_TOPIC_DEF;
char const* GET_LED_TOPIC = GET_LED_TOPIC_DEF;
char const* SET_POT_TOPIC = SET_POT_TOPIC_DEF;
char const* GET_POT_TOPIC = GET_POT_TOPIC_DEF;
char const* SUBSCRIBE_TOPIC = SUBSCRIBE_TOPIC_DEF;

/** Button vars and constants **/
int const BUTTON_PIN = D0;
Debounce button(BUTTON_PIN);
bool localButtonState = LOW;
bool mqttButtonState = LOW;

/** 0 - 100% limiter **/
int const LED_LOWER_LIMIT = 0;
int const LED_UPPER_LIMIT = 100;
int const MAX_LED_COUNTS = 1024;
int limitLEDValue(int newValue) {
  if (newValue < LED_LOWER_LIMIT) { return LED_LOWER_LIMIT; }
  if (newValue > LED_UPPER_LIMIT) { return LED_UPPER_LIMIT; }
}

/** LED vars and constants **/
int const PWM_PIN = D2;
int ledValue = LED_LOWER_LIMIT;
int mqttLedValue = -1; // set to -1 to force update on boot

/** pot vars and constants **/
int potValue = LED_LOWER_LIMIT;
int mqttPotValue = -1; // set to -1 to force update on boot

/** Dimmer control state var and constants **/
enum dimmerStates { DIMMER_IDLE, DIMMER_UP, DIMMER_DOWN };
int dimmerState = DIMMER_IDLE;
int lastDimmerState = DIMMER_DOWN;
int const DIMMER_INC = 1;

/******************************************************************************/
void ServiceDimmer(bool buttonState) {
  switch (dimmerState) {
    case DIMMER_IDLE:
      if (buttonState) {
        dimmerState = lastDimmerState == DIMMER_UP ? DIMMER_DOWN : DIMMER_UP;
        lastDimmerState = dimmerState;
      }
      break;
    case DIMMER_UP:
      if (buttonState) {
        potValue = limitLEDValue(potValue + DIMMER_INC);
      }
      if (!buttonState || potValue >= LED_UPPER_LIMIT) {
        dimmerState = DIMMER_IDLE;
      }
      break;
    case DIMMER_DOWN:
      if (buttonState) {
        potValue = limitLEDValue(potValue - DIMMER_INC);
      }
      if (!buttonState || potValue <= LED_LOWER_LIMIT) {
        dimmerState = DIMMER_IDLE;
      }
      break;
  }
}

/******************************************************************************/
void onMessage(char* topic, byte* payload, unsigned int length) {
  String topicString = String(topic);
  String payloadString = String((char*)(payload));
  payloadString.trim();
  if (topicString == String(SET_POT_TOPIC)) {
    potValue = limitLEDValue(payloadString.toInt());
  } else if (topicString == String(SET_LED_TOPIC)) {
    ledValue = limitLEDValue(payloadString.toInt());
  }
}

/** Metro scheduler stuff **/
Metro LedScheduler(100);
Metro MqttScheduler(100);
Metro DimmerScheduler(50);
int const METRO_READY = 1;

/******************************************************************************/
void setup() {
  //setup ota on esphelper
  myESP.OTA_enable();
  myESP.OTA_setPassword(OTA_PASSWORD);
  myESP.OTA_setHostnameWithVersion(OTA_HOSTNAME);

  myESP.enableHeartbeat(D1);

  myESP.addSubscription(SUBSCRIBE_TOPIC);

  myESP.begin();
  myESP.setCallback(onMessage);

 	pinMode(BUTTON_PIN, INPUT);
}

/******************************************************************************/
void loop() {
  localButtonState = !button.read();
  if (DimmerScheduler.check() == METRO_READY) {
    ServiceDimmer(localButtonState);
  }
  espStatus = myESP.loop();
  if (espStatus == FULL_CONNECTION && MqttScheduler.check() == METRO_READY) {
    if (potValue != mqttPotValue) {
      mqttPotValue = potValue;
      myESP.publish(GET_POT_TOPIC, String(mqttPotValue).c_str(), true);
    }
    if (ledValue != mqttLedValue) {
      mqttLedValue = ledValue;
      myESP.publish(GET_LED_TOPIC, String(mqttLedValue).c_str(), true);
    }
  }
  if (LedScheduler.check() == METRO_READY) {
    int ledCounts = (int)((float)ledValue / LED_UPPER_LIMIT * MAX_LED_COUNTS);
    if (ledCounts > MAX_LED_COUNTS) { ledCounts = MAX_LED_COUNTS; }
    analogWrite(PWM_PIN, ledCounts);
  }
  yield();
}
