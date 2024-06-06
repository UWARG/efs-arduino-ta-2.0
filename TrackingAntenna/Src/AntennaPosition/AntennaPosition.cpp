#include "AntennaPosition.hpp"
#include <Wire.h>
#include "../Config/Config.hpp"

bool AntennaPosition::beginGPS() {
    PDEBUG("Waiting for GPS fix... \n");

    Wire.begin();

    if (GPS_.begin() == false) //Connect to the u-blox module using Wire port
    {
        PDEBUG(F("u-blox GPS not detected. Retrying... \n"));
        return false;
    }

    GPS_.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); //Set the I2C port to output both NMEA and UBX messages
    GPS_.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
    
    // these were with the old code, don't know how to implement these yet
    /* GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    GPS.sendCommand(PGCMD_ANTENNA); */
    PDEBUG("GPS: connected through I2C \n");

    return true;
}

bool AntennaPosition::getGPSPosition() {

    latitude_ = GPS_.getLatitude() / 10000000.0;;
    PDEBUG(F("Lat: "));
    PDEBUG(latitude_);

    longitude_ = GPS_.getLongitude() / 10000000.0;
    PDEBUG(F(" Long: "));
    PDEBUG(longitude_);
    PDEBUG(F(" (degrees)"));

    altitude_ = GPS_.getAltitudeMSL() / 1000.0; // Altitude above Mean Sea Level
    PDEBUG(F(" Alt: "));
    PDEBUG(altitude_);
    PDEBUG(F(" (m)"));

    byte SIV = GPS_.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    PDEBUG("\n");

    return SIV > MIN_SATELLITES;

    // E7 Bay coordinates
    // latitude_ = 43.47360952;
    // longitude_ = -80.5401446;
    // altitude_ = 330;
    // return true;
}

float AntennaPosition::latitude() {
    return latitude_;
}

float AntennaPosition::longitude() {
    return longitude_;
}

float AntennaPosition::altitude() {
    return altitude_;
}

float AntennaPosition::northBearing() {
    return 0;
    // return northBearing_; // nothing getting this value currently
}