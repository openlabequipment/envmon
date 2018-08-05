#include <Arduino.h>

#include <Ticker.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include <WiFiManager.h>

Ticker statusLedTicker;

void tickStatusLed()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void cb_wifiConfig(WiFiManager *wifi_manager)
{
    Serial.println("[ok] entered AP mode for WAP connection config");
    Serial.print("... ip is: ");
    Serial.println(WiFi.softAPIP());

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

    WiFiManager wifi_manager;
    wifi_manager.setDebugOutput(false);
    wifi_manager.setAPCallback(cb_wifiConfig);
    if (!wifi_manager.autoConnect("envmon_configure", "password"))
    {
        Serial.println("[fail] didn't connect to WAP or hit timeout");

        ESP.reset();
    }

    Serial.println("[ok] connected to wifi");
    statusLedTicker.detach();
    digitalWrite(LED_BUILTIN, HIGH); // turn off LED
}

void loop()
{
}