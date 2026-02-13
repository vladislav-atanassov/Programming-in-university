#include <Arduino.h>

#include "config.h"
#include "manifest.h"
#include "ota_updater.h"
#include "version.h"
#include "wifi_manager.h"


// ---------------------------------------------------------------------------
// Timing
// ---------------------------------------------------------------------------
static unsigned long last_ota_check_ms = 0;

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000); // Allow serial monitor to connect

  Serial.println();
  Serial.println("==============================================");
  Serial.printf(" Secure OTA Firmware — v%d.%d.%d (code %u)\n",
                FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR,
                FIRMWARE_VERSION_PATCH, (uint32_t)FIRMWARE_VERSION);
  Serial.println("==============================================");

  // Mark current firmware as valid (cancels rollback timer)
  ota_mark_valid();

  // Connect to WiFi
  if (!wifi_init()) {
    Serial.println("[Main] WiFi failed — will retry in loop");
  }
}

// ---------------------------------------------------------------------------
// Loop
// ---------------------------------------------------------------------------
void loop() {
  unsigned long now = millis();

  // Reconnect WiFi if lost
  if (!wifi_is_connected()) {
    Serial.println("[Main] WiFi disconnected — reconnecting...");
    wifi_init();
    return;
  }

  // Periodic OTA check
  if (now - last_ota_check_ms >= (unsigned long)OTA_CHECK_INTERVAL_S * 1000UL) {
    last_ota_check_ms = now;

    Serial.println();
    Serial.println("[Main] ---- OTA check ----");

    FirmwareManifest manifest;
    if (manifest_fetch_and_parse(MANIFEST_URL, manifest)) {
      ota_perform_update(manifest);
      // If update succeeded, the device has rebooted.
      // If it returned, no update was needed or an error occurred.
    } else {
      Serial.println("[Main] Manifest fetch failed");
    }
  }

  // Heartbeat log every 30 seconds
  static unsigned long last_heartbeat = 0;
  if (now - last_heartbeat >= 30000UL) {
    last_heartbeat = now;
    Serial.printf(
        "[Main] Heartbeat — uptime %lu s, firmware v%u, free heap %u bytes\n",
        now / 1000, (uint32_t)FIRMWARE_VERSION, ESP.getFreeHeap());
  }
}