#include "Util.hpp"
#include <cmath>
#include "Arduino.h"

inline double toRadians(double degrees) {
  return degrees * (M_PI / 180);
}

inline double toDegrees(double radians) {
  return radians * (180 / M_PI);
}

//https://stackoverflow.com/questions/27126714/c-latitude-and-longitude-distance-calculator
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  lat1  = toRadians(lat1);
  lon1 = toRadians(lon1);
  lat2  = toRadians(lat2);
  lon2 = toRadians(lon2);
  double haversine = (pow(sin((1.0 / 2) * (lat2 - lat1)), 2)) + ((cos(lat1)) * (cos(lat2)) * (pow(sin((1.0 / 2) * (lon2 - lon1)), 2)));
  double temp = 2 * asin(min(1.0, sqrt(haversine)));
  float point_dist = EARTH_RADIUS * temp;
  return point_dist;
}

//https://stackoverflow.com/questions/21060891/android-how-can-i-get-the-bearing-degree-between-two-locations
double calculateBearing(double baseLat, double baseLon, double droneLat, double droneLon) {
  double startLat = toRadians(baseLat);
  double startLon = toRadians(baseLon);
  double endLat = toRadians(droneLat);
  double endLon = toRadians(droneLon);

  double dLon = endLon - startLon;

  double x = sin(dLon) * cos(endLat);
  double y = cos(startLat) * sin(endLat) - sin(startLat) * cos(endLat) * cos(dLon);
  double initialBearing = atan2(x, y);

  // Convert bearing from radians to degrees
  double bearing = toDegrees(initialBearing);

  // Normalize the bearing
  bearing = fmod((bearing + 360), 360);

  return bearing;
}

float calculatePitchAngle(float distance, float altitude) {
  float out;
  out = toDegrees(atan(altitude/distance));
  if (out > 90) {
    out = 90;
  }
  if (out < 0) {
    out = 0;
  }
  return out;
}

int calculateYawAngle(int bearing) {
  // float upperLimit;
  // float lowerLimit;
  // float out;
  // out = map(bearing, 0, upperLimit, -85, 85);
  //float og_bearing = bearing;
  //bool rollOverRight = false;
  /*
  if (AAT_BEARING > 180) {
    if (bearing < AAT_BEARING - 85) {
      bearing = bearing + 360;
    }
  } else {
    if (bearing > AAT_BEARING + 85) {
      bearing = bearing - 360;
    }
  }
  upperLimit = AAT_BEARING + 85;
  lowerLimit = AAT_BEARING - 85;
  out = map(bearing, lowerLimit, upperLimit, -85, 85);
  Serial.println(out);
  if (og_bearing) {
    out = -85;
  }
  if (og_bearing > upperLimit) {
    out = 85;
  }
  */
  bearing = (bearing + 360) % 360;

  // int rawDifference = bearing - AAT_BEARING; // this is how he was doing it, figure out what he's really doing here and whats necessary
  int rawDifference = bearing;

  if (rawDifference > 180) {
      rawDifference = rawDifference - 360;
  } else if (rawDifference < -180) {
      rawDifference = rawDifference + 360;
  }
  if (rawDifference > 85) {
    rawDifference = 85;
  }

  if (rawDifference < -85) {
    rawDifference = -85;
  }

  //Serial.println(rawDifference);
  return rawDifference;
}

