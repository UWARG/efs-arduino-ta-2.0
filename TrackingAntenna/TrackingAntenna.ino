#include "Src/AntennaPosition/AntennaPosition.hpp"
#include "Src/DronePosition/DronePosition.hpp"
#include "Src/AntennaDynamics/AntennaDynamics.hpp"
#include "Src/Util/Util.hpp"
#include "Src/Config/Config.hpp"

AntennaPosition antennaPos{};
AntennaDynamics antennaDyn{};

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // Serial1.begin(115200);

    while(!Serial);

    antennaDyn.begin();

    while (!DronePosition::beginWiFi()) {
        PDEBUG("Could not connect to WiFi, retrying... \n");
        delay(1000);
    }
    
    while (!DronePosition::connectWiFi("roni iPhone", "roniwifi")) {
        PDEBUG("Attempting to connect to WiFi, this may take a bit... \n");
        delay(10000);
    }

    while (!antennaPos.beginGPS()) {
        PDEBUG("Could not connect to GPS, retrying... \n");
        delay(1000);
    }

    while (!antennaPos.getGPSPosition()) {
        PDEBUG("Not enough satellites found, retrying... \n"); 
        delay(1000);
    }

    antennaDyn.manualSetup(); // until compass is installed

//   antennaDyn.setNorthBearing(antennaPos.northBearing()); // Once compass is installed

}

void runAntenna() {
    // More about the Azimuth/Elevation coordinate system https://en.wikipedia.org/wiki/Horizontal_coordinate_system
    float antennaToDroneDistance = calculateDistance(antennaPos.latitude(), antennaPos.longitude(), DronePosition::latitude(), DronePosition::longitude());
    float antennaToDroneAzimuth = calculateAzimuth(antennaPos.latitude(), antennaPos.longitude(), DronePosition::latitude(), DronePosition::longitude());
    float antennaToDroneElevation = calculateElevation(antennaToDroneDistance, antennaPos.altitude(), DronePosition::altitude());
    antennaDyn.setAzimuth(antennaToDroneAzimuth);
    antennaDyn.setElevation(antennaToDroneElevation);
}

static unsigned long lastMillisGetPosition {0};
static unsigned long lastMillisRunAntenna {0};
void loop() {
    // put your main code here, to run repeatedly:
    if (millis() - lastMillisGetPosition > 100) {
        lastMillisGetPosition = millis();
        DronePosition::getPosition();
        
    }

    if (millis() - lastMillisRunAntenna > 1000) {
        lastMillisRunAntenna = millis();        
        runAntenna();
    }
}
