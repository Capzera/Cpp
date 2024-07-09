#include <iostream>
using namespace std;

int main() {
	int a;
	cin >> a;
	char b[100];
	int n[100][a];

	for (int i = 0; i < a; i++) {
		cin >> b;
		int m = sizeof(b);
		int c = 0;
		for (int j = 0; j < m; j++) {
			if (b[j] != '-') {
				if (b[j] >= 'A' && b[j] <= 'P') {
					n[c]
					[i] = (b[j] - 'A') / 3 + 2;
				} else if (b[j] >= 'R' && b[j] <= 'Y') {
					n[c][i] = (b[j] - 'B') / 3 + 2;

				} else {
					n[c][i] = b[j] - '0';
				}
				c++;
			}
		}
	}
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < 7; j++) {
			cout << n[j][i] << ' ';
		}
		cout << endl;
	}
	return 0;
}
