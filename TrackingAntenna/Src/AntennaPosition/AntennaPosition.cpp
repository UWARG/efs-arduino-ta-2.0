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

  // Grab the sensor's altitude above sea level while on some surface and assign it to base_elev_
	// Subtract this from measurements at greater heights for the height above that initial surface.
	for (int i = 0; i < 100; i++) { //take a few measurements to make sure they are accurate/consistent.
		// setBaseElevation();
	}

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

  return true;
  
}

bool AntennaPosition::getGPSPosition() {

//     uint32_t timer = millis();

//   do {
//     char c = GPS.read();
//     if (GPS.newNMEAreceived()) {
//     Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
//     if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
//       continue; // we can fail to parse a sentence in which case we should just wait for another
//     }

//     // approximately every 2 seconds or so, print out the current stats
//     if (millis() - timer > 2000){
//       timer = millis(); // reset the timer
//       sats = GPS.satellites;
//       Serial.println("Getting GPS, repeating...");
//       Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
//     }
  
//   } while(sats < 7);

//   AAT_LAT = GPS.latitude_fixed /10000000.0;
//   AAT_LON = GPS.longitude_fixed /10000000.0;

//   return true;


    // if (GPS.getPVT() == true) {
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

    //     return true;
    // } else {
    //     return false;
    // }

    return SIV > MIN_SATELLITES;
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