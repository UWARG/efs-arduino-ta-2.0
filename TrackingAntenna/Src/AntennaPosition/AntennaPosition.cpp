#include "AntennaPosition.hpp"
#include <Wire.h>
#include "../Config/Config.hpp"

bool AntennaPosition::beginAltimeter() {
  Wire.begin();

  if (altimeter_.begin() == false) {
      PDEBUG("MS5611 not found!\n");
      return false;
  }

  altimeter_.setOversampling(OSR_ULTRA_HIGH); // OSR_ULTRA_HIGH -> 8.22 millis of samples

  return true;
}

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

bool AntennaPosition::getAltimeterPosition() {
  int result = altimeter_.read();
  if (result != MS5611_READ_OK)
  {
    PDEBUG("Error in read: ");
    PDEBUG(result);
    PDEBUG("\n");
    return false;
  }

  PDEBUG("Temperature: ");
  PDEBUG(altimeter_.getTemperature());
  PDEBUG("\tPressure: ");
  PDEBUG(altimeter_.getPressure());
  PDEBUG("\n");

  altitude_ = getAltitudeAboveSeaLevel();

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

    // altitude_ = GPS_.getAltitudeMSL() / 1000.0; // Altitude above Mean Sea Level
    // PDEBUG(F(" Alt: "));
    // PDEBUG(altitude_);
    // PDEBUG(F(" (m)"));

    byte SIV = GPS_.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    PDEBUG("\n");

    return SIV > MIN_SATELLITES;
}

// Adapted from Fola Fatola https://github.com/UWARG/efs-zeropilot-3.5/pull/53
float AntennaPosition::getAltitudeAboveSeaLevel(){
	float pressure = altimeter_.getPressure();

	const float CURRENT_PRESSURE = pressure * 100.0f;
	const float EXPONENT = (log(CURRENT_PRESSURE) - log(REFERENCE_PRESSURE)) / EXP_GMRL;

	float altitudeAboveSeaLevel = REFERENCE_TEMP / TEMP_LAPSE_RATE * (1 - pow(M_E, EXPONENT));

  PDEBUG("Altitude Above Sea Level: ");
  PDEBUG(altitudeAboveSeaLevel);
  PDEBUG("\n");

	return altitudeAboveSeaLevel;
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