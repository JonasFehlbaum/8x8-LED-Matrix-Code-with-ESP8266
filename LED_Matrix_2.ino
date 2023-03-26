// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <Scheduler.h>
#include "HTML.h"

// Replace with your network credentials
const char* ssid = "WLH3";
const char* password = "jogologo05";

#define ledPin D1
#define numLeds 64
bool ledState = 0;

//WebVariablen
int pattern = 2;
int brightness = 50;
int Delay = 1;
int Speed = 100;
float seed = 64;
int pU;

int brightnessUpdate;

//Adafruit_NeoPixel pixels(8, ledPin);
Adafruit_NeoPixel strip(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);
uint32_t purple = strip.Color(255, 0, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t cyan = strip.Color(0, 255, 255);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String message(String enkryption, int value) {
  return enkryption + String(value);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, int num) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    //Serial.print("WebSocketEvent Message = ");
    //Serial.println((char*)data);
    if (strcmp((char*)data, "*toggle") == 0) {
      ledState = !ledState;
      ws.textAll(String(message("*", ledState)));
    }
    if (strncmp((char*)data, "#brightUpdate=", 14) == 0) {
      brightness = (uint16_t) strtol((char *) &data[14], NULL, 10);
      //ws.textAll(String(message("#b",brightness)));
      LinkedList conClients = ws.getClients();
      for (const auto& c : conClients) {
        if (c->id() != num) {
          ws.text(c->id(), String(message("#b", brightness)));
        }
      }
    }

    if (strncmp((char*)data, "#speedUpdate=", 13) == 0) {
      Speed = (uint16_t) strtol((char *) &data[13], NULL, 10);
      LinkedList conClients = ws.getClients();
      for (const auto& c : conClients) {
        if (c->id() != num) {
          ws.text(c->id(), String(message("#s", Speed)));
        }
      }
    }

    if (strncmp((char*)data, "#patternUpdate=", 15) == 0) {
      pU = (uint16_t) strtol((char *) &data[15], NULL, 10);
      ws.textAll(String(message("#p", pU)));
      //Serial.println(message("#p", pU));
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws.textAll(String(message("*", ledState)));
      ws.textAll(String(message("#b", brightness)));
      ws.textAll(String(message("#s", Speed)));
      ws.textAll(String(message("#p", pU)));
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len, client->id());
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var) {
  Serial.println(var);
  if (var == "STATE") {
    if (ledState) {
      return "OFF";
    }
    else {
      return "ON";
    }
  }
  return String();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//Patterns
void noise() {
  //call 1
  if (seed <= numLeds) {
    seed = numLeds;
  }
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < numLeds; j++) {
      strip.setPixelColor(random(0, seed), Wheel((i * 1 + j) & 255));
      if (ledState == 0)
        break;
    }
    if (ledState == 0)
      break;
    strip.show();
    delay(Speed);
  }
}

void rainbow() {
  //call 2
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < numLeds; i++) {
      strip.setPixelColor(i, Wheel((i * 1 + j) & 255));
      if (ledState == 0)
        break;
    }
    if (ledState == 0)
      break;
    strip.show();
    delay(Speed);
  }
}

void matrix() {
  //call 3
  uint32_t rain = Wheel(random(1, 256));
  for (int i = 1; i <= 4; i++) {
    switch (i) {
      case 1:
        strip.fill(off, 0, 64);
        strip.setPixelColor(2, rain);
        strip.setPixelColor(3, rain);
        strip.setPixelColor(4, rain);
        strip.setPixelColor(5, rain);
        strip.setPixelColor(9, rain);
        strip.setPixelColor(10, rain);
        strip.setPixelColor(11, rain);
        strip.setPixelColor(12, rain);
        strip.setPixelColor(13, rain);
        strip.setPixelColor(14, rain);
        strip.setPixelColor(16, rain);
        strip.setPixelColor(17, rain);
        strip.setPixelColor(18, rain);
        strip.setPixelColor(19, rain);
        strip.setPixelColor(20, rain);
        strip.setPixelColor(21, rain);
        strip.setPixelColor(22, rain);
        strip.setPixelColor(24, rain);
        strip.setPixelColor(25, rain);
        strip.setPixelColor(26, rain);
        strip.setPixelColor(27, rain);
        strip.setPixelColor(28, rain);
        strip.setPixelColor(29, rain);
        strip.setPixelColor(30, rain);
        strip.setPixelColor(32, rain);
        strip.setPixelColor(33, rain);
        strip.setPixelColor(34, rain);
        strip.setPixelColor(35, rain);
        strip.setPixelColor(36, rain);
        strip.setPixelColor(37, rain);
        strip.setPixelColor(40, rain);
        strip.setPixelColor(41, rain);
        strip.setPixelColor(42, rain);
        strip.setPixelColor(43, rain);
        strip.setPixelColor(49, rain);
        strip.setPixelColor(50, rain);
        strip.setPixelColor(51, rain);
        //strip.setPixelColor(53, rain);
        strip.setPixelColor(58, rain);
        strip.setPixelColor(59, rain);
        strip.setPixelColor(60, rain);
        // strip.setPixelColor(62, rain);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 2:
        strip.fill(off, 0, 64);
        strip.setPixelColor(2, rain);
        strip.setPixelColor(3, rain);
        strip.setPixelColor(4, rain);
        strip.setPixelColor(5, rain);
        strip.setPixelColor(9, rain);
        strip.setPixelColor(10, rain);
        strip.setPixelColor(11, rain);
        strip.setPixelColor(12, rain);
        strip.setPixelColor(13, rain);
        strip.setPixelColor(14, rain);
        strip.setPixelColor(16, rain);
        strip.setPixelColor(17, rain);
        strip.setPixelColor(18, rain);
        strip.setPixelColor(19, rain);
        strip.setPixelColor(20, rain);
        strip.setPixelColor(21, rain);
        strip.setPixelColor(22, rain);
        strip.setPixelColor(23, rain);
        strip.setPixelColor(24, rain);
        strip.setPixelColor(25, rain);
        strip.setPixelColor(26, rain);
        strip.setPixelColor(27, rain);
        strip.setPixelColor(28, rain);
        strip.setPixelColor(29, rain);
        strip.setPixelColor(30, rain);
        strip.setPixelColor(31, rain);
        strip.setPixelColor(32, rain);
        strip.setPixelColor(35, rain);
        strip.setPixelColor(36, rain);
        strip.setPixelColor(37, rain);
        strip.setPixelColor(38, rain);
        //strip.setPixelColor(39, rain);
        strip.setPixelColor(41, rain);
        strip.setPixelColor(43, rain);
        strip.setPixelColor(44, rain);
        strip.setPixelColor(45, rain);
        strip.setPixelColor(46, rain);
        strip.setPixelColor(47, rain);
        //strip.setPixelColor(48, rain);
        strip.setPixelColor(52, rain);
        strip.setPixelColor(53, rain);
        strip.setPixelColor(54, rain);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 3:
        strip.fill(off, 0, 64);
        //strip.setPixelColor(1, rain);
        strip.setPixelColor(3, rain);
        strip.setPixelColor(4, rain);
        strip.setPixelColor(5, rain);
        //strip.setPixelColor(10, rain);
        strip.setPixelColor(12, rain);
        strip.setPixelColor(13, rain);
        strip.setPixelColor(14, rain);
        strip.setPixelColor(20, rain);
        strip.setPixelColor(21, rain);
        strip.setPixelColor(22, rain);
        strip.setPixelColor(23, rain);
        strip.setPixelColor(26, rain);
        strip.setPixelColor(27, rain);
        strip.setPixelColor(28, rain);
        strip.setPixelColor(29, rain);
        strip.setPixelColor(30, rain);
        strip.setPixelColor(31, rain);
        strip.setPixelColor(33, rain);
        strip.setPixelColor(34, rain);
        strip.setPixelColor(35, rain);
        strip.setPixelColor(36, rain);
        strip.setPixelColor(37, rain);
        strip.setPixelColor(38, rain);
        strip.setPixelColor(39, rain);
        strip.setPixelColor(41, rain);
        strip.setPixelColor(42, rain);
        strip.setPixelColor(43, rain);
        strip.setPixelColor(44, rain);
        strip.setPixelColor(45, rain);
        strip.setPixelColor(46, rain);
        strip.setPixelColor(47, rain);
        strip.setPixelColor(49, rain);
        strip.setPixelColor(50, rain);
        strip.setPixelColor(51, rain);
        strip.setPixelColor(52, rain);
        strip.setPixelColor(53, rain);
        strip.setPixelColor(54, rain);
        strip.setPixelColor(58, rain);
        strip.setPixelColor(59, rain);
        strip.setPixelColor(60, rain);
        strip.setPixelColor(61, rain);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 4:
        strip.fill(off, 0, 64);
        strip.setPixelColor(9, rain);
        strip.setPixelColor(10, rain);
        strip.setPixelColor(11, rain);
        //strip.setPixelColor(15, rain);
        strip.setPixelColor(16, rain);
        strip.setPixelColor(17, rain);
        strip.setPixelColor(18, rain);
        strip.setPixelColor(19, rain);
        strip.setPixelColor(20, rain);
        //strip.setPixelColor(22, rain);
        strip.setPixelColor(24, rain);
        strip.setPixelColor(25, rain);
        strip.setPixelColor(26, rain);
        strip.setPixelColor(27, rain);
        strip.setPixelColor(28, rain);
        strip.setPixelColor(31, rain);
        strip.setPixelColor(32, rain);
        strip.setPixelColor(33, rain);
        strip.setPixelColor(34, rain);
        strip.setPixelColor(35, rain);
        strip.setPixelColor(36, rain);
        strip.setPixelColor(37, rain);
        strip.setPixelColor(38, rain);
        strip.setPixelColor(39, rain);
        strip.setPixelColor(40, rain);
        strip.setPixelColor(41, rain);
        strip.setPixelColor(42, rain);
        strip.setPixelColor(43, rain);
        strip.setPixelColor(44, rain);
        strip.setPixelColor(45, rain);
        strip.setPixelColor(46, rain);
        strip.setPixelColor(47, rain);
        strip.setPixelColor(49, rain);
        strip.setPixelColor(50, rain);
        strip.setPixelColor(51, rain);
        strip.setPixelColor(52, rain);
        strip.setPixelColor(53, rain);
        strip.setPixelColor(54, rain);
        strip.setPixelColor(58, rain);
        strip.setPixelColor(59, rain);
        strip.setPixelColor(60, rain);
        strip.setPixelColor(61, rain);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
    }
  }
}

void fire() {
  //call 4
  for (int i = 1; i <= 4; i++) {
    switch (i) {
      case 1:
        strip.fill(off, 0, 64);
        strip.setPixelColor(2, red);
        strip.setPixelColor(3, red);
        strip.setPixelColor(4, red);
        strip.setPixelColor(5, red);
        strip.setPixelColor(9, red);
        strip.setPixelColor(10, red);
        strip.setPixelColor(11, yellow);
        strip.setPixelColor(12, yellow);
        strip.setPixelColor(13, yellow);
        strip.setPixelColor(14, red);
        strip.setPixelColor(16, red);
        strip.setPixelColor(17, red);
        strip.setPixelColor(18, yellow);
        strip.setPixelColor(19, white);
        strip.setPixelColor(20, yellow);
        strip.setPixelColor(21, yellow);
        strip.setPixelColor(22, red);
        strip.setPixelColor(24, red);
        strip.setPixelColor(25, yellow);
        strip.setPixelColor(26, white);
        strip.setPixelColor(27, yellow);
        strip.setPixelColor(28, yellow);
        strip.setPixelColor(29, yellow);
        strip.setPixelColor(30, red);
        strip.setPixelColor(32, red);
        strip.setPixelColor(33, yellow);
        strip.setPixelColor(34, yellow);
        strip.setPixelColor(35, red);
        strip.setPixelColor(36, red);
        strip.setPixelColor(37, red);
        strip.setPixelColor(40, red);
        strip.setPixelColor(41, yellow);
        strip.setPixelColor(42, yellow);
        strip.setPixelColor(43, red);
        strip.setPixelColor(49, red);
        strip.setPixelColor(50, yellow);
        strip.setPixelColor(51, red);
        strip.setPixelColor(53, red);
        strip.setPixelColor(58, red);
        strip.setPixelColor(59, red);
        strip.setPixelColor(60, red);
        strip.setPixelColor(62, red);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 2:
        strip.fill(off, 0, 64);
        strip.setPixelColor(2, red);
        strip.setPixelColor(3, red);
        strip.setPixelColor(4, red);
        strip.setPixelColor(5, red);
        strip.setPixelColor(9, red);
        strip.setPixelColor(10, yellow);
        strip.setPixelColor(11, yellow);
        strip.setPixelColor(12, yellow);
        strip.setPixelColor(13, red);
        strip.setPixelColor(14, red);
        strip.setPixelColor(16, red);
        strip.setPixelColor(17, yellow);
        strip.setPixelColor(18, yellow);
        strip.setPixelColor(19, yellow);
        strip.setPixelColor(20, white);
        strip.setPixelColor(21, yellow);
        strip.setPixelColor(22, red);
        strip.setPixelColor(23, red);
        strip.setPixelColor(24, red);
        strip.setPixelColor(25, red);
        strip.setPixelColor(26, red);
        strip.setPixelColor(27, red);
        strip.setPixelColor(28, yellow);
        strip.setPixelColor(29, white);
        strip.setPixelColor(30, yellow);
        strip.setPixelColor(31, red);
        strip.setPixelColor(32, red);
        strip.setPixelColor(35, red);
        strip.setPixelColor(36, yellow);
        strip.setPixelColor(37, yellow);
        strip.setPixelColor(38, yellow);
        strip.setPixelColor(39, red);
        strip.setPixelColor(41, red);
        strip.setPixelColor(43, red);
        strip.setPixelColor(44, yellow);
        strip.setPixelColor(45, yellow);
        strip.setPixelColor(46, yellow);
        strip.setPixelColor(47, red);
        strip.setPixelColor(48, red);
        strip.setPixelColor(52, red);
        strip.setPixelColor(53, red);
        strip.setPixelColor(54, red);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 3:
        strip.fill(off, 0, 64);
        strip.setPixelColor(1, red);
        strip.setPixelColor(3, red);
        strip.setPixelColor(4, red);
        strip.setPixelColor(5, red);
        strip.setPixelColor(10, red);
        strip.setPixelColor(12, red);
        strip.setPixelColor(13, yellow);
        strip.setPixelColor(14, red);
        strip.setPixelColor(20, red);
        strip.setPixelColor(21, yellow);
        strip.setPixelColor(22, yellow);
        strip.setPixelColor(23, red);
        strip.setPixelColor(26, red);
        strip.setPixelColor(27, red);
        strip.setPixelColor(28, red);
        strip.setPixelColor(29, yellow);
        strip.setPixelColor(30, yellow);
        strip.setPixelColor(31, red);
        strip.setPixelColor(33, red);
        strip.setPixelColor(34, yellow);
        strip.setPixelColor(35, yellow);
        strip.setPixelColor(36, yellow);
        strip.setPixelColor(37, white);
        strip.setPixelColor(38, yellow);
        strip.setPixelColor(39, red);
        strip.setPixelColor(41, red);
        strip.setPixelColor(42, yellow);
        strip.setPixelColor(43, yellow);
        strip.setPixelColor(44, white);
        strip.setPixelColor(45, yellow);
        strip.setPixelColor(46, red);
        strip.setPixelColor(47, red);
        strip.setPixelColor(49, red);
        strip.setPixelColor(50, yellow);
        strip.setPixelColor(51, yellow);
        strip.setPixelColor(52, yellow);
        strip.setPixelColor(53, red);
        strip.setPixelColor(54, red);
        strip.setPixelColor(58, red);
        strip.setPixelColor(59, red);
        strip.setPixelColor(60, red);
        strip.setPixelColor(61, red);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
      case 4:
        strip.fill(off, 0, 64);
        strip.setPixelColor(9, red);
        strip.setPixelColor(10, red);
        strip.setPixelColor(11, red);
        strip.setPixelColor(15, red);
        strip.setPixelColor(16, red);
        strip.setPixelColor(17, yellow);
        strip.setPixelColor(18, yellow);
        strip.setPixelColor(19, yellow);
        strip.setPixelColor(20, red);
        strip.setPixelColor(22, red);
        strip.setPixelColor(24, red);
        strip.setPixelColor(25, yellow);
        strip.setPixelColor(26, yellow);
        strip.setPixelColor(27, yellow);
        strip.setPixelColor(28, red);
        strip.setPixelColor(31, red);
        strip.setPixelColor(32, red);
        strip.setPixelColor(33, yellow);
        strip.setPixelColor(34, white);
        strip.setPixelColor(35, yellow);
        strip.setPixelColor(36, red);
        strip.setPixelColor(37, red);
        strip.setPixelColor(38, red);
        strip.setPixelColor(39, red);
        strip.setPixelColor(40, red);
        strip.setPixelColor(41, red);
        strip.setPixelColor(42, yellow);
        strip.setPixelColor(43, white);
        strip.setPixelColor(44, yellow);
        strip.setPixelColor(45, yellow);
        strip.setPixelColor(46, yellow);
        strip.setPixelColor(47, red);
        strip.setPixelColor(49, red);
        strip.setPixelColor(50, red);
        strip.setPixelColor(51, yellow);
        strip.setPixelColor(52, yellow);
        strip.setPixelColor(53, yellow);
        strip.setPixelColor(54, red);
        strip.setPixelColor(58, red);
        strip.setPixelColor(59, red);
        strip.setPixelColor(60, red);
        strip.setPixelColor(61, red);
        strip.show();
        if (!ledState)
          break;
        delay(Speed);
        break;
    }
  }
}

void windows() {
  //call 5
  for (int i = 1; i < 4; i++) {
    switch (i) {
      case 1:
        strip.fill(off, 0, 64);
        //strip.setPixelColor(0, red);
        //strip.setPixelColor(1, red);
        //strip.setPixelColor(2, red);
        //strip.setPixelColor(3, red);
        //strip.setPixelColor(8, red);
        strip.setPixelColor(9, red);
        strip.setPixelColor(10, red);
        strip.setPixelColor(11, red);
        //strip.setPixelColor(16, red);
        strip.setPixelColor(17, red);
        strip.setPixelColor(18, red);
        strip.setPixelColor(19, red);
        //strip.setPixelColor(24, red);
        strip.setPixelColor(25, red);
        strip.setPixelColor(26, red);
        strip.setPixelColor(27, red);

        //strip.setPixelColor(4, green);
        //strip.setPixelColor(5, green);
        //strip.setPixelColor(6, green);
        //strip.setPixelColor(7, green);
        strip.setPixelColor(12, green);
        strip.setPixelColor(13, green);
        strip.setPixelColor(14, green);
        //strip.setPixelColor(15, green);
        strip.setPixelColor(20, green);
        strip.setPixelColor(21, green);
        strip.setPixelColor(22, green);
        //strip.setPixelColor(23, green);
        strip.setPixelColor(28, green);
        strip.setPixelColor(29, green);
        strip.setPixelColor(30, green);
        //strip.setPixelColor(31, green);

        //strip.setPixelColor(32, blue);
        strip.setPixelColor(33, blue);
        strip.setPixelColor(34, blue);
        strip.setPixelColor(35, blue);
        //strip.setPixelColor(40, blue);
        strip.setPixelColor(41, blue);
        strip.setPixelColor(42, blue);
        strip.setPixelColor(43, blue);
        //strip.setPixelColor(48, blue);
        strip.setPixelColor(49, blue);
        strip.setPixelColor(50, blue);
        strip.setPixelColor(51, blue);
        //strip.setPixelColor(56, blue);
        //strip.setPixelColor(57, blue);
        //strip.setPixelColor(58, blue);
        //strip.setPixelColor(59, blue);

        strip.setPixelColor(36, yellow);
        strip.setPixelColor(37, yellow);
        strip.setPixelColor(38, yellow);
        //strip.setPixelColor(39, yellow);
        strip.setPixelColor(44, yellow);
        strip.setPixelColor(45, yellow);
        strip.setPixelColor(46, yellow);
        //strip.setPixelColor(47, yellow);
        strip.setPixelColor(52, yellow);
        strip.setPixelColor(53, yellow);
        strip.setPixelColor(54, yellow);
        //strip.setPixelColor(55, yellow);
        //strip.setPixelColor(60, yellow);
        //strip.setPixelColor(61, yellow);
        //strip.setPixelColor(62, yellow);
        //strip.setPixelColor(63, yellow);
        strip.show();
        if (!ledState)
          break;
        delay(Speed * 5);
        break;
      case 2:
        strip.fill(off, 0, 64);
        strip.setPixelColor(0, blue);
        strip.setPixelColor(1, blue);
        strip.setPixelColor(2, blue);
        strip.setPixelColor(3, blue);
        strip.setPixelColor(4, blue);
        strip.setPixelColor(5, blue);
        strip.setPixelColor(6, blue);
        strip.setPixelColor(7, blue);
        strip.setPixelColor(8, blue);
        strip.setPixelColor(9, white);
        strip.setPixelColor(10, blue);
        strip.setPixelColor(11, blue);
        strip.setPixelColor(12, blue);
        strip.setPixelColor(13, white);
        strip.setPixelColor(14, white);
        strip.setPixelColor(15, blue);
        strip.setPixelColor(16, blue);
        strip.setPixelColor(17, blue);
        strip.setPixelColor(18, blue);
        strip.setPixelColor(19, white);
        strip.setPixelColor(20, blue);
        strip.setPixelColor(21, blue);
        strip.setPixelColor(22, blue);
        strip.setPixelColor(23, blue);
        strip.setPixelColor(24, blue);
        strip.setPixelColor(25, white);
        strip.setPixelColor(26, blue);
        strip.setPixelColor(27, blue);
        strip.setPixelColor(28, blue);
        strip.setPixelColor(29, blue);
        strip.setPixelColor(30, blue);
        strip.setPixelColor(31, blue);
        strip.setPixelColor(32, green);
        strip.setPixelColor(33, green);
        strip.setPixelColor(34, green);
        strip.setPixelColor(35, blue);
        strip.setPixelColor(36, blue);
        strip.setPixelColor(37, blue);
        strip.setPixelColor(38, blue);
        strip.setPixelColor(39, blue);
        strip.setPixelColor(40, green);
        strip.setPixelColor(41, green);
        strip.setPixelColor(42, green);
        strip.setPixelColor(43, green);
        strip.setPixelColor(44, green);
        strip.setPixelColor(45, green);
        strip.setPixelColor(46, green);
        strip.setPixelColor(47, green);
        strip.setPixelColor(48, green);
        strip.setPixelColor(49, green);
        strip.setPixelColor(50, green);
        strip.setPixelColor(51, green);
        strip.setPixelColor(52, green);
        strip.setPixelColor(53, green);
        strip.setPixelColor(54, green);
        strip.setPixelColor(55, green);
        strip.setPixelColor(56, cyan);
        strip.setPixelColor(57, blue);
        strip.setPixelColor(58, blue);
        strip.setPixelColor(59, blue);
        strip.setPixelColor(60, blue);
        strip.setPixelColor(61, blue);
        strip.setPixelColor(62, blue);
        strip.setPixelColor(63, blue);
        strip.show();
        if (!ledState)
          break;
        delay(Speed * 5);
        break;
      case 3:
        strip.fill(off, 0, 64);
        strip.fill(blue, 0, 64);
        strip.setPixelColor(8, white);
        strip.setPixelColor(32, white);
        strip.setPixelColor(35, white);
        strip.setPixelColor(11, white);
        strip.setPixelColor(18, white);
        strip.setPixelColor(26, white);
        strip.setPixelColor(49, white);
        strip.setPixelColor(50, white);
        strip.setPixelColor(51, white);
        strip.setPixelColor(52, white);
        strip.setPixelColor(53, white);
        strip.show();
        if (!ledState)
          break;
        delay(Speed * 5);
        break;
    } if (!ledState)
      break;
  }
}


void baustrahler() {
  //call 6
  strip.fill(white, 0, 64);
}

int wait(int intervall = 1000) {
  unsigned long prevMillis = 0;
  unsigned long curMillis = millis();

  if (curMillis - prevMillis >= intervall) {
    prevMillis = curMillis;
    return 1;
  }
  else
  {
    return 2;
  }
}

void call(int m) {
  switch (m) {
    case 1:
      noise();
      break;
    case 2:
      rainbow();
      break;
    case 3:
      matrix();
      break;
    case 4:
      fire();
      break;
    case 5:
      windows();
      break;
    case 6:
      baustrahler();
      break;
    default:
      for (int i = 1; i <= 8; i++) {
        switch (i) {
          case 1:
            strip.fill(off, 0, 64);
            strip.setPixelColor(11, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 2:
            strip.fill(off, 0, 64);
            strip.setPixelColor(17, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;
          case 3:
            strip.fill(off, 0, 64);
            strip.setPixelColor(33, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 4:
            strip.fill(off, 0, 64);
            strip.setPixelColor(50, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 5:
            strip.fill(off, 0, 64);
            strip.setPixelColor(52, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 6:
            strip.fill(off, 0, 64);
            strip.setPixelColor(46, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 7:
            strip.fill(off, 0, 64);
            strip.setPixelColor(30, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;

          case 8:
            strip.fill(off, 0, 64);
            strip.setPixelColor(13, red);
            strip.show();
            if (!ledState)
              break;
            delay(Speed);
            break;
        }
      }
      break;
  }
}

class colourTask : public Task {
  public:
    void setup() {
      strip.begin();
      strip.setBrightness(brightness);
      strip.show();
    }

    void loop() {
      if (ledState == 1) {
        while (ledState == 1) {
          call(pU);
          strip.setBrightness(brightness);
          strip.show();
          yield();
        }
      } else {
        strip.fill((0, 0, 0), 0, 64);
        strip.show();
      }
    }
} colourTask;

class serverTask : public Task {
  public:
    void setup() {
      // Serial port for debugging purposes
      Serial.begin(115200);

      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);

      // Connect to Wi-Fi
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
      }

      // Print ESP Local IP Address
      Serial.println(WiFi.localIP());

      initWebSocket();

      // Route for root / web page
      server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        //        request->send_P(200, "text/html", index_html, processor);
        request->send_P(200, "text/html", index_html);
      });

      // Start server
      server.begin();
    }

    void loop() {
      ws.cleanupClients();
    }
} serverTask;

void setup() {
  Serial.begin(115200);

  Scheduler.start(&serverTask);
  Scheduler.start(&colourTask);

  Scheduler.begin();
}

void loop() {}
