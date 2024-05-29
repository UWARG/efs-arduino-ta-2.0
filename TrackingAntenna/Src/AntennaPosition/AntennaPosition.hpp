#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "MS5611.h"

#define GPS_TX 0
#define GPS_RX 1

// also handle the barometer
class AntennaPosition {
   private:
    SFE_UBLOX_GNSS GPS_;
    MS5611 altimeter_(0x77);
    float latitude_;
    float longitude_;
    float altitude_;

   public:
    void beginGPS();
    bool getPosition();
    float latitude();
    float longitude();
    float altitude();
    float northBearing();
};
