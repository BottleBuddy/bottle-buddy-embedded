#include "devices/IMU.h"

int imu_sensor_setup() {
    if(!IMU.begin()) {
        return -1;
    }
    return 0;
}

void read_accelerometer(float& x, float& y, float& z) {
    float x_dim, y_dim, z_dim;
    while(!IMU.accelerationAvailable())
        ;
    IMU.readAcceleration(x_dim, y_dim, z_dim);
    x = x_dim;
    y = y_dim;
    z = z_dim;
}

/* THE FUNCTION BELOW DOES NOT WORK! But it's on the right track :)
 * ORIENTATION CHECKER
 * Description: This returns an integer value corresponding to 
 * the orientation of the cap. 
 *    1 = right side up
 *    2 = up side down
 *    3 = tilted left
 *    4 = left
 *    5 = tilted right 
 *    6 = right
 * Input: None
 * Output: Integer value oreintation
*/
int orientation(){
    float IMU_DELTA = .1;
  float x, y, z;
  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(x, y, z);
 
    if(y >= -IMU_DELTA && y <= 0)
          //Serial.println("flat");
          return 1;
    if(y <= IMU_DELTA && y > 0)
          //Serial.println("upside down"); 
          return 2;   
    if(y > IMU_DELTA && y < 1 - IMU_DELTA)
          //Serial.println("tilted to the left");
          return 3;
    if(y >= 1 - IMU_DELTA)
          //Serial.println("left");
          return 4;
    if(y < -IMU_DELTA && y > IMU_DELTA - 1)
          //Serial.println("tilted to the right");
          return 5;
    if(y <= IMU_DELTA - 1)
          //Serial.println("right");
          return 6;
  }
}

//This shows you how to read the gyroscope
int gyroscope_example(){
      float x, y, z;

  if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.println(z);
  }
}