#include <Arduino.h>

#include "version.h"

#include "Application.h"
#include "CryptoService.h"
#include "ManifestFetcher.h"
#include "OTAUpdateService.h"
#include "WifiManager.h"

static WifiManager wifi;
static ManifestFetcher manifestFetcher;
static CryptoService crypto;
static OTAUpdateService otaService(wifi, manifestFetcher, crypto);
static Application app;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("==============================================");
    Serial.printf(" Secure OTA Firmware — v%d.%d.%d (code %u)\n", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR,
                  FIRMWARE_VERSION_PATCH, static_cast<uint32_t>(FIRMWARE_VERSION));
    Serial.println("==============================================");

    otaService.markCurrentFirmwareValid();

    if (!wifi.connect())
    {
        Serial.println("[Main] WiFi failed — will retry in loop");
    }

    app.setup();
}

void loop()
{
    wifi.ensureConnected();
    otaService.checkForUpdate();
    app.loop();
}
