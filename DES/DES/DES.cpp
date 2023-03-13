#include <iostream>
#include <string>
#include <math.h>
#include<cstdlib>
#include<ctime>

using namespace std;

string round_keys[16];
string text = "000000000000000095F8A5E5DD31D900DD7F121CA50156192E8653104F3834EA4BD388FF6CD81D4F20B9E767B2FB145655579380D77138EF6CC5DEFAAF04512F0D9F279BA5D87260D9031B0271BD5A0A";

string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 28; i++)
	{
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
}

string shift_left_twice(string key_chunk) {
	key_chunk = shift_left_once(key_chunk);
	key_chunk = shift_left_once(key_chunk);
	return key_chunk;
}

string Xor(string a, string b) {
	string result = "";
	int size = a.size();
	for (int i = 0; i < size; i++)
	{
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
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

string decimal_to_binary(int decimal) {
	string binary;
	while (decimal != 0)
	{
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal /= 2;
	}
	while (binary.length() < 4)
	{
		binary = "0" + binary;
	}
	return binary;
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

string binary_to_hex(string binary) {
	string hex;
	int size = binary.length();
	string tmp;
	for (int i = 0; i <= size; i++)
	{
		if (i % 4 == 0 && i != 0) {
			if (tmp == "0000") hex += "0";
			else if (tmp == "0001") hex += "1";
			else if (tmp == "0010") hex += "2";
			else if (tmp == "0011") hex += "3";
			else if (tmp == "0100") hex += "4";
			else if (tmp == "0101") hex += "5";
			else if (tmp == "0110") hex += "6";
			else if (tmp == "0111") hex += "7";
			else if (tmp == "1000") hex += "8";
			else if (tmp == "1001") hex += "9";
			else if (tmp == "1010") hex += "A";
			else if (tmp == "1011") hex += "B";
			else if (tmp == "1100") hex += "C";
			else if (tmp == "1101") hex += "D";
			else if (tmp == "1110") hex += "E";
			else if (tmp == "1111") hex += "F";
			tmp = "";
		}
		tmp += binary[i];
	}
	return hex;
}

void generate_keys(string key) {
	// PC1 table 
	int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};
	// PC2 table
	int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
	};

	// Compressing the key using the PC1 table
	string perm_key = "";
	for (int i = 0; i < 56; i++)
	{
		perm_key += key[pc1[i] - 1];
	}

	// Dividing the result into two parts.
	string left = perm_key.substr(0, 28);
	string right = perm_key.substr(28, 28);

	// Generate 16 keys
	for (int i = 0; i < 16; i++)
	{
		// Rounds 1, 2, 9, 16 the key_chunks shift left by one
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// Other rounds the key_chunks shift left by two
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// Combine
		string combined_key = left + right;

		string round_key = "";
		// Transposing the key bit using the PC2 table
		for (int i = 0; i < 48; i++)
		{
			round_key += combined_key[pc2[i] - 1];
		}
		round_keys[i] = round_key;
	}
}

string DES_Encrypte(string text) {
	// Initial permutation table
	int initial_permutation[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
	};
	// The expansion table
	int expansion_table[48] = {
	32,1,2,3,4,5,4,5,
	6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,
	28,29,28,29,30,31,32,1
	};
	// S box
	int substition_boxes[8][4][16] =
	{ {
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	} };
	// The permutation table
	int permutation_table[32] = {
	16,7,20,21,
	29,12,28,17,
	1,15,23,26,
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25
	};
	// Inverse permutation table
	int inverse_permutation[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
	};

	// Initial permute
	string perm = "";
	for (int i = 0; i < 64; i++)
	{
		perm += text[initial_permutation[i] - 1];
	}
	// Dividing the result into two parts
	string left = perm.substr(0, 32);
	string right = perm.substr(32, 32);

	// Encrypted 16 times
	for (int i = 0; i < 16; i++)
	{
		string right_expanded = "";
		// Use expansion table to expand right part
		for (int i = 0; i < 48; i++)
		{
			right_expanded += right[expansion_table[i] - 1];
		}
		// Xor with the key
		string xored = Xor(round_keys[i], right_expanded);
		string res = "";

		// Divide the result into 8 parts and pass through S box
		for (int i = 0; i < 8; i++)
		{
			string str_row = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
			int row = binary_to_decimal(str_row);
			string str_col = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);
			int col = binary_to_decimal(str_col);
			int int_res = substition_boxes[i][row][col];
			res += decimal_to_binary(int_res);
		}

		// Permute again
		string perm2 = "";
		for (int i = 0; i < 32; i++)
		{
			perm2 += res[permutation_table[i] - 1];
		}
		// Xor with left part
		xored = Xor(perm2, left);
		left = xored;
		if (i < 15) {
			string tmp = right;
			right = xored;
			left = tmp;
		}
	}
	//Combine
	string combined_text = left + right;
	string ciphertext = "";
	// Inverse initial permute
	for (int i = 0; i < 64; i++)
	{
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}
	return ciphertext;
}

int main() {
	string key = "10316E028C8F3B4A";
	for (int j = 0; j < 10; j++)
	{
		if (j > 0) {
			key = "0101010101010101";
		}
		cout << j + 1 << '.' << endl;
		cout << "Key:\t\t";
		for (int i = 0; i < 16; i++) {
			if (i % 2 == 0) {
				cout << "0x" << key[i];
				i++;
			}
			cout << key[i] << ' ';
		}
		cout << endl;
		key = hex_to_binary(key);
		generate_keys(key);

		string plain_text = text.substr(j * 16, 16);
		cout << "Plain text:\t";
		for (int i = 0; i < 16; i++) {
			if (i % 2 == 0) {
				cout << "0x" << plain_text[i];
				i++;
			}
			cout << plain_text[i] << " ";
		}
		cout << endl;
		plain_text = hex_to_binary(plain_text);
	
		string cipher = DES_Encrypte(plain_text);
		cipher = binary_to_hex(cipher);

		cout << "Encrypted text:\t";
		for (int i = 0; i < 16; i++) {
			if (i % 2 == 0) {
				cout << "0x" << cipher[i];
				i++;
			}
			cout << cipher[i] << " ";
		}
		cout << endl;
		
		cipher = hex_to_binary(cipher);
		for (int i = 0; i < 8; i++) {
			swap(round_keys[i], round_keys[15 - i]);
		}
		plain_text = DES_Encrypte(cipher);
		plain_text = binary_to_hex(plain_text);
		cout << "Decrypted text:\t";
		for (int i = 0; i < 16; i++) {
			if (i % 2 == 0) {
				cout << "0x" << plain_text[i];
				i++;
			}
			cout << plain_text[i] << " ";
		}
		cout << endl << endl;
	}
	system("pause");
}