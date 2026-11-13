#include <bits/stdc++.h>
using namespace std;
int n, x;
string s;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		int len = s.size(), sum = 0;
		for (int i = 0; i < len; i++) {
			sum += pow(s[i] - '0', len);
		}
		cout << (to_string(sum) == s ? "T\n" : "F\n");
	}
	return 0;
}
