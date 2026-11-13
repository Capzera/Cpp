#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << char((i % 26 + j) % 26 + 'A');
		}
		cout << endl;
	}
	return 0;
}
