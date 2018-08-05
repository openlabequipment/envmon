#include <Arduino.h>

#include <Ticker.h>
#include <Streaming.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

#include <WiFiManager.h>

Ticker statusLedTicker;

void tickStatusLed()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void cb_wifiConfig(WiFiManager *wifi_manager)
{
    Serial << F("[ok] entered AP mode for WAP connection config\n");
    Serial << F("... ip is: ") << WiFi.softAPIP() << '\n';

    // faster blinky
    statusLedTicker.attach(0.2, tickStatusLed);
}

void setup()
{
    Serial.begin(115200);

    // led pin as output
    pinMode(LED_BUILTIN, OUTPUT);
    // start off ~slowly~ doin the blinky
    statusLedTicker.attach(0.6, tickStatusLed);

    // auto-reconnect
    WiFi.setAutoReconnect(true);

    WiFiManager wifi_manager;
    wifi_manager.setDebugOutput(false);
    wifi_manager.setAPCallback(cb_wifiConfig);
    if (!wifi_manager.autoConnect("envmon_configure", "password"))
    {
        Serial << F("[fail] didn't connect to WAP or hit timeout\n");
        Serial << F("... rebooting\n");

        delay(1000);
        ESP.reset();
    }

    Serial << F("[ok] connected to wifi\n");
    Serial << F("... ip: ") << WiFi.localIP();
    statusLedTicker.detach();
    digitalWrite(LED_BUILTIN, HIGH); // turn off LED

    if (!MDNS.begin("envmon")) {
        Serial << F("[fail] couldn't set up mdns responder\n");
        Serial << F("... rebooting\n");

        delay(1000);
        ESP.reset();
    }
    Serial << F("[ok] mDNS responder started\n"); 
}

void loop()
{
}