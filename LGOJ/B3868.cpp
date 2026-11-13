#include <bits/stdc++.h>
using namespace std;
long long n;
string s;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		int f2 = 1, f8 = 1, f10 = 1, f16 = 1;
		for (auto& ch : s) {
			if (ch > '1') f2 = 0;
			if (ch > '7') f8 = 0;
			if (isalpha(ch)) f10 = 0;
			if (ch > 'F') f16 = 0;
		}
		cout << f2 << " " << f8 << " " << f10 << " " << f16 << endl;
	}
	return 0;
}
