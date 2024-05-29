#define EARTH_RADIUS 6372797.56085

inline double toRadians(double degrees);
inline double toDegrees(double radians);

float calculateDistance(float lat1, float lon1, float lat2, float lon2);
double calculateBearing(double baseLat, double baseLon, double droneLat, double droneLon);

float calculatePitchAngle(float distance, float altitude);
int calculateYawAngle(int bearing);

void setPitchAngle(float angle);
void setYawAngle(float angle);