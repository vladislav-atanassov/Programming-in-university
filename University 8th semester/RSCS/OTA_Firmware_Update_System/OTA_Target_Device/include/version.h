#ifndef VERSION_H
#define VERSION_H

#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 1
#define FIRMWARE_VERSION_PATCH 0

// Encoded as integer for comparison: MAJOR*10000 + MINOR*100 + PATCH
// e.g., v1.2.3 = 10203
#define FIRMWARE_VERSION                                                       \
  (FIRMWARE_VERSION_MAJOR * 10000 + FIRMWARE_VERSION_MINOR * 100 +             \
   FIRMWARE_VERSION_PATCH)

#endif // VERSION_H
