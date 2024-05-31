#include "DronePosition.hpp"
#include "../Config/Config.hpp"
#include <MAVLink.h>

bool DronePosition::getPosition() {
    bool retVal = false;
    mavlink_message_t msg;
    mavlink_status_t status;

    while (MavlinkSerial.available() > 0) {
        char c = MavlinkSerial.read();

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
                PDEBUG(F(" (m)"));

                retVal = true;  
            }
        }
    }

    return retVal;
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