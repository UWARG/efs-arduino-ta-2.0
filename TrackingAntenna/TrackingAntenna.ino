#include "Src/AntennaPosition/AntennaPosition.hpp"
#include "Src/DronePosition/DronePosition.hpp"
#include "Src/AntennaDynamics/AntennaDynamics.hpp"
#include "Src/Util/Util.hpp"
#include "Src/Config/Config.hpp"

AntennaPosition antennaPos{};
DronePosition dronePos{};
AntennaDynamics antennaDyn{};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while(!Serial);

  while (!antennaPos.beginGPS()) {
    PDEBUG("Could not connect to GPS, retrying... \n");
    delay(1000);
  }

  while (!antennaPos.getGPSPosition()) {
    PDEBUG("Not enough satellites found, retrying... \n");
    delay(1000);
  }

  while (!antennaPos.beginAltimeter()) {
    PDEBUG("Could not connect to altimeter, retrying... \n");
    delay(1000);
  }
  
  while (!getAltimeterPosition()) {
    PDEBUG("Altimeter read failed, retrying... \n");
    delay(1000);
  }

  antennaDyn.manualSetup(); // until compass is installed

  // antennaDyn.setNorthBearing(antennaPos.northBearing()); // Once compass is installed
}

void loop() {
  // put your main code here, to run repeatedly:
  // More about the Azimuth/Elevation coordinate system https://en.wikipedia.org/wiki/Horizontal_coordinate_system
  dronePos.getPosition();
  float antennaToDroneDistance = calculateDistance(antennaPos.latitude(), antennaPos.longitude(), dronePos.latitude(), dronePos.longitude());
  float antennaToDroneAzimuth = calculateAzimuth(antennaPos.latitude(), antennaPos.longitude(), dronePos.latitude(), dronePos.longitude());
  float antennaToDroneElevation = calculateElevation(antennaToDroneDistance, antennaPos.altitude(), dronePos.altitude());
  antennaDyn.setAzimuth(antennaToDroneAzimuth);
  antennaDyn.setElevation(antennaToDroneElevation);
}
