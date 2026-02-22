#include "WifiManager.h"

#include "config.h"

#include <Arduino.h>
#include <WiFi.h>

static constexpr unsigned long WIFI_TIMEOUT_MS     = 15000;
static constexpr unsigned long WIFI_RETRY_DELAY_MS = 500;

bool WifiManager::connect()
{
    Serial.print("[WiFi] Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > WIFI_TIMEOUT_MS)
        {
            Serial.println("\n[WiFi] Connection timeout!");
            return false;
        }
        delay(WIFI_RETRY_DELAY_MS);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("[WiFi] Connected — IP: ");
    Serial.println(WiFi.localIP());
    return true;
}

bool WifiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

void WifiManager::ensureConnected()
{
    if (!isConnected())
    {
        Serial.println("[WiFi] Disconnected — reconnecting...");
        connect();
    }
}
