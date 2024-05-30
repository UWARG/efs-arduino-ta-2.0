#include "Arduino.h"
#include "AntennaDynamics.hpp"
#include "../Config/Config.hpp"

AntennaDynamics::AntennaDynamics():
 northMicroseconds_ {YAW_MID_POS}
{
    pitchServo_.attach(PITCH_PWM);
    yawServo_.attach(YAW_PWM);
}

void AntennaDynamics::manualSetup() { // don't use this when we have the compass again
    yawServo_.write(YAW_MID_POS);
    delay(5000);
}

void AntennaDynamics::setNorthBearing(float angle) {
    northMicroseconds_ = map(angle, -175, 175, YAW_END_POS, YAW_START_POS);
}

// Function to set the pitch angle of the tracker in degrees
void AntennaDynamics::setYawAngle(float angle) {
    int yawMicroseconds;
    int y_offset_microseconds;
    
    yawMicroseconds = map(angle, -175, 175, YAW_END_POS, YAW_START_POS);
    y_offset_microseconds = YAW_MID_POS - northMicroseconds_;
    if ((yawMicroseconds - y_offset_microseconds) > YAW_END_POS || (yawMicroseconds - y_offset_microseconds) < YAW_START_POS) {
        return;
    }
    yawServo_.writeMicroseconds(yawMicroseconds - y_offset_microseconds);
    
    PDEBUG("Yaw Microseconds (no offset): ");
    PDEBUG(yawMicroseconds);
    PDEBUG("\n");
    
    PDEBUG("Yaw Microseconds (with offset): ");
    PDEBUG(yawMicroseconds - y_offset_microseconds);
    PDEBUG("\n");
}

// Function to set the yaw angle of the tracker in degrees
void AntennaDynamics::setPitchAngle(float angle) {
    if (angle > 90 || angle < 0) {
        return;
    }
    int p_microseconds;
    p_microseconds = map(angle, 0, 90, 575, 2300);
    pitchServo_.writeMicroseconds(p_microseconds);
}
