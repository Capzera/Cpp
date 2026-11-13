#include <bits/stdc++.h>
using namespace std;
string enA, enB, deB, deA;
int key;
int main() {
	cin >> enA >> enB >> deB;
	for (int x = 0; x < 26; x++) {
		if (enB[0] == (enA[0] - 'A' + x) % 26 + 'A') {
			key = x;
		}
	}
	for (auto& x : deB) {
		deA += (x - 'A' - key + 26) % 26 + 'A';
	}
	cout << deA;
	return 0;
}
