#include "DronePosition.hpp"
#include <MAVLink.h>
// #include <Arduino_FreeRTOS.h>
#include <WiFiNINA.h>

WiFiUDP DronePosition::UDP_;
uint8_t DronePosition::packetBuffer_[PACKET_BUFFER_SIZE] {}; 
int DronePosition::len {0};
int DronePosition::status_ = WL_IDLE_STATUS;
float DronePosition::latitude_ = 0;
float DronePosition::longitude_ = 0;
float DronePosition::altitude_ = 0;

bool DronePosition::beginWiFi() {
    WiFi.setPins(WIFI_SPI_CS, WIFI_SPI_ACK, WIFI_RESETN, WIFI_GPIO0, &WIFI_SPI);
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        PDEBUG("Communication with WiFi module failed! \n");
        return false;
    } else {
        PDEBUG("Communication with WiFi module succeeded! \n");
        return true;
    }
}

bool DronePosition::connectWiFi(char ssid[], char pass[]) {
    if (status_ != WL_CONNECTED) {
        // attempt to connect to Wifi network:
        PDEBUG("Attempting to connect to SSID: ");
        PDEBUG(ssid);
        PDEBUG("\n");
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status_ = WiFi.begin(ssid, pass);
        return false;
    } else {
        PDEBUG("Connected to WiFi \n");
        // print the SSID of the network you're attached to:
        PDEBUG("SSID: ");
        PDEBUG(WiFi.SSID());
        PDEBUG("\n");

        // print your board's IP address:
        IPAddress ip = WiFi.localIP();
        PDEBUG("IP Address: ");
        PDEBUG(ip);
        PDEBUG("\n");

        // print the received signal strength:
        long rssi = WiFi.RSSI();
        PDEBUG("Signal strength (RSSI): ");
        PDEBUG(rssi);
        PDEBUG(" dBm\n");

        PDEBUG("Starting UDP connection at port: ");
        PDEBUG(LOCAL_PORT);
        PDEBUG("\n");
        // if you get a connection, report back via serial:
        UDP_.begin(LOCAL_PORT);
        return true;
    }
}

bool DronePosition::parseUDP() {
    // if there's data available, read a packet
    int packetSize = UDP_.parsePacket();
    if (packetSize) {
        // PDEBUG("Received packet of size ");
        // PDEBUG(packetSize);
        // PDEBUG("\nFrom ");
        // PDEBUG(UDP_.remoteIP());
        // PDEBUG(", port ");
        // PDEBUG(UDP_.remotePort());
        // PDEBUG("\n");

        // read the packet into packetBufffer
        len = UDP_.read(packetBuffer_, PACKET_BUFFER_SIZE); // this is the issue?
        if (len > 0) {
            packetBuffer_[len] = 0;
        }
        // PDEBUG("Contents:\n");
        // PDEBUG(packetBuffer_);
        // PDEBUG("\n");

        return true;
    }
    return false;
}

void DronePosition::getPosition(void * pvParameters) {
    // while (true) {
        mavlink_message_t msg;
        mavlink_status_t status;
        // if (WiFi.status() != WL_CONNECTED) {
        //     // PDEBUG(WiFi.status());
        // }

        if (parseUDP()) {
            for (uint16_t i{0};  i < len; ++i) {
                uint8_t c = packetBuffer_[i];

                if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
                    if (msg.msgid == MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {
                        mavlink_global_position_int_t position;
                        mavlink_msg_global_position_int_decode(&msg, &position);

                        latitude_ = position.lat / 1e7;
                        PDEBUG(F("Drone Lat: "));
                        PDEBUG(latitude_);

                        longitude_ = position.lon / 1e7;
                        PDEBUG(F(" Drone Long: "));
                        PDEBUG(longitude_);
                        PDEBUG(F(" (degrees)"));

                        altitude_ = position.alt / 1e3;
                        PDEBUG(F(" Drone Alt: "));
                        PDEBUG(altitude_);
                        PDEBUG(F(" (m) \n"));

                    }
                }

                // vTaskDelay(2 / portTICK_PERIOD_MS); // assuming 9600 baud for mavlink
            }
        }
        // vTaskDelay(100 / portTICK_PERIOD_MS); // assuming 9600 baud for mavlink
    // }
}

float DronePosition::latitude() {
    return latitude_;
}

float DronePosition::longitude() {
    return longitude_;
}

float DronePosition::altitude() {
    return altitude_;
}