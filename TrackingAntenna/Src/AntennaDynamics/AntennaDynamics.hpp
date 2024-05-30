#include "Servo.h"

class AntennaDynamics {
   private:
    Servo pitchServo_;
    Servo yawServo_;
    int northMicroseconds_;

   public:
    AntennaDynamics();
    void manualSetup(); // until compass is installed
    void setNorthBearing(float angle);
    void setYawAngle(float angle);
    void setPitchAngle(float angle);
};
