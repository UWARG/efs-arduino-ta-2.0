#include "Arduino.h"
#include <SPI.h>

// comment the next line out to disable debug prints
#define DEBUG

#ifdef DEBUG
    #define PDEBUG(x) Serial.print(x)
#else
    #define PDEBUG(x) do {} while(0)
#endif

// Servo Pins
#define YAW_PWM 12
#define PITCH_PWM 13

// Airlift Pins
#define WIFI_SPI       SPI  // The default SPI pins
#define WIFI_SPI_CS    40   // Chip select pin
#define WIFI_SPI_ACK    42   // a.k.a BUSY or READY pin
#define WIFI_RESETN   44   // Reset pin
#define WIFI_GPIO0   -1   // Not connected

// Servo Limits and Angles
#define YAW_START_MICROSECONDS 880
#define YAW_END_MICROSECONDS 2200

#define YAW_START_ANGLE 175
#define YAW_END_ANGLE -175

#define PITCH_START_MICROSECONDS 1000
#define PITCH_END_MICROSECONDS 2000

#define MIN_SATELLITES 5 // sets the minimum number of satellites that need to be detected for antenna get position to return true

#define PACKET_BUFFER_SIZE 512 // if significantly changing this value, make sure iterator data types and FreeRTOS delays are sufficient for it

#define LOCAL_PORT 16550

// disable GPS to use ANTENNA_ values instead
// #define GPS

#ifndef GPS
    #define ANTENNA_LATITUDE 43.47360952;
    #define ANTENNA_LONGITUDE -80.5401446;
    #define ANTENNA_ALTITUDE 330;
#endif


// disable WIFI to use DRONE_ values instead
#define WIFI

#ifndef WIFI
    #define DRONE_LATITUDE 43.47360952;
    #define DRONE_LONGITUDE -80.5401446;
    #define DRONE_ALTITUDE 330;
#endif
