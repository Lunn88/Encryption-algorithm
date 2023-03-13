#include<iostream>
#pragma warning (disable : 4996)

using namespace std;

int main() {
	char mes[100];
	char cipher[100];
	memset(cipher, '\0', 100);
	int choice;
	cin >> choice;
	cin.get();
	cin.getline(mes,100);
	int s = 2;
	char table[2][27];
	switch (choice)
	{
	case 1:
		for (int i = 0; i < strlen(mes); i++) {
			int tmp = mes[i] + s;
			if (mes[i] == ' ') {
				cipher[i] = ' ';
				continue;
			}
			else if (97 <= mes[i] && mes[i] <= 122 && tmp > 122) {
				tmp -= 26;
			}
			else if (65 <= mes[i] && mes[i] <= 90 && tmp > 90) {
				tmp -= 26;
			}
			cipher[i] = tmp;
		}
		cout << cipher;
		break;
	case 2:
		strcpy(table[0],"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		strcpy(table[1],"CRYPTOABDEFGHIJKLMNQSUVWXZ");
		for (int i = 0; i < strlen(mes); i++) {
			for (int j = 0; j < 26; j++) {
				if (mes[i] == table[0][j] || mes[i] == (table[0][j] + 32)) {
					cipher[i] = table[1][j];
					break;
				}
			}
		}
		cout << cipher;
		break;
	case 4:
		strcpy(table[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		strcpy(table[0], "CRYPTOABDEFGHIJKLMNQSUVWXZ");
		for (int i = 0; i < strlen(mes); i++) {
			for (int j = 0; j < 26; j++) {
				if (mes[i] == table[0][j] || mes[i] == (table[0][j] + 32)) {
					cipher[i] = table[1][j];
					break;
				}
			}
		}
		cout << cipher;
		break;
	case 3:
		int guess_s = 1;
		char all[25][100];
		for (int i = 0; i < 25; i++) {
			memset(all[i], '\0', 100);
		}
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < strlen(mes); j++) {
				int tmp = mes[j] + guess_s;
				if (mes[j] == ' ') {
					all[i][j] = ' ';
					continue;
				}
				else if (97 <= mes[j] && mes[j] <= 122 && tmp > 122) {
					tmp -= 26;
				}
				else if (65 <= mes[j] && mes[j] <= 90 && tmp > 90) {
					tmp -= 26;
				}
				all[i][j] = tmp;
			}
			cout << all[i] << endl;
			guess_s += 1;
		}
	}
}