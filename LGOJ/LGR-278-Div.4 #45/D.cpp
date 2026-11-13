#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
int n, k, x, a[N], mp1[N], mp2[N];
int main() {
	cin >> n;
	for (int i = 1; i <= 2 * n; i++) {
		cin >> a[i];
		if (!mp1[a[i]]) mp1[a[i]] = i;
		else mp2[a[i]] = i;
	}
	for (int i = 1; i <= 2 * n; i++) {
		cout << (mp1[a[i]] == i ? mp2[a[i]] : mp1[a[i]]) << " ";
	}
	return 0;
}
