#include "Src/AntennaPosition/AntennaPosition.hpp"
#include "Src/DronePosition/DronePosition.hpp"
#include "Src/AntennaDynamics/AntennaDynamics.hpp"
#include "Src/Util/Util.hpp"
#include "Src/Config/Config.hpp"
#include <Arduino_FreeRTOS.h>

AntennaPosition antennaPos{};
AntennaDynamics antennaDyn{};
TaskHandle_t xDronePosHandle = NULL;
TaskHandle_t xRunAntennaHandle = NULL;
TaskHandle_t xSetupAntennaHandle = NULL;

bool myDelay(int delayMilliseconds) { // To be only used from setup() to replace delay(). Can not be used when multiple threads are running with FreeRTOS. Call this function once before using the if() to initialize lastMillis
    static uint32_t lastMillis; // right now this will work as long as the arduino isnt stuck on setup() for a couple days (integer overflow for lastMillis)
    if (millis() - lastMillis > delayMilliseconds) {
        lastMillis = millis();
        return true;
    }
    return false;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // Serial1.begin(115200);

    while(!Serial);

    // antennaDyn.begin();

    while (true) {
        if (myDelay(1000)) {
            if (DronePosition::beginWiFi()) {
                break;
            } else {
                PDEBUG("Could not connect to WiFi, retrying... \n");
            }
        }
    }

    while (true) {
        if (myDelay(10000)) {
            if (DronePosition::connectWiFi("roni iPhone", "roniwifi")) {
                break;
            } else {
                PDEBUG("Attempting to connect to WiFi, this may take a bit... \n");
            }   
        }
    }

    // while (!antennaPos.beginGPS()) {
    //     PDEBUG("Could not connect to GPS, retrying... \n");
    //     delay(1000);
    // }

    // while (!antennaPos.getGPSPosition()) {
    //     PDEBUG("Not enough satellites found, retrying... \n");   
    //     delay(1000);
    // }

    // antennaDyn.manualSetup(); // until compass is installed

//   antennaDyn.setNorthBearing(antennaPos.northBearing()); // Once compass is installed

    xTaskCreate(DronePosition::getPosition, "Get Drone Position", 1000, ( void * ) 1, 1, &xDronePosHandle);
    // xTaskCreate(runAntenna, "Run Antenna", 1000, ( void* ) 1, 1, &xRunAntennaHandle);

    vTaskStartScheduler();
}

void runAntenna(void * pvParameters) {

    while (true) {
        // More about the Azimuth/Elevation coordinate system https://en.wikipedia.org/wiki/Horizontal_coordinate_system
        float antennaToDroneDistance = calculateDistance(antennaPos.latitude(), antennaPos.longitude(), DronePosition::latitude(), DronePosition::longitude());
        float antennaToDroneAzimuth = calculateAzimuth(antennaPos.latitude(), antennaPos.longitude(), DronePosition::latitude(), DronePosition::longitude());
        float antennaToDroneElevation = calculateElevation(antennaToDroneDistance, antennaPos.altitude(), DronePosition::altitude());
        antennaDyn.setAzimuth(antennaToDroneAzimuth);
        antennaDyn.setElevation(antennaToDroneElevation);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    
}
