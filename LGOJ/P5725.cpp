#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	string s = "01";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << s;
			s[1]++;
			if (s[1] > '9') s[1] = '0', s[0]++;
		}
		cout << endl;
	}
	cout << endl;
	s = "01";
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < 2 * n - 2 * i; j++) cout << " ";
		for (int j = 0; j < i; j++) {
			cout << s;
			s[1]++;
			if (s[1] > '9') s[1] = '0', s[0]++;
		}
		cout << endl;
	}
	return 0;
}
/*
1 2 * n - 2 * i
1 6
2 4
3 2
4 0
n 
*/
