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
  

  // antennaDyn.setup(); // until compass is installed

  // antennaDyn.setNorthBearing(antennaPos.northBearing()); // Once compass is installed
}

void loop() {
  // put your main code here, to run repeatedly:
  dronePos.getPosition();
  float antennaToDroneDistance = calculateDistance(antennaPos.latitude(), antennaPos.longitude(), dronePos.latitude(), dronePos.longitude());
  float antennaToDroneBearing = calculateBearing(antennaPos.latitude(), antennaPos.longitude(), dronePos.latitude(), dronePos.longitude());
  antennaDyn.setYawAngle(calculatePitchAngle(antennaToDroneDistance, dronePos.altitude()));
  antennaDyn.setPitchAngle(calculateYawAngle(antennaToDroneBearing));
}
