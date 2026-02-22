#include "Application.h"

#include <Arduino.h>

static constexpr unsigned long PRINT_INTERVAL_MS = 2000;

void Application::setup()
{
    Serial.println("[App] Application started");
}

void Application::loop()
{
    unsigned long now = millis();

    if (now - lastPrintMs_ >= PRINT_INTERVAL_MS)
    {
        lastPrintMs_ = now;
        Serial.printf("[App] Hello from application — uptime %lu s\n", now / 1000);
    }
}
