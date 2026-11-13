#include <bits/stdc++.h>
using namespace std;
string s, mp[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "a", "both", "another", "first", "second", "third"}; //对应
int f, key[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 00, 21, 44, 69, 96, 25, 56, 89, 24, 61, 0, 1, 4, 1, 1, 4, 9};
int main() {
	vector<int> word;
	for (int i = 0; i < 6; i++) {
		cin >> s;
		for (int j = 0; j < 27; j++) {
			if (s != mp[j]) continue;
			word.push_back(key[j]);
			break;
		}
	}
	sort(word.begin(), word.end());
	for (auto& x : word) {
		if (f) printf("%.2d", x);
		else if (x) {
			cout << x;
			f = 1;
		}
	}
	if (!f) cout << 0;
	return 0;
}
