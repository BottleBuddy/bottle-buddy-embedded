#include <Arduino_LSM9DS1.h>

/* THE FUNCTION BELOW DOES NOT WORK!
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
const float IMU_DELTA = .1;

float lastX, lastY, lastZ = 0; //used for accelerometer
int orientation()
{
    float x, y, z;
    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(x, y, z);

        if (y >= -IMU_DELTA && y <= 0)
            //Serial.println("flat");
            return 1;
        if (y <= IMU_DELTA && y > 0)
            //Serial.println("upside down");
            return 2;
        if (y > IMU_DELTA && y < 1 - IMU_DELTA)
            //Serial.println("tilted to the left");
            return 3;
        if (y >= 1 - IMU_DELTA)
            //Serial.println("left");
            return 4;
        if (y < -IMU_DELTA && y > IMU_DELTA - 1)
            //Serial.println("tilted to the right");
            return 5;
        if (y <= IMU_DELTA - 1)
            //Serial.println("right");
            return 6;
    }
}

/*
 * CAP STEADY CHECKER
 * Description: 
*/
int cap_steady()
{
    float currentX, currentY, currentZ;
    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(currentX, currentY, currentZ);
        if ((abs(currentX - lastX) > IMU_DELTA) || (abs(currentY - lastY) > IMU_DELTA) || (abs(currentZ - lastZ) > IMU_DELTA))
        {
            lastX = currentX;
            lastY = currentY;
            lastZ = currentZ;
            return 0;
        }
        else
        {
            lastX = currentX;
            lastY = currentY;
            lastZ = currentZ;
            return 1;
        }
    }
}