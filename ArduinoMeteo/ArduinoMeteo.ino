#include <DFRobot_MLX90614.h>

#include <SQM_TSL2591.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define DHTTYPE DHT22

const int DHTPin = 5;

Adafruit_BMP280 bmp;
SQM_TSL2591 tsl = SQM_TSL2591(2591);
DHT dht(DHTPin, DHTTYPE);
DFRobot_MLX90614_I2C mlx;


float hum, lux, temp;
double gainscale = TSL2591_GAIN_MAX;
uint32_t luminosity;
uint16_t ir, full;
double mag_arcsec2;

void setup() {
  Serial.begin(57600, SERIAL_8N1);
  hum = 1000;
  temp = 1000;
  while (!Serial);
  Serial.println("ArduinoMeteo Init");
  
  if (tsl.begin()) 
  {
    Serial.println("TSL config");
    /* Configure the sensor */
    configureSensorTSL();  
  } else 
  {
    Serial.println(F("No TSL2591 found ... check your wiring?"));
    //while (1);
  }
  
  delay(500);

  if (bmp.begin()) 
  {
    Serial.println("BMP Config");
    /* Configure the sensor */
    configureSensorBMP();  
  } else 
  {
    Serial.println(F("No BMP280 found ... check your wiring?"));
    //while (1);
  }

  if (NO_ERR == mlx.begin()) 
  {
    Serial.println("MLX Config");
    /* Configure the sensor */
    mlx.enterSleepMode();
    delay(50);
    mlx.enterSleepMode(false);
    delay(200);  
  } else 
  {
    Serial.println(F("No MLX90614 found ... check your wiring?"));
    //while (1);
  }
  
  delay(500);
  
  dht.begin();
  
  Serial.println("ArduinoMeteo Ready");

}

void loop() {
  String cmd;
  if(Serial.available() > 0) {
    cmd = Serial.readString();
    cmd.trim();
    if(cmd=="h"){ // Help
      help();      
    } else if(cmd=="0"){ // Temperature
      float readedTemp = bmp.readTemperature();
      if(!isnan(readedTemp)){
        temp = readedTemp;  
      }
      Serial.print(readedTemp);
      Serial.println("#");      
    } else if(cmd=="1"){ // Humidity
      float readedHum = dht.readHumidity();
      if(!isnan(readedHum)){
        hum = readedHum;  
      }
      Serial.print(readedHum);
      Serial.println("#");      
    } else if(cmd=="2"){ // Pressure
      Serial.print(bmp.readPressure());
      Serial.println("#");      
    } else if(cmd=="3"){ // Lux
      uint32_t lum = tsl.getFullLuminosity();
      uint16_t ir, full;
      ir = lum >> 16;
      full = lum & 0xFFFF;
      Serial.print(tsl.calculateLux(full, ir), 6);  
      Serial.println("#");      
    } else if(cmd=="4"){ // SkyTemperature
      Serial.print(mlx.getObjectTempCelsius());  
      Serial.println("#");      
    } else if(cmd=="5"){ // mag_arcsec2
      readSQM();
      Serial.print(mag_arcsec2);  
      Serial.println("#");      
    }
  }
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensorTSL(void)
{
  tsl.setGain(TSL2591_GAIN_HIGH);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  
  tsl.configSensor();
  tsl.setCalibrationOffset(0.0);
}

void configureSensorBMP(void)
{
/* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void help(void){
  Serial.println("0 - Temperature: Celsius");      
  Serial.println("1 - Humidity: %");      
  Serial.println("2 - Pressure: Pascal");      
  Serial.println("3 - Light intensity: Lux");      
  Serial.println("4 - Sky temperature: Celsius");      
  Serial.println("5 - SQM: mag/arcsec2");      
}

void readSQM(void) {
  if(temp != 1000){
    tsl.setTemperature(temp);  
  }
  tsl.takeReading();
  mag_arcsec2 = tsl.mpsas;
}
