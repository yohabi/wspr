//gcc pro.c -Wno-pointer-to-int-cast

/*
type 1:
callsignCode (aka N) takes 28bit: [0 .. 2^28] == [0 .. 262177560]
QTHCode     (aka M1) takes 15bit: [0 .. 2^15] == [0 .. 32768]
powerQTHCode (aka M) takes 22bit: [0 .. 2^22] == [0 .. 4194304]

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
char alphabetQTH[] = "ABCDEFGHIJKLMNOPQR";
char callsign[] = "UB1CBV";
char QTH[] = "KO59";
uint8_t power = 30;
//syncMask162 = '110000001000111000100101111000000010010100000010110011010001101000011010101010010010110001101010001000001001001110110011010001110000010100110000000110101100011000'

int strfind(char str[], char chr) {
	if (strchr(str, chr) != NULL) return (int)strchr(str, chr) - (int)str;
	else return -1;
}

void main() {
	uint32_t callsignCode;
	uint32_t powerQTHCode;

	callsignCode = strfind(alphabet, callsign[0]);
	callsignCode = callsignCode*36 + strfind(alphabet, callsign[1]);
	callsignCode = callsignCode*10 + strfind(alphabet, callsign[2]);
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[3]) - 10;
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[4]) - 10;
	callsignCode = callsignCode*27 + strfind(alphabet, callsign[5]) - 10;
	printf("callsignCode\t%12u\t%08xh\n", callsignCode, callsignCode);

	powerQTHCode = 180*(179 - 10*strfind(alphabetQTH, QTH[0]) - strfind(alphabetQTH, QTH[2])) + 10*strfind(alphabetQTH, QTH[1]) + strfind(alphabetQTH, QTH[3]);
	printf("QTHCode\t\t%12u\t%08xh\n", powerQTHCode, powerQTHCode);

	powerQTHCode = powerQTHCode<<7 + power + 64;
	printf("powerQTHCode\t%12u\t%08xh\n", powerQTHCode, powerQTHCode);

	//shift xCodes to MSB
	callsignCode = callsignCode<<4;
	powerQTHCode = powerQTHCode<<10;



	printf("callsignCode\t%12u\t%08xh\n", callsignCode, callsignCode);
	printf("powerQTHCode\t%12u\t%08xh\n", powerQTHCode, powerQTHCode);
}
