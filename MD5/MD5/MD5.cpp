#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

#pragma warning (disable : 4996)

typedef union uwb {
	unsigned w;
	unsigned char b[4];
} MD5union;

typedef unsigned DigestArray[4];

unsigned F(unsigned abcd[]) {
	return (abcd[1] & abcd[2]) | (~abcd[1] & abcd[3]);
}

unsigned G(unsigned abcd[]) {
	return (abcd[3] & abcd[1]) | (~abcd[3] & abcd[2]);
}

unsigned H(unsigned abcd[]) {
	return  abcd[1] ^ abcd[2] ^ abcd[3];
}

unsigned I(unsigned abcd[]) {
	return abcd[2] ^ (abcd[1] | ~abcd[3]);
}

unsigned* calc_table()
{
	unsigned* k = new unsigned[64];
	double s, pwr;
	int i;

	pwr = pow(2.0, 32);
	for (i = 0; i < 64; i++) {
		s = fabs(sin(1.0 + i));
		k[i] = (unsigned)(s * pwr);
	}
	return k;
}

unsigned rol(unsigned r, short N)
{
	unsigned  mask1 = (1 << N) - 1;
	return ((r >> (32 - N)) & mask1) | ((r << N) & ~mask1);
}

typedef unsigned(*DgstFctn)(unsigned a[]);

unsigned* MD5Hash(string msg)
{
	DigestArray ABCD = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
	DgstFctn ff[] = { &F, &G, &H, &I };
	short M[] = { 1, 5, 3, 7 };
	short OFF[] = { 0, 1, 5, 0 };
	short rot0[] = { 7, 12, 17, 22 };
	short rot1[] = { 5, 9, 14, 20 };
	short rot2[] = { 4, 11, 16, 23 };
	short rot3[] = { 6, 10, 15, 21 };
	short* rots[] = { rot0, rot1, rot2, rot3 };
	unsigned* k = calc_table();

	union {
		unsigned w[16];
		char     b[64];
	}mm;
	unsigned char* msg2;

	static DigestArray ABCD_tmp;
	for (int i = 0; i < 4; i++)
		ABCD_tmp[i] = ABCD[i];

	// preprocess the message
	int mlen = msg.length();
	int times_64B = 1 + (mlen + 8) / 64;
	msg2 = (unsigned char*)malloc(64 * times_64B);
	memcpy(msg2, msg.c_str(), mlen); // copy to msg2 and fill
	msg2[mlen] = (unsigned char)0x80; // add 0x80
	for (int i = mlen + 1; i < 64 * times_64B; i++) {
		// fill the rest with zero
		msg2[i] = 0;
	}
	
	MD5union u;
	u.w = 8 * mlen;
	memcpy(msg2 + 64 * times_64B - 8, &u.w, 4); // set length(little endian)

	DigestArray abcd;
	DgstFctn fctn;
	for (int t = 0; t < times_64B; t++)
	{
		memcpy(mm.b, msg2 + 64*t, 64);
		for (int i = 0; i < 4; i++) 
			abcd[i] = ABCD_tmp[i];
		for (int i = 0; i < 4; i++) {
			// 0~15 use F, 16~31 use G...
			fctn = ff[i];
			short* rotn = rots[i];
			short m = M[i];
			short offset = OFF[i];
			for (int j = 0; j < 16; j++) {
				short g = (m * j + offset) % 16;
				unsigned f = abcd[1] + rol(abcd[0] + fctn(abcd) + k[j + 16 * i] + mm.w[g], rotn[j % 4]);

				abcd[0] = abcd[3];
				abcd[3] = abcd[2];
				abcd[2] = abcd[1];
				abcd[1] = f;
			}
		}
		for (int i = 0; i < 4; i++)
			// A = A + a, B = B + b...
			ABCD_tmp[i] += abcd[i];
	}

	return ABCD_tmp;
}

string GetMD5String(string msg) {
	string str;
	int j, k;
	unsigned* d = MD5Hash(msg);
	MD5union u;
	for (j = 0; j < 4; j++) {
		u.w = d[j];
		char s[9];
		sprintf(s, "%02X%02X%02X%02X", u.b[0], u.b[1], u.b[2], u.b[3]);
		str += s;
	}
	return str;
}

string hex_to_binary(string hex) {
	string binary;
	int size = hex.length();
	for (int i = 0; i < size; i++)
	{
		char h = hex[i];
		switch (h)
		{
		case '0':binary += "0000"; break;
		case '1':binary += "0001"; break;
		case '2':binary += "0010"; break;
		case '3':binary += "0011"; break;
		case '4':binary += "0100"; break;
		case '5':binary += "0101"; break;
		case '6':binary += "0110"; break;
		case '7':binary += "0111"; break;
		case '8':binary += "1000"; break;
		case '9':binary += "1001"; break;
		case 'A':binary += "1010"; break;
		case 'B':binary += "1011"; break;
		case 'C':binary += "1100"; break;
		case 'D':binary += "1101"; break;
		case 'E':binary += "1110"; break;
		case 'F':binary += "1111"; break;
		}
	}
	return binary;
}

string decimal_to_binary(int decimal) {
	string binary;
	while (decimal != 0)
	{
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal /= 2;
	}
	while (binary.length() < 8)
	{
		binary = "0" + binary;
	}
	return binary;
}

int binary_to_decimal(string binary) {
	int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (binary[i] == '1') {
			decimal += pow(2, counter);
		}
		counter++;
	}
	return decimal;
}

int diff(string hex1, string hex2) {
	string bin1 = hex_to_binary(hex1);
	string bin2 = hex_to_binary(hex2);
	int d = 0;
	for (int i = 0; i < bin1.length(); i++) {
		if (bin1[i] != bin2[i]) {
			d++;
		}
	}
	return d;
}

int main() {
	srand(time(0));
	char *abc = new char[3];
	memset(abc, 0, sizeof(abc));
	abc[0] = 97;
	abc[1] = 98;
	abc[2] = 99;
	string last = GetMD5String(abc);
	cout << 0 << ". " << last << endl;
	for (int i = 1; i < 9; i++) {

		int c = rand() % 3;
		string bin;
		switch (c)
		{
			case 0:
				bin = decimal_to_binary(abc[0]);
				break;
			case 1:
				bin = decimal_to_binary(abc[1]);
				break;
			case 2:
				bin = decimal_to_binary(abc[2]);
				break;
		}
		
		int bit = rand() % bin.length();
		if (bin[bit] == '0')
			bin[bit] = '1';
		else
			bin[bit] = '0';

		int dec = binary_to_decimal(bin);
		abc[c] = (char)dec;

		cout << i << ". " << GetMD5String(abc) << ' ' << diff(last, GetMD5String(abc)) << endl;
		last = GetMD5String(abc);
	}
}