#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

/**
 * Connect to WiFi using credentials from config.h.
 * Blocks until connected or timeout.
 * @return true if connected successfully
 */
bool wifi_init();

/**
 * @return true if WiFi is currently connected
 */
bool wifi_is_connected();

#endif // WIFI_MANAGER_H
