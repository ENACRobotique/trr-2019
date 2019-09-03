#include "TRR2019.h"

#include "Metro.h"
#include "params.h"
#include "lidar/LidarXV11.h"
//#include "lidar/LidarManager.h"



LidarXV11 lidar = LidarXV11();
bool isLidarSet;
Metro lidarAsservTime(500);
unsigned int lidarPWM;
unsigned int startTime;


void dataGather(){
	int distances[181];
	for(int angle = 270; angle < 360; angle++){
		distances[angle - 270] = lidar.get_distance(angle);
		Serial.print(angle);
		Serial.print("   ");
		Serial.println(distances[angle - 270]);
	}

	for(int angle = 0; angle <= 90; angle++){
		distances[angle + 90] = lidar.get_distance(angle);
		Serial.print(angle);
		Serial.print("   ");
		Serial.println(distances[angle + 90]);
	}

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
	pinMode(MOT1_DIR, OUTPUT);
	while(!Serial);
	startTime = millis();
}



// The loop function is called in an endless loop
void loop()
{

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
	dataGather();
	asservLidar();

}
