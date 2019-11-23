/* 

	DHT22 temperature sensor driver

*/

#ifndef DHT22_H_  
#define DHT22_H_

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2
#define SENSOR_READ_ERROR 100

// == function prototypes =======================================

void 	setDHTgpio(int gpio);
void 	errorHandler(int response);
int 	readDHT();
float 	getHumidity();
float 	getTemperature();
int 	getSignalLevel( int usTimeOut, bool state );
float   ReadTemperature(int);

#endif
