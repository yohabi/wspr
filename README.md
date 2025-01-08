This is software WSPR implementation for DIY QRP/QRPp transiever: ESP32 (WROOM 32) + SI5351 (I2C) + GPS (serial TTL) + OLED (I2C).

Project for tracking radio-amateur during trip.

## TODO:

- WSPR message ~~type 1~~ and **type 2**

- ~~GPStoQTH6()~~ **(done)**

- make schematic and check WSPR code in HW

- GPS module: getGPSData(), getGPSDateTime()

- SI5351 module: setFrequency()

- SSD13xx OLED module

- WIFI interface for configuration: callsign, power, switch types 1/2, intervals (every N interval, once/many at GPS warmed and accuracy OK)


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

git fetch; git pull
