#include "mbedtls/md.h"

#include <Arduino.h>
#include <Keypad.h>
#include <NimBLEDevice.h>

// Configuration (must match Station)
static NimBLEUUID SERVICE_UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static NimBLEUUID PASSWORD_CHAR_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static NimBLERemoteCharacteristic *passwordCharacteristic = nullptr;
static NimBLEClient               *bleClient              = nullptr;
static bool                        isServerConnected      = false;

constexpr int PASSWORD_LENGTH    = 4;
constexpr int CHALLENGE_BYTE_LEN = 5;
constexpr int PREHASH_TEXT_LEN   = CHALLENGE_BYTE_LEN + PASSWORD_LENGTH;
constexpr int SHA256_BYTE_LEN    = 32;

std::array<uint8_t, PREHASH_TEXT_LEN> prehashText;

// Keypad configuration
const byte ROWS             = 4;
const byte COLS             = 3;
char       keys[ROWS][COLS] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};

// GPIO pins for XIAO C3 - Updated to your actual wiring
byte rowPins[ROWS] = {2, 4, 5, 7}; // GPIO 2, 4, 5, 7
byte colPins[COLS] = {8, 9, 10};   // GPIO 8, 9, 10

struct Sha256Hash
{
    std::array<uint8_t, SHA256_BYTE_LEN> bytes;

    Sha256Hash() = default;

    Sha256Hash(const uint8_t *hash)
    {
        std::copy(hash, hash + SHA256_BYTE_LEN, bytes.begin());
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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

template <typename T, size_t N>
void printArrayHex(const std::array<T, N> &arr, const char *separator = " ")
{
    for (size_t i = 0; i < N; ++i)
    {
        if constexpr (std::is_same<T, uint8_t>::value)
        {
            // Print uint8_t as 2-digit hex
            if (arr[i] < 16)
                Serial.print("0");
            Serial.print(arr[i], HEX);
        }
        else
        {
            Serial.print(arr[i], HEX);
        }
        if (i < N - 1)
            Serial.print(separator);
    }
    Serial.println();
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

void sendPassword()
{
    if (!isServerConnected || passwordCharacteristic == nullptr)
    {
        Serial.println("Not connected!");
        return;
    }

    Serial.print("Sending password: ");

    if (passwordCharacteristic->readValue())
    {
        const auto &raw = passwordCharacteristic->getValue();
        if (raw.size() >= sizeof(ChallengeData))
        {
            ChallengeData value = passwordCharacteristic->getValue<ChallengeData>(nullptr, false);
            printArrayHex(value.challenge);
            Serial.println("======================================================================="
                           "===========================");

            value.challenge[CHALLENGE_BYTE_LEN - 2] = ~value.challenge[CHALLENGE_BYTE_LEN - 2];
            std::copy(value.challenge.begin(), value.challenge.end(), prehashText.begin());
        }
        else
        {
            Serial.println("Read failed or data too short");
            Serial.println(raw.size());
        }
    }
    else
    {
        Serial.println("Read failed");
    }
    Sha256Hash hash(static_cast<const uint8_t *>(generateSHA256(prehashText).data()));
    printArrayHex(hash.bytes);
    Serial.println("==============================================================================="
                   "===================");

    passwordCharacteristic->writeValue<Sha256Hash>(hash, false);

    prehashText = {0};
}

void handleKeypadInput()
{
    char       key     = keypad.getKey();
    static int counter = PASSWORD_LENGTH;
    if (!key)
        return;

    Serial.print("Key: ");
    Serial.println(key);

    if (key == '*')
    {
        prehashText = {0};
        counter     = PASSWORD_LENGTH;
        Serial.println("Password cleared");
    }
    else if (key == '#')
    {
        if (counter == 0)
        {
            sendPassword();
            counter = PASSWORD_LENGTH;
        }
        else
            Serial.println("No password entered");
    }
    else if (key >= '0' && key <= '9')
    {
        if (counter > 0)
        {
            prehashText[PREHASH_TEXT_LEN - counter] = static_cast<unsigned char>(key);
            --counter;
            Serial.println("Digit added");
        }
    }
}

bool connectToStation()
{
    NimBLEScan       *scan    = NimBLEDevice::getScan();
    NimBLEScanResults results = scan->getResults(5000, false);

    for (int i = 0; i < results.getCount(); i++)
    {
        const NimBLEAdvertisedDevice *device = results.getDevice(i);

        if (device->isAdvertisingService(SERVICE_UUID))
        {
            bleClient = NimBLEDevice::createClient();

            if (!bleClient->connect(device))
            {
                Serial.println("Connection failed");
                return false;
            }

            NimBLERemoteService *service = bleClient->getService(SERVICE_UUID);
            if (!service)
                return false;

            passwordCharacteristic = service->getCharacteristic(PASSWORD_CHAR_UUID);
            if (!passwordCharacteristic)
                return false;

            Serial.println("Connected to station!");
            return true;
        }
    }

    return false;
}

void setup()
{
    Serial.begin(115200);
    NimBLEDevice::init("Remote");
}

void loop()
{
    if (!isServerConnected)
    {
        Serial.println("Scanning for station...");
        isServerConnected = connectToStation();
        if (!isServerConnected)
        {
            delay(2000);
            return;
        }
    }

    handleKeypadInput();

    if (!bleClient->isConnected())
    {
        Serial.println("Lost connection!");
        isServerConnected = false;
        delay(500);
    }
}
