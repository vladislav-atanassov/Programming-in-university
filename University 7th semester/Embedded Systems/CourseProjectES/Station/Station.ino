/**
 * BLE Access Control - Station (Server with Persistent Logging)
 * ESP32 XIAO C3 with SPIFFS-based logging
 */

#include "mbedtls/md.h"

#include <Arduino.h>
#include <FS.h>
#include <NimBLEDevice.h>
#include <SPIFFS.h>
#include <time.h>

// Configuration
static NimBLEUUID SERVICE_UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static NimBLEUUID PASSWORD_CHAR_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static NimBLEUUID LOGS_CHAR_UUID("c0f0f1f2-1f0f-2f0f-3f0f-4f0f5f0f6f0f");
static NimBLEUUID CLEAR_LOGS_CHAR_UUID("d1d2d3d4-1d0d-2d0d-3d0d-4d0d5d0d6d0d");

// Access control state
enum LockState
{
    LOCKED,
    UNLOCKED
};

constexpr int PASSWORD_LENGTH    = 4;
constexpr int CHALLENGE_BYTE_LEN = 5;
constexpr int PREHASH_TEXT_LEN   = CHALLENGE_BYTE_LEN + PASSWORD_LENGTH;
constexpr int SHA256_BYTE_LEN    = 32;

LockState                               currentState    = LOCKED;
std::array<uint8_t, PASSWORD_LENGTH>    correctPassword = {'1', '2', '3', '4'};
std::array<uint8_t, CHALLENGE_BYTE_LEN> challenge;
std::array<uint8_t, PREHASH_TEXT_LEN>   prehashText;
std::array<uint8_t, SHA256_BYTE_LEN>    hashedText;
static bool                             challengeComplete = true;

// File paths
#define LOG_FILE            "/event_log.txt"
#define MAX_LOG_SIZE        10000 // Maximum log file size in bytes (10KB)
#define MAX_LINES_IN_MEMORY 100   // Maximum lines to keep in memory

// BLE Server components
// BLE Server components
static NimBLEServer         *bleServer               = nullptr;
static NimBLECharacteristic *passwordCharacteristic  = nullptr;
static NimBLECharacteristic *logsCharacteristic      = nullptr;
static NimBLECharacteristic *clearLogsCharacteristic = nullptr;
static bool                  isDeviceConnected       = false;
static bool                  wasDeviceConnected      = false;

void addLogEntry(String event, String details);
bool clearLogsFile();
void printRawBytes(const std::vector<uint8_t> &v);
std::array<uint8_t, SHA256_BYTE_LEN>
     generateSHA256(const std::array<uint8_t, PREHASH_TEXT_LEN> &text);
void handleSerialInput();

struct Sha256Hash
{
    std::array<uint8_t, SHA256_BYTE_LEN> bytes;

    Sha256Hash() = default;

    Sha256Hash(const uint8_t *hash)
    {
        std::copy(hash, hash + SHA256_BYTE_LEN, bytes.begin());
    }

    // Allow comparison
    bool operator==(const std::array<uint8_t, SHA256_BYTE_LEN> &other) const
    {
        return bytes == other;
    }
};

struct ChallengeData
{
    std::array<uint8_t, CHALLENGE_BYTE_LEN> challenge;

    ChallengeData() = default;

    ChallengeData(const uint8_t *chal)
    {
        std::copy(chal, chal + CHALLENGE_BYTE_LEN, challenge.begin());
    }
};

// Server Callbacks
class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override
    {
        isDeviceConnected = true;
        Serial.println("\n>>> REMOTE CONNECTED! <<<");
        Serial.print("Address: ");
        Serial.println(connInfo.getAddress().toString().c_str());
        Serial.println("Ready to receive password\n");

        addLogEntry("REMOTE_CONNECTED",
                    "Remote device connected: " + String(connInfo.getAddress().toString().c_str()));
    }

    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override
    {
        isDeviceConnected = false;
        Serial.println("\n>>> Remote disconnected <<<");
        addLogEntry("REMOTE_DISCONNECTED", "Remote device disconnected");
    }
};

// Password Characteristic Callbacks
class PasswordCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *pChar, NimBLEConnInfo &connInfo) override
    {
        Sha256Hash  value;
        const auto &rawResponse = pChar->getValue();
        if (rawResponse.size() >= sizeof(Sha256Hash))
        {
            value = passwordCharacteristic->getValue<Sha256Hash>(nullptr, false);
        }
        else
        {
            Serial.println("Read failed or data too short");
        }

        hashedText = generateSHA256(prehashText);
        // Verify password
        if (value == hashedText)
        {
            Serial.println("Password correct!");
            challengeComplete = true;

            // Toggle lock state
            if (currentState == LOCKED)
            {
                currentState = UNLOCKED;
                Serial.println("\n================");
                Serial.println(">>> UNLOCKED <<<");
                Serial.println("================\n");

                pChar->setValue("UNLOCKED");
            }
            else
            {
                currentState = LOCKED;
                Serial.println("\n==============");
                Serial.println(">>> LOCKED <<<");
                Serial.println("==============\n");

                pChar->setValue("LOCKED");
            }
            pChar->notify();
        }
        else
        {
            Serial.println("Password incorrect!");
            pChar->setValue("INCORRECT");
            pChar->notify();
        }
    }

    // TODO: challenge must change when station gets unlocked, not on every request like it is now
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override
    {
        if (challengeComplete)
        {
            esp_fill_random(challenge.data(), CHALLENGE_BYTE_LEN);

            // Use a local copy for the prehash calculation to keep strict integrity of the global
            // challenge
            auto tempChallenge                    = challenge;
            tempChallenge[CHALLENGE_BYTE_LEN - 2] = ~tempChallenge[CHALLENGE_BYTE_LEN - 2];

            std::copy(tempChallenge.begin(), tempChallenge.end(), prehashText.begin());
            std::copy(correctPassword.begin(), correctPassword.end(),
                      prehashText.begin() + CHALLENGE_BYTE_LEN);
            challengeComplete = false;
        }

        // Always write the current challenge so it can be read again if the previous attempt failed
        ChallengeData chal(static_cast<unsigned char *>(challenge.data()));
        pCharacteristic->setValue<ChallengeData>(chal);
    }
};

// Clear Logs Characteristic Callbacks
class ClearLogsCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *pChar, NimBLEConnInfo &connInfo) override
    {
        std::string value = pChar->getValue();

        if (value.length() > 0)
        {
            String command = String(value.c_str());
            if (command == "CLEAR")
            {
                Serial.println("\n>>> Clearing all logs <<<");
                if (clearLogsFile())
                {
                    pChar->setValue("LOGS_CLEARED");
                    addLogEntry("LOGS_CLEARED",
                                "Logs cleared by remote: " +
                                    String(connInfo.getAddress().toString().c_str()));
                }
                else
                {
                    pChar->setValue("CLEAR_FAILED");
                    Serial.println("Failed to clear logs!");
                }
                pChar->notify();
            }
        }
    }
};

static ServerCallbacks    serverCallbacks;
static PasswordCallbacks  passwordCallbacks;
static ClearLogsCallbacks clearLogsCallbacks;

// SPIFFS Logging Functions
bool initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS initialization failed!");
        return false;
    }

    Serial.println("SPIFFS initialized");

    // Check if log file exists, create if not
    if (!SPIFFS.exists(LOG_FILE))
    {
        Serial.println("Creating new log file...");
        File file = SPIFFS.open(LOG_FILE, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to create log file!");
            return false;
        }
        file.println("=== Security System Log File ===");
        file.println("System initialized: " + String(millis()) + "ms");
        file.close();
    }

    // Print file info
    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (file)
    {
        Serial.print("Log file size: ");
        Serial.print(file.size());
        Serial.println(" bytes");
        file.close();
    }

    return true;
}

void addLogEntry(String event, String details)
{
    // Create timestamp
    unsigned long currentMillis = millis();
    unsigned long days          = currentMillis / 86400000;
    unsigned long hours         = (currentMillis % 86400000) / 3600000;
    unsigned long minutes       = (currentMillis % 3600000) / 60000;
    unsigned long seconds       = (currentMillis % 60000) / 1000;

    // Format log line
    String logLine = "[";
    if (days > 0)
    {
        logLine += String(days) + "d ";
    }
    logLine += String(hours) + ":";
    if (minutes < 10)
        logLine += "0";
    logLine += String(minutes) + ":";
    if (seconds < 10)
        logLine += "0";
    logLine += String(seconds) + "] ";
    logLine += event + " - " + details;

    // Append to file
    File file = SPIFFS.open(LOG_FILE, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open log file for writing!");
        return;
    }

    file.println(logLine);
    file.close();

    // Log rotation: check file size
    checkLogRotation();

    // Print to serial
    Serial.print("[LOG] ");
    Serial.println(logLine);
}

void checkLogRotation()
{
    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (!file)
        return;

    size_t fileSize = file.size();
    file.close();

    if (fileSize > MAX_LOG_SIZE)
    {
        Serial.println("Log file too large, performing rotation...");
        rotateLogFile();
    }
}

void rotateLogFile()
{
    // Create backup filename with timestamp
    String backupFile = "/event_log_backup_" + String(millis()) + ".txt";

    // Rename current log file to backup
    if (SPIFFS.rename(LOG_FILE, backupFile.c_str()))
    {
        Serial.println("Log file rotated to: " + backupFile);

        // Create new log file
        File file = SPIFFS.open(LOG_FILE, FILE_WRITE);
        if (file)
        {
            file.println("=== New log file created after rotation ===");
            file.println("Previous log saved as: " + backupFile);
            file.println("Rotation time: " + String(millis()) + "ms");
            file.close();
        }
    }
    else
    {
        Serial.println("Failed to rotate log file!");

        // If rotation fails, just truncate the file
        File file = SPIFFS.open(LOG_FILE, FILE_WRITE);
        if (file)
        {
            file.println("=== Log file truncated ===");
            file.println("Truncation time: " + String(millis()) + "ms");
            file.close();
            Serial.println("Log file truncated instead");
        }
    }
}

String readLogsFromFile(int maxLines = MAX_LINES_IN_MEMORY)
{
    if (!SPIFFS.exists(LOG_FILE))
    {
        return "Log file not found";
    }

    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (!file)
    {
        return "Failed to open log file";
    }

    // Count total lines
    int totalLines = 0;
    while (file.available())
    {
        if (file.read() == '\n')
            totalLines++;
    }
    file.seek(0);

    // Read last maxLines
    String result = "=== Security System Logs ===\n";
    result += "Total log entries: " + String(totalLines) + "\n";
    result += "File size: " + String(file.size()) + " bytes\n";
    result += "Showing last " + String(min(maxLines, totalLines)) + " entries:\n\n";

    if (totalLines <= maxLines)
    {
        // Read entire file
        while (file.available())
        {
            result += (char) file.read();
        }
    }
    else
    {
        // Skip to the last maxLines
        int linesToSkip = totalLines - maxLines;
        int currentLine = 0;
        while (file.available() && currentLine < linesToSkip)
        {
            if (file.read() == '\n')
                currentLine++;
        }

        // Read the remaining lines
        while (file.available())
        {
            result += (char) file.read();
        }
    }

    file.close();
    return result;
}

bool clearLogsFile()
{
    if (!SPIFFS.exists(LOG_FILE))
    {
        return true; // File doesn't exist, so it's already "cleared"
    }

    if (SPIFFS.remove(LOG_FILE))
    {
        // Create new empty log file
        File file = SPIFFS.open(LOG_FILE, FILE_WRITE);
        if (file)
        {
            file.println("=== Log file cleared ===");
            file.println("Cleared at: " + String(millis()) + "ms");
            file.close();
            return true;
        }
    }
    return false;
}

void printFileInfo()
{
    if (!SPIFFS.exists(LOG_FILE))
    {
        Serial.println("No log file exists");
        return;
    }

    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (!file)
    {
        Serial.println("Failed to open log file for reading");
        return;
    }

    int lineCount      = 0;
    int lastUnlockLine = -1;
    int lastLockLine   = -1;
    int lastDeniedLine = -1;
    int currentLine    = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        currentLine++;

        if (line.indexOf("UNLOCKED") != -1)
            lastUnlockLine = currentLine;
        if (line.indexOf("LOCKED") != -1)
            lastLockLine = currentLine;
        if (line.indexOf("ACCESS_DENIED") != -1)
            lastDeniedLine = currentLine;

        lineCount++;
    }
    file.close();

    Serial.println("\n=== Log File Statistics ===");
    Serial.print("Total lines: ");
    Serial.println(lineCount);
    Serial.print("File size: ");
    Serial.print(SPIFFS.open(LOG_FILE, FILE_READ).size());
    Serial.println(" bytes");
    Serial.print("Last UNLOCKED at line: ");
    Serial.println(lastUnlockLine);
    Serial.print("Last LOCKED at line: ");
    Serial.println(lastLockLine);
    Serial.print("Last ACCESS_DENIED at line: ");
    Serial.println(lastDeniedLine);
    Serial.println("==========================\n");
}

void printRawBytes(const std::vector<uint8_t> &v)
{
    Serial.print(String(reinterpret_cast<const char *>(v.data())));
}

std::array<uint8_t, SHA256_BYTE_LEN>
generateSHA256(const std::array<uint8_t, PREHASH_TEXT_LEN> &text)
{
    std::array<uint8_t, SHA256_BYTE_LEN> hash;

    mbedtls_md_context_t ctx;
    mbedtls_md_type_t    md_type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, text.data(), text.size());
    mbedtls_md_finish(&ctx, static_cast<unsigned char *>(hash.data()));
    mbedtls_md_free(&ctx);

    return hash;
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=================================");
    Serial.println("BLE Access Control - STATION");
    Serial.println("  WITH PERSISTENT LOGGING");
    Serial.println("=================================\n");

    // Initialize SPIFFS for persistent storage
    Serial.println("Initializing SPIFFS...");
    if (!initSPIFFS())
    {
        Serial.println("Critical: SPIFFS initialization failed!");
        Serial.println("System cannot store logs persistently.");
    }

    // Add startup log entry
    addLogEntry("SYSTEM_START", "Station initialized");

    // Initialize NimBLE
    Serial.println("Initializing BLE...");
    NimBLEDevice::init("Station");

    // Set power level
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    // Create BLE Server
    Serial.println("Creating BLE Server...");
    bleServer = NimBLEDevice::createServer();
    bleServer->setCallbacks(&serverCallbacks);

    // Create service
    Serial.println("Creating BLE Service...");
    NimBLEService *pService = bleServer->createService(SERVICE_UUID);

    // Create password characteristic
    Serial.println("Creating Password Characteristic...");
    passwordCharacteristic = pService->createCharacteristic(
        PASSWORD_CHAR_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    passwordCharacteristic->setValue("LOCKED");
    passwordCharacteristic->setCallbacks(&passwordCallbacks);

    // Create logs characteristic (read-only)
    Serial.println("Creating Logs Characteristic...");
    logsCharacteristic = pService->createCharacteristic(LOGS_CHAR_UUID, NIMBLE_PROPERTY::READ);
    logsCharacteristic->setValue("Logs available - read to view");

    // Create clear logs characteristic (write-only)
    Serial.println("Creating Clear Logs Characteristic...");
    clearLogsCharacteristic = pService->createCharacteristic(
        CLEAR_LOGS_CHAR_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    clearLogsCharacteristic->setValue("Send 'CLEAR' to clear logs");
    clearLogsCharacteristic->setCallbacks(&clearLogsCallbacks);

    // Start service
    Serial.println("Starting service...");
    pService->start();

    // Configure advertising
    Serial.println("Configuring advertising...");
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setName("Station");
    pAdvertising->enableScanResponse(true);

    // Start advertising
    Serial.println("Starting advertising...");
    pAdvertising->start();

    Serial.println("\nStation with Persistent Logging initialized!");
    Serial.println("Logs are stored in SPIFFS and will survive power cycles");
    Serial.println("Log file: " + String(LOG_FILE));
    Serial.println("Max log size: " + String(MAX_LOG_SIZE) + " bytes");
    Serial.println("\nWaiting for Remote to connect...");
    Serial.println("Current state: LOCKED\n");

    // Print initial file info
    printFileInfo();
}

void loop()
{
    // Handle advertising restart after disconnect
    if (!isDeviceConnected && wasDeviceConnected)
    {
        delay(500);
        NimBLEDevice::startAdvertising();
        Serial.println("Advertising restarted - waiting for Remote");
        wasDeviceConnected = isDeviceConnected;
    }

    // Handle new connection
    if (isDeviceConnected && !wasDeviceConnected)
    {
        wasDeviceConnected = isDeviceConnected;

        // Update logs characteristic when device connects
        if (logsCharacteristic != nullptr)
        {
            logsCharacteristic->setValue(readLogsFromFile(50).c_str());
        }
    }

    handleSerialInput();

    // Print status periodically
    static unsigned long lastStatusPrint = 0;
    if (millis() - lastStatusPrint > 30000)
    {
        lastStatusPrint = millis();
        Serial.print("[Status] State: ");
        Serial.print(currentState == LOCKED ? "LOCKED" : "UNLOCKED");
        Serial.print(" | Remote: ");
        Serial.print(isDeviceConnected ? "CONNECTED" : "DISCONNECTED");
        Serial.print(" | Logs: ");
        if (SPIFFS.exists(LOG_FILE))
        {
            File file = SPIFFS.open(LOG_FILE, FILE_READ);
            if (file)
            {
                Serial.print(file.size());
                Serial.println(" bytes");
                file.close();
            }
        }
        else
        {
            Serial.println("No file");
        }
    }

    delay(100);
}

void handleSerialInput()
{
    // Handle serial commands
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input == "logs")
        {
            Serial.println("\n=== READING LOG FILE ===");
            Serial.println(readLogsFromFile(100));
            Serial.println("=========================");
        }
        else if (input == "tail")
        {
            Serial.println("\n=== LAST 10 LOG ENTRIES ===");
            String logs = readLogsFromFile(10);
            // Extract only the last entries (skip header)
            int start = logs.indexOf("Showing last");
            if (start != -1)
            {
                Serial.println(logs.substring(start));
            }
        }
        else if (input == "fileinfo")
        {
            printFileInfo();
        }
        else if (input == "clearlogs")
        {
            if (clearLogsFile())
            {
                Serial.println("Log file cleared successfully");
                addLogEntry("LOGS_CLEARED", "Logs cleared via serial command");
            }
            else
            {
                Serial.println("Failed to clear logs!");
            }
        }
        else if (input == "test")
        {
            // Simulate events for testing
            if (currentState == LOCKED)
            {
                currentState = UNLOCKED;
                addLogEntry("UNLOCKED", "Test unlock via serial");
                Serial.println("Test UNLOCKED event logged");
            }
            else
            {
                currentState = LOCKED;
                addLogEntry("LOCKED", "Test lock via serial");
                Serial.println("Test LOCKED event logged");
            }
        }
        else if (input == "status")
        {
            Serial.print("\nCurrent State: ");
            Serial.println(currentState == LOCKED ? "LOCKED" : "UNLOCKED");
            Serial.print("Remote Connected: ");
            Serial.println(isDeviceConnected ? "YES" : "NO");
            if (SPIFFS.exists(LOG_FILE))
            {
                File file = SPIFFS.open(LOG_FILE, FILE_READ);
                if (file)
                {
                    Serial.print("Log file size: ");
                    Serial.print(file.size());
                    Serial.println(" bytes");
                    file.close();
                }
            }
        }
        else if (input == "format")
        {
            Serial.println("\n=== FORMATTING SPIFFS ===");
            Serial.println("WARNING: This will erase ALL logs!");
            Serial.println("Type 'CONFIRM' to proceed: ");

            // Wait for confirmation
            unsigned long start = millis();
            while (millis() - start < 10000)
            {
                if (Serial.available())
                {
                    String confirm = Serial.readStringUntil('\n');
                    confirm.trim();
                    if (confirm == "CONFIRM")
                    {
                        SPIFFS.format();
                        Serial.println("SPIFFS formatted successfully");
                        initSPIFFS();
                        addLogEntry("SYSTEM_START", "SPIFFS formatted and reinitialized");
                        break;
                    }
                    else
                    {
                        Serial.println("Format cancelled");
                        break;
                    }
                }
                delay(10);
            }
        }
    }
}
