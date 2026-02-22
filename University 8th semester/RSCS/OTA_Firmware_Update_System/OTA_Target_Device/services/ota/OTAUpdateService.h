#ifndef OTA_UPDATE_SERVICE_H
#define OTA_UPDATE_SERVICE_H

#include <Ticker.h>

#include "CryptoService.h"
#include "ManifestFetcher.h"
#include "WifiManager.h"

class OTAUpdateService
{
public:
    OTAUpdateService(WifiManager& wifi, ManifestFetcher& manifestFetcher, CryptoService& crypto);

    void markCurrentFirmwareValid();
    void checkForUpdate();

private:
    WifiManager& wifi_;
    ManifestFetcher& manifestFetcher_;
    CryptoService& crypto_;

    Ticker otaCheckTicker_;
    volatile bool checkPendingUpdate_ = true;

    static constexpr size_t CHUNK_SIZE             = 4096;
    static constexpr size_t VERSION_BYTES_LEN      = sizeof(uint32_t);
    static constexpr unsigned long REBOOT_DELAY_MS = 1000;

    bool performUpdate(const FirmwareManifest& manifest);
    void startPeriodicCheck();

    template<typename ChunkCallback>
    bool httpsStreamDownload(const String& url, size_t expected_size, ChunkCallback on_chunk);
};

#endif // OTA_UPDATE_SERVICE_H
