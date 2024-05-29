#include "Src/AntennaPosition/AntennaPosition.hpp"
#include "Src/DronePosition/DronePosition.hpp"
#include "Src/AntennaDynamics/AntennaDynamics.hpp"
#include "Src/Util/Util.hpp"

AntennaPosition antennaPos{};
DronePosition dronePos{};
AntennaDynamics antennaDyn{};

void setup() {
  // put your setup code here, to run once:
  antennaPos.beginGPS();
  antennaPos.getPosition();  

  antennaDyn.setup(); // until compass is installed

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
