This is software WSPR implementation for DIY QRP/QRPp transiever: ESP32 (WROOM 32) + SI5351 (I2C) + GPS (serial TTL) + OLED (I2C).

Project for tracking radio-amateur during trip.

Wanted to xmit but Xiegu G90 too heavy and may be damaged by vibration.

Pre-configured mode:

1. with GPS

- Precise positioning

- DS3231 is configured with GPS-data and is used to start xmit more accurate

- Bonus: current coordinates available on screen

- If no GPS-satellites are available, xmit may be started with QTH set manually (optional: with buttons or web-page over wifi)

2. without GPS (?)

- May work as preconfigured beacon

- Lower power consumption without additional GPS module (but needs clock reference, e.g. DS3231) for longer standalone operation

- Instant xmit with DS3231, no need to wait for GPS warm-up to be completed

- Price lower for 3$..6$

## TODO:

- WSPR message ~~type 1~~ and **type 2**

- ~~GPStoQTH6()~~ **(done)**

- make schematic and check WSPR code in HW

- GPS module: getGPSData(), getGPSDateTime()

- SI5351 module: setFrequency()

- ~~SSD13xx OLED module~~ **(done)**

- ~~DS3231 RTC module~~ **(done)**

- WIFI interface for configuration: set callsign, power, switch WSPR message types 1/2, set intervals, set DS date/time

## Accuracy calculation

WSPR accuracy with message type 1 (AA00):

- at equator LON 2deg = **222,63km** x LAT 1deg = **111,13km**

- at 60deg LAT (SPb) **111,32km** x **111,13km**

WSPR accuracy with message type 2 (AA00AA):

- at equator LON 2deg/24 = **9,3km** x LAT 1deg/24 = **4,6km**

- at 60deg **4,6** x **4,6km**

## Xtra debug information:

UB1CBV -> 214762719 0ccd04df   0000[1100.11001101.00000100.11011111]

KO59   -> 13469     0000349d   00000000.00000000.0[0110100.10011101]

30dBm  -> 30+64 = 94 0[1011110]

QTHPower -> 1724126 001a4ede   00000000.00[011010.01001110.1][1011110]

summary [1100.11001101.00000100.11011111][011010.01001110.1][1011110]

11001100110100000100110111110110100100111011011110

## Schematic

### ESP32

ESP-WROOM-32 board - "ESP32 Dev Module" in Arduino IDE.

EN connected to GND via 10u capacitor to allow flashing without using buttons.

### OLED 0.91" I2C 128x32

VCC -> 3.3V output of AMS1117 on ESP32

SDA -> GPIO13

SCL -> GPIO14

```Wire.begin(13, 14);```

### ZS-042 DS3231 RTC


## Wifi web-page

Checkbox for "Allow WSPR msg type1 (1 minute, lower accuracy)"

Drop-down list for "Intervals"

Everytime (every 2 minutes) (no x option)

Every 10 minutes

Every 20 minutes

Every hour

"Sequence"

x1. Transmit once

x2. Transmit 2 times

x3. Transmit 3 times

Input for "Callsign"

Input for "Power (dBm, Watts)"

Save to EEPROM

Print current Date/Time

Input for Date/Time

Save to DS3231
