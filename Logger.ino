#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>

const int baudRate = 9600;
const int chipSelect = 10;

int16_t ax, ay, az;
int16_t gx, gy, gz;

RTC_DS1307 rtc;
MPU6050 accelgyro;
Adafruit_BMP085 bmp;

String data = "";
void setup() {
  Serial.begin(baudRate);
  Wire.begin();

  // Initializing BMP085
  if (!bmp.begin()){
   Serial.println("Could not initialize BMP085");
  }

  // Initializing MPU6050
  accelgyro.initialize();
  accelgyro.setI2CBypassEnabled(true);

  // Initializing RTC
  rtc.adjust(DateTime(F(__DATE__),1,0,0));
  
  if (!rtc.begin()){
    Serial.println("RTC ERROR");
    // add some sort of led for status
  }

  // Initializing SD Card
  if (!SD.begin(chipSelect)){
    Serial.println("SD Card Error");
    // add some sort of led for status
  }
  File dataLogger = SD.open("Log.csv", FILE_WRITE);
  data += "Time,Altitude,Corrected Altitude,Accel X,Accel Y,Accel Z,Gryo X,Gyro Y,Gyro Z";
  dataLogger.println(data);
  dataLogger.close();
}

void loop() {
  data = "";
  File dataLogger = SD.open("Log.csv", FILE_WRITE);
  DateTime RTC_Time = rtc.now();

  // RTC Time
  data += (String)RTC_Time.timestamp(DateTime::TIMESTAMP_TIME) + ",";

  // bmp data 
  data += (String)bmp.readAltitude() + ",";
  data += (String)bmp.readAltitude(101325) + ",";


  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
    data += (String)ax + ",";
    data += (String)ay + ",";
    data += (String)az + ",";
    data += (String)gx + ",";
    data += (String)gy + ",";
    data += (String)gz;
    
  if (dataLogger){
    Serial.println(data);
    dataLogger.println(data);
    dataLogger.close();
  }
}
