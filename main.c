//gcc pro.c -Wno-pointer-to-int-cast
// https://github.com/robertostling/wspr-tools/

/*
type 1:
callsignCode (aka N) takes 28bit: [0 .. 2^28] == [0 .. 262177560]
QTHCode     (aka M1) takes 15bit: [0 .. 2^15] == [0 .. 32768]
powerQTHCode (aka M) takes 22bit: [0 .. 2^22] == [0 .. 4194304]

*/

// Saint-Petersburg, Aurora ship at 59.955316N, 30.337978E KO59ew09
#define _LAT 59.955316
#define _LON 30.337978
#define _CALLSIGN "UB1CBV"
#define _POWERDBM 30

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t message[162];

int strfind(char str[], char chr) {
	if (strchr(str, chr) != NULL) return (int)strchr(str, chr) - (int)str;
	else return -1;
}

void printBinary(uint32_t number, uint8_t digits) {
	uint8_t i;
	for (i = 0; i < digits; i++) if ((number>>(digits-1-i))&1 == 1) printf("1"); else printf("0");
}

void printBinaryReverse(uint32_t number, uint8_t digits) {
	uint8_t i;
	for (i = 0; i < digits; i++) if ((number>>i)&1 == 1) printf("1"); else printf("0");
}

uint8_t parity32Check(uint32_t number) {
	uint32_t p;
	p = (number&0xffff)^((number>>16)&0xffff);
    p = (p&0xff)^((p>>8)&0xff);
    p = (p&0xf)^((p>>4)&0xf);
    p = (p&0x3)^((p>>2)&0x3);
    p = (p&0x1)^((p>>1)&0x1);
    return (uint8_t)p;
}

uint8_t byteReverse(uint8_t in) {
	uint8_t i;
	uint8_t out = 0;
	for (i = 0; i < 8; i++) out |= ((in>>i)&1)<<(7-i);
	return out;
}

void encodeMessage(uint8_t* messageOutput, char* callsign, char* QTH, uint8_t powerdBm) {
	char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	char alphabetQTH[] = "ABCDEFGHIJKLMNOPQR";
	uint32_t reg162mask[6] = {0x7A47103, 0x58B340A4, 0x56349558, 0xE2CDC904, 0x63580CA0, 0x00};
	uint32_t callsignCode;
	uint32_t powerQTHCode;
	uint32_t reg0 = 0;
	uint32_t reg162[6] = {0, 0, 0, 0, 0, 0};
	uint32_t reg162i[6] = {0, 0, 0, 0, 0, 0};
	uint8_t i;
	uint8_t sourceIndex;
	uint8_t targetIndex;
	uint8_t i162 = 0;
	uint8_t i162byte = 0;
	uint8_t i162bit = 0;

	// 1) encode callsign, QTH and power (dBm) to 28 and 15+7 bit-codes
	callsignCode = strfind(alphabet, callsign[0]);
	callsignCode = callsignCode*36 + strfind(alphabet, callsign[1]);
	callsignCode = callsignCode*10 + strfind(alphabet, callsign[2]);
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[3]) - 10;
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[4]) - 10;
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[5]) - 10;
	powerQTHCode = 180*(179 - 10*strfind(alphabetQTH, QTH[0]) - strfind(alphabet, QTH[2])) + 10*strfind(alphabetQTH, QTH[1]) + strfind(alphabet, QTH[3]);
	powerQTHCode = (powerQTHCode<<7) + (powerdBm + 64);

	// 2) convolutional encoding
	/*
	reg0 - 32битный пустой регистр. Выполняем следующие манипуляции 81 раз.
	- сдвигаем регистр reg0 на один разряд влево
	- добавляем поочередно от старшего к младшему бит из callsignCode (затем из powerQTHCode, затем 31 раз нули) младшим в регистр reg0
	- вычисляем биты четности для (reg0 & 0xF2D05351) и добавляем в результат reg162 от младшего к старшему. Затем бит четности для (reg1 & 0xE4613C47)
	В итоге обработан 81 бит и результаты вычислений из каждого регистра дали 81x2 = 162 бита
	TODO: translate ^^^
	*/
	// callsign: 28 bits
	for (i = 0; i < 28; i++) {
		i162byte = i162 / 32;
		i162bit = i162 % 32;
		reg0 <<= 1;
		reg0 = reg0 | (callsignCode>>(27-i)&1);
		reg162[i162byte] |= (parity32Check(reg0 & 0xF2D05351) << i162bit);
		reg162[i162byte] |= (parity32Check(reg0 & 0xE4613C47) << (i162bit+1));
		i162 += 2;
	}
	// power & QTH: 15+7=22 bits
	for (i = 0; i < 22; i++) {
		i162byte = i162 / 32;
		i162bit = i162 % 32;
		reg0 <<= 1;
		reg0 = reg0 | (powerQTHCode>>(21-i)&1);
		reg162[i162byte] |= (parity32Check(reg0 & 0xF2D05351) << i162bit);
		reg162[i162byte] |= (parity32Check(reg0 & 0xE4613C47) << (i162bit+1));
		i162 += 2;
	}
	// zero's: 31 bits
	for (i = 0; i < 31; i++) {
		i162byte = i162 / 32;
		i162bit = i162 % 32;
		reg0 <<= 1;
		reg162[i162byte] |= (parity32Check(reg0 & 0xF2D05351) << i162bit);
		reg162[i162byte] |= (parity32Check(reg0 & 0xE4613C47) << (i162bit+1));
		i162 += 2;
	}

	// 3) interleaving
	sourceIndex = 0;
	for (i = 0; i < 255; i++) {
		targetIndex = byteReverse(i);
		if (targetIndex < 162) {
			reg162i[targetIndex / 32] |= ((reg162[sourceIndex / 32]>>sourceIndex)&1)<<(targetIndex % 32);
			sourceIndex++;
		}
	}
	
	// 4) mask
	for (i = 0; i < 162; i++) {
		messageOutput[i] = (((reg162i[i / 32]>>(i % 32))&1)<<1) + ((reg162mask[i / 32]>>(i % 32))&1);
	}
}

void GPStoQTH6(char* QTH6, double lon, double lat) {
	// LON A..R: WEST -180..0..180 EAST
	// LAT A..R: SOUTH -90..0..90 NORTH
	// Earth = 18 x 18 fields
	// Field = 20deg LON x 10deg LAT = 10x10 squares
	// Square = 24x24 subsquares
	char alphabetQTH[] = "ABCDEFGHIJKLMNOPQRSTUVWX";
	uint8_t s1 = (lon + 180) / 20;
	uint8_t s2 = (lat + 90) / 10;
	uint8_t f1 = (uint8_t)((lon+180) - s1*20)/2;
	uint8_t f2 = (uint8_t)((lat+90) - s2*10);
	uint8_t ss1 = 24*(((lon + 180) - 20*s1 - 2*f1)/2);
	uint8_t ss2 = 24*((lat + 90) - 10*s2 - f2);
	QTH6[0] = alphabetQTH[s1];
	QTH6[1] = alphabetQTH[s2];
	QTH6[2] = 48+f1;
	QTH6[3] = 48+f2;
	QTH6[4] = alphabetQTH[ss1];
	QTH6[5] = alphabetQTH[ss2];
}

int main() {
	char QTH[6];
	char callsign[] = _CALLSIGN;
	uint8_t powerdBm = _POWERDBM;
	uint8_t i;

	GPStoQTH6(QTH, _LON, _LAT);
	printf("%s %c%c%c%c%c%c %d\n", callsign, QTH[0], QTH[1], QTH[2], QTH[3], QTH[4], QTH[5], powerdBm);
	encodeMessage(message, callsign, QTH, powerdBm);
	for (i = 0; i < 162; i++) printf("%u", message[i]);

	return 0;
}
