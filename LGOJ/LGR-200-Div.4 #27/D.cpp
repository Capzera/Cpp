#include <bits/stdc++.h>
using namespace std;
long long x, y, w, k;
int main() {
	cin >> x >> y >> w >> k;
	int i = 1;
	for (int sum = 0; sum < k; i++) {
		sum += i / w;
		if (i >= x) sum += y;
	}
	cout << i - 1;
	return 0;
}
