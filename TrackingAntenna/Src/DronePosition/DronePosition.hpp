#include <WiFiUdp.h>
#include "../Config/Config.hpp"

class DronePosition {
   private:
    static WiFiUDP UDP_;
    static uint8_t packetBuffer_[PACKET_BUFFER_SIZE]; //buffer to hold incoming packet
    static int len;
    static int status_;
    static float latitude_;
    static float longitude_;
    static float altitude_;

    static bool parseUDP();

   public:
    static bool beginWiFi();
    static bool connectWiFi(char ssid[], char pass[]);
    static void getPosition(void * pvParameters);
    static float latitude();
    static float longitude();
    static float altitude();
};