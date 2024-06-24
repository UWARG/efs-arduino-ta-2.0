#include <WiFiUdp.h>
#include "../Config/Config.hpp"

class DronePosition {
   private:
    static WiFiUDP UDP_;
    static uint8_t packetBuffer_[PACKET_BUFFER_SIZE]; //buffer to hold incoming packet
    static uint8_t status_;
    static float latitude_;
    static float longitude_;
    static float altitude_;

    static uint16_t parseUDP();

   public:
    static bool beginWiFi();
    static bool connectWiFi(char ssid[], char pass[]);
    static bool getPosition();
    static float latitude();
    static float longitude();
    static float altitude();
};