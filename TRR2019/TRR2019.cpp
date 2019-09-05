#include "TRR2019.h"

#include "Metro.h"
#include "params.h"
#include "Servo.h"
#include "lidar/LidarXV11.h"
//#include "lidar/LidarManager.h"



Servo servo = Servo();
LidarXV11 lidar = LidarXV11();
bool isLidarSet;
Metro lidarAsservTime(500);
unsigned int lidarPWM;
uint32_t startTime;

void test(){
	uint32_t startTime = millis(), currentTime;
	//int i = 1;
	//int servoAngleCommand = 80;
	//servo.write(servoAngleCommand);
	/*while (servoAngleCommand > 30){ //rester dans [50;120]
		servo.write(servoAngleCommand);
		servoAngleCommand--;
		while ((millis() - startTime) < 50){}
		startTime = millis();
	}*/
	for (int cmd = 75; cmd<110; cmd += 5){
		if( ! (abs(cmd - 85)<10)){
			Serial.println(cmd);
			servo.write(cmd);
			analogWrite(MOT1_PWM, 50);
			startTime = millis();
			while ((millis() - startTime) < 2000){}
			analogWrite(MOT1_PWM, 0);
			while ((millis() - startTime) < 3000){}

		}
	}
	/*while (millis() - startTime < 1000){
		servo.write(servoAngleCommand);<
		Serial.print(millis());
		analogWrite(MOT1_PWM, 50);
		/*if (millis() - startTime > 500){
			startTime = millis();
			i *= 0;
			servoAngleCommand = 80 + i*10;
		}*/

	//test distance et rayon
	/*do {
		currentTime = millis();
	} while ((currentTime - startTime) < 3000);
	analogWrite(MOT1_PWM,0);
	*/

	//servo.write(servoAngleCommand+10);

	while(1){} ; // bloque
}

void command() { // Updates the servo command
	int NB_LIDAR_ANGLES = 5; // <= 8
	int TURN_DISTANCE = 1500;
	int distanceD;
	int distanceG;

	int servoAngleCommand = 90;
	int i = 2;
	while (i <= NB_LIDAR_ANGLES && servoAngleCommand == 90) {
		distanceG = lidar.get_distance(i * 10);
		if (!lidar.is_valid(i * 10)) {
			distanceG = 3000;
		}
		distanceD = lidar.get_distance(360 - i * 10);
		if (!lidar.is_valid(360 - i * 10)) {
			distanceD = 3000;
		}

		Serial.print(i);
		Serial.print("		");
		Serial.print(distanceG);
		Serial.print("		");
		Serial.print(lidar.is_valid(i * 10));
		Serial.print("		");
		Serial.print(distanceD);
		Serial.print("		");
		Serial.println(lidar.is_valid(360 - i * 10));


		if (distanceG < TURN_DISTANCE || distanceD < TURN_DISTANCE) {
			if (distanceG - distanceD > 100) {
				//servoAngleCommand = 120 - (i - 1) * 4;
				Serial.println("coucou G");
				servoAngleCommand = 60 + (i - 1) * 4;
			}

			else if (distanceG - distanceD < -100) {
				//servoAngleCommand = 60 + (i - 1) * 4;
				Serial.println("coucou D");
				servoAngleCommand = 120 - (i - 1) * 4;
			}
		}
		i++;
	}

	/*Serial.print("Commande :    ");
	Serial.println(servoAngleCommand);*/
	servoAngleCommand = min(max(60, servoAngleCommand), 120);


	//Serial.println(servoAngleCommand);

	servo.write(servoAngleCommand);

}


void asservLidar(){
	//Serial.print("Speed:");
	//Serial.println(lidar.getSpeed());
	if (!isLidarSet && lidarAsservTime.check()){
		if (lidar.getSpeed() >= 260){
			lidarPWM -= 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else if (lidar.getSpeed() <= 230){
			lidarPWM += 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else{
			//isLidarSet = true;
		}
	}
}


//The setup function is called once at startup of the sketch
void setup()
{
	isLidarSet = false;
	lidarPWM = 60;
	Serial.begin(115200);
	Serial2.begin(115200);
	servo.attach(SERVO1);
	pinMode(MOT1_DIR, OUTPUT);
	digitalWrite(MOT1_DIR, HIGH);
	servo.write(90);
	while(!Serial);
	startTime = millis();
}



// The loop function is called in an endless loop
void loop()
{
	if (millis() - startTime > 4000){
		analogWrite(MOT1_PWM, 50);
	}
	uint8_t b = 0;
	if(Serial2.available()){
		b = Serial2.read();
		lidar.update(b);
		//Serial.println("Available");
	}
	/*Serial.print("Distance :");
	Serial.print(lidar.get_distance(350));
	Serial.print("    ");
	Serial.println(lidar.is_valid(350));
	Serial.print(lidar.get_distance(10));
	Serial.print("    ");
	Serial.println(lidar.is_valid(10));
	Serial.print("    ");
*/
	//command();

	//asservLidar();
	test();

}
