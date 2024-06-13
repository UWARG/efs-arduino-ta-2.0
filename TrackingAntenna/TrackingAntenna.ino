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

// #define MACRO(milliseconds, condition, else) \
// ( \
//     (uint32_t lastMillis {0}), \
//     ()
// )

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // Serial1.begin(115200);

    while(!Serial);

    // antennaDyn.begin();

    while (!DronePosition::beginWiFi()) {
        PDEBUG("Could not connect to WiFi, retrying... \n");
        myDelay(1000);
    }

    // while (true) {
    //     if (myDelay(1000)) {
    //         if (DronePosition::beginWiFi()) {
    //             break;
    //         } else {
    //             PDEBUG("Could not connect to WiFi, retrying... \n");
    //         }
    //     }
    // }
    
    while (!DronePosition::connectWiFi("roni iPhone", "roniwifi")) {
        PDEBUG("Attempting to connect to WiFi, this may take a bit... \n");
        myDelay(10000);
    }

    // while (true) {
    //     if (myDelay(10000)) {
    //         if (DronePosition::connectWiFi("roni iPhone", "roniwifi")) {
    //             break;
    //         } else {
    //             PDEBUG("Attempting to connect to WiFi, this may take a bit... \n");
    //         }   
    //     }
    // }

    while (!antennaPos.beginGPS()) {
        PDEBUG("Could not connect to GPS, retrying... \n");
        myDelay(1000);
    }

    // while (true) {
    //     if (myDelay(1000)) {
    //         if (antennaPos.beginGPS()) {
    //             break;
    //         } else {
    //             PDEBUG("Could not connect to GPS, retrying... \n");
    //         }   
    //     }
    // }

    while (!antennaPos.getGPSPosition()) {
        PDEBUG("Not enough satellites found, retrying... \n"); 
        myDelay(1000);
    }

    // while (true) {
    //     if (myDelay(1000)) {
    //         if (antennaPos.getGPSPosition()) {
    //             break;
    //         } else {
    //             PDEBUG("Not enough satellites found, retrying... \n"); 
    //         }   
    //     }
    // }

    antennaDyn.manualSetup(); // until compass is installed

//   antennaDyn.setNorthBearing(antennaPos.northBearing()); // Once compass is installed

    xTaskCreate(DronePosition::getPosition, "Get Drone Position", 1000, ( void * ) 1, 1, &xDronePosHandle);
    xTaskCreate(runAntenna, "Run Antenna", 1000, ( void* ) 1, 1, &xRunAntennaHandle);

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
        PDEBUG("test123 \n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    // PDEBUG("TASKS FAILED");
}
