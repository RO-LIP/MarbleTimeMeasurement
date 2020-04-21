# MarbleTimeMeasurement
A simple arduino project to measure time in marble races. 
Compiled with Platform.io and used with the [quercetti marlbe run](https://www.quercettistore.com/en/toys/marble-run/6315-big-marbledrome)

![img](Images/IMG_20200421_140640.jpg width="200" )


## Components
* Arduino Nano
* 4×20 Display with I²C Modul
* Rotary Encoder KY-040
* Laser transmit
* Photoresistor

We bought the last three components from [here](https://www.banggood.com/Geekcreit-37-In-1-Sensor-Module-Board-Set-Starter-Kits-Geekcreit-products-that-work-with-official-Arduino-boards-p-1137051.html?cur_warehouse=UK )

## Connection

### Power supply

Connect the Rotary Encoder with 3.3V. \
All other parts (Display, Laser, Photoresitor) need 5V. \
The Ground pins should be connect togehter. 

### Photoresistor

Connet the signal pin with A0 on the Arduino Nano.

### Rotary Encoder

PIN_CLK -> D5   
DT_PIN  -> D4  \
SW_PIN  -> D3

### Display 

SDA -> A4
SCL  -> A5

## Software installation

Install Platform.io on an IDE of your choice and open the folder. It will automatical install all nessessery compiler, libraries ect.

## 3D Print

You can find the file in the 3DPrint folder. It is designed to be used with [quercetti marlbe run](https://www.quercettistore.com/en/toys/marble-run/6315-big-marbledrome)
