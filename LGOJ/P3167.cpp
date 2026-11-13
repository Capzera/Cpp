#include <bits/stdc++.h>
using namespace std;
string model, text;
int n;
bool check() {
	int i = model.size() - 1, j = text.size() - 1;
	while (i >= 0 || j >= 0) {
		if (j >= 0) {
			if (model[i] == text[j] || model[i] == '?') {
				i--, j--;
				continue;
			}
			return 0;
		}
		for (int x = i; x >= 0; x--) {
			if (model[x] != '*') return 0;
		}
	}
	return i < 0;
}
int main() {
	cin >> model >> n;
	for (int i = 1; i <= n; i++) {
		cin >> text;
		cout << (check() ? "Yes\n" : "No\n");
	}
	return 0;
}
