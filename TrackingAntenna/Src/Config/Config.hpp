#include "Arduino.h"

// comment the next line out to disable debug prints
#define DEBUG

#ifdef DEBUG
    #define PDEBUG(x) Serial.print(x)
#else
    #define PDEBUG(x) do {} while(0)
#endif

#define MavlinkSerial Serial

#define PITCH_PWM A2
#define YAW_PWM A3

#define YAW_START_POS 740
#define YAW_END_POS 2260
#define YAW_MID_POS (YAW_START_POS + YAW_END_POS)/2

#define MIN_SATELLITES 5 // sets the minimum number of satellites that need to be detected for antenna get position to return true

// make all of this constant case (capitals with underscores)