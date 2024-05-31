#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "MS5611.h"

class AntennaPosition {
   private:
    SFE_UBLOX_GNSS GPS_;
    MS5611 altimeter_ {0x77};
    float latitude_;
    float longitude_;
    float altitude_;

   public:
    bool beginGPS();
    bool beginAltimeter();
    bool getGPSPosition();
    bool getAltimeterPosition();
    float latitude();
    float longitude();
    float altitude();
    float northBearing();
};

//These constants are for the barometric formula
//https://en.wikipedia.org/wiki/Barometric_formula
constexpr float REFERENCE_TEMP = 288.15f; //Ref temperature in Kelvins.
constexpr float TEMP_LAPSE_RATE = 0.0065f;
constexpr float EXP_GMRL = 5.2558;
constexpr float REFERENCE_PRESSURE = 101325.0f; /* in Pa */