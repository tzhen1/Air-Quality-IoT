# Air-Quality-IoT

IoT Air quality monitoring system that will measure toxic gas concertation such as carbon dioxide, monoxide, nitrous oxides, ammonia, sulphide, and benzene-based vapor using the MQ135 module. It will measure dust particles (> 1 micron) using DSM501A module. Temperature and humidity will be measured using DH22 module. The temperature and humidity sensor will acquire data and transmit analogue signal to the ESP8266 using the ADC (Analog to Digital Converter). A 5V DC fan will clear the air, whilst sucking in particles.

The OLED will be programmed on the ESP8266 microcontroller through serial communication (I2C). Data generated from these sensors will be sent and stored securely to an Amazon Web Services Cloud (AWS) using a Wi-Fi connectivity module implemented inside the device.

![image](https://github.com/tzhen1/Air-Quality-IoT/assets/79606359/b8ae74a9-6be5-4461-966a-4c221b180f2a)
