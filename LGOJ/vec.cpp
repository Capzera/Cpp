#include <bits/stdc++.h>
using namespace std;
int main() {
	vector<int> a(10);
	iota(a.begin() + 1, a.begin() + 6, 7);
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	return 0;
}
