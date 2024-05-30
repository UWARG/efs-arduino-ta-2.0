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
