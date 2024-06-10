#include <WiFiUdp.h>

class DronePosition {
   private:
    static WiFiUDP UDP_;
    static char packetBuffer_[255]; //buffer to hold incoming packet
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