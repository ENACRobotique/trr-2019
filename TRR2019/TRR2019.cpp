#include "TRR2019.h"

#include "Metro.h"
#include "params.h"
#include "Servo.h"
#include "lidar/LidarXV11.h"
//#include "lidar/LidarManager.h"



Servo servo = Servo();
LidarXV11 lidar = LidarXV11();


void command() { // Updates the servo command
	int NB_LIDAR_ANGLES = 5;
	int TURN_DISTANCE = 150;

	int servoAngleCommand = 90;
	int i = 1;
	while (i <= NB_LIDAR_ANGLES && servoAngleCommand == 90) {
		int distanceD = lidar.get_distance(i * 10);
		int distanceG = lidar.get_distance(360 - i * 10);
		if (distanceD < TURN_DISTANCE || distanceG < TURN_DISTANCE) {
			if (distanceD - distanceG > 10) {
				servoAngleCommand = 180 - (i - 1) * 10;
			}

			else if (distanceD - distanceG < -10) {
				servoAngleCommand = (i - 1) * 10;
			}
		}
		i++;
	}
   servo.write(servoAngleCommand);
}



//The setup function is called once at startup of the sketch
void setup()
{
	servo.attach(SERVO2);
	analogWrite(MOT1_PWM, 40);
	servo.write(90);

}



// The loop function is called in an endless loop
void loop()
{
	command();

}
