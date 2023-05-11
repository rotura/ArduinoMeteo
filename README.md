# ArduinoMeteo
 This is a simple weather station with ASCOM driver for astronomical purpose.
 It's based on an Arduino UNO.
 
 ![ArduinoMeteo on NINA](https://github.com/rotura/ArduinoMeteo/blob/main/images/ArduinoMeteo_NINA.jpg)
 
 The actual arduinon code works with the following sensor:
 - I2C:
  - TSL2591: Luxometer for SQM reading
  - MLX90614: Infrared sky temperature for clouds 
 - SPI:
  - BMP280: Ground temperature and pressure
   - MISO: Pin 12
   - MOSI: Pin 11
   - SCK:  Pin 13
   - CS:   Pin 10 
 - Digital:
  - DHT22: Humidity (Pin 5)

I made a simple PCB, but has some bugs. The I2C_1 can't be used because it touch the Arduino UNO USB.
 ![ArduinoMeteo PCB](https://github.com/rotura/ArduinoMeteo/blob/main/images/PCB.jpg)
 
