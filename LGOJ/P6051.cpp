#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	while (getline(cin, s)) {
		long long sum = 0, num = 0, f = 1, flag = 0;
		for (int i = 0; i < s.size(); i++) {
			char x = s[i];
			if (!isdigit(x)) {
				if (!num && x == '-') {
					f = -1;
					continue;
				}
				sum += f * num;
				num = 0;
				f = 1;
			}
			if (isdigit(x)) {
				flag = 1;
				num = num * 10 + x - '0';
			}
		}
		if (num) sum += num * f;	
		if (flag) cout << sum << endl;
	}
	return 0;
}
