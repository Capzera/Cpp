#include <bits/stdc++.h>
using namespace std;
int main() {
	long long seed = 191, i = 1;
	vector<int> mp(23333);
	while (1) {
		seed = seed * seed % 23333;
		//cout << seed << endl;
		if (mp[seed]) break;
		mp[seed] = i++;
	}
	cout << i << endl;
	return 0;
}
