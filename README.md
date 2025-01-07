This is software WSPR implementation for DIY QRP/QRPp transiever: ESP32/STM32 + SI5351 + GPS + DS3231 + OLED.

Project for tracking radio-amateur during trip.

TODO:

- check WSPR code in HW

- ~~WSPR message type 1~~ and **type 2**

- ~~GPStoQTH6()~~

- make schematic

- GPS module: getGPSCoordinates()

- GPS module: getGPSDateTime()

- DS3231 module: putDSDateTime()

- DS3231 module: getDSDateTime()

- SI5351 module: setFrequency()

- SSD13xx OLED module

UB1CBV -> 214762719 0ccd04df   0000[1100.11001101.00000100.11011111]

KO59   -> 13469     0000349d   00000000.00000000.0[0110100.10011101]

30dBm  -> 30+64 = 94 0[1011110]

QTHPower -> 1724126 001a4ede   00000000.00[011010.01001110.1][1011110]

summary [1100.11001101.00000100.11011111][011010.01001110.1][1011110]

11001100110100000100110111110110100100111011011110

git fetch; git pull
