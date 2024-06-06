#include "Arduino.h"
#include "AntennaDynamics.hpp"
#include "../Config/Config.hpp"

AntennaDynamics::AntennaDynamics():
    initialAntennaAzimuth_ {0}
{
    
}

void AntennaDynamics::begin() {
    pitchServo_.attach(PITCH_PWM);
    yawServo_.attach(YAW_PWM);
}

void AntennaDynamics::manualSetup() { // don't use this when we have the compass again
    setYawAngle(0);
    delay(5000);
}

void AntennaDynamics::setNorthAngle(float angle) {
    initialAntennaAzimuth_ = angle;
}

bool AntennaDynamics::setAzimuth(float azimuth) {
    azimuth = fmod((azimuth + 360), 360);

    float angle = azimuth - initialAntennaAzimuth_;

    PDEBUG("Azimuth: ");
    PDEBUG(azimuth);
    PDEBUG("\n");

    PDEBUG("Initial Antenna Azimuth: ");
    PDEBUG(initialAntennaAzimuth_);
    PDEBUG("\n");

    return setYawAngle(angle);
}

// Function to set the yaw angle of the tracker in degrees
bool AntennaDynamics::setYawAngle(float angle) {
    angle = fmod((angle + 360), 360);

    if (angle > 180) {
        angle = angle - 360;
    } else if (angle < -180) {
        angle = angle + 360;
    }

    if (angle > YAW_START_ANGLE) {
        angle = YAW_START_ANGLE;
    } else if (angle < YAW_END_ANGLE) {
        angle = YAW_END_ANGLE;
    }

    int yawMicroseconds = map(angle, YAW_END_ANGLE, YAW_START_ANGLE, YAW_END_MICROSECONDS, YAW_START_MICROSECONDS);

    if (yawMicroseconds > YAW_END_MICROSECONDS || yawMicroseconds < YAW_START_MICROSECONDS) {
        return false;
    }

    yawServo_.writeMicroseconds(yawMicroseconds);

    PDEBUG("Yaw Angle: ");
    PDEBUG(angle);
    PDEBUG("\n");

    PDEBUG("Yaw Microseconds: ");
    PDEBUG(yawMicroseconds);
    PDEBUG("\n");

    return true;
}

bool AntennaDynamics::setElevation(float elevation) {
    PDEBUG("Elevation: ");
    PDEBUG(elevation);
    PDEBUG("\n");

    return setPitchAngle(elevation);
}

// Function to set the pitch angle of the tracker in degrees
bool AntennaDynamics::setPitchAngle(float angle) {
    if (angle > 90) {
        angle = 90;
    } else if (angle < 0) {
        angle = 0;
    }

    if (angle > 90 || angle < 0) {
        return false;
    }
    
    int pitchMicroseconds = map(angle, 0, 90, PITCH_START_MICROSECONDS, PITCH_END_MICROSECONDS);

    pitchServo_.writeMicroseconds(pitchMicroseconds);

    PDEBUG("Pitch Angle: ");
    PDEBUG(angle);
    PDEBUG("\n");

    PDEBUG("Pitch Microseconds: ");
    PDEBUG(pitchMicroseconds);
    PDEBUG("\n");

    return true;
}
