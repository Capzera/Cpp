#include <bits/stdc++.h>
using namespace std;
long long main() {
	long long n;
	vector<long long> ans(1000);
	cin >> n;
	ans[0] = ans[1] = 1;
	for (long long i = 2; i < 50; i++) ans[i] = ans[i - 1] + ans[i - 2];
	cout << ans[n - 1] << endl;
	return 0;
}
/*
1134903170
1836311903
2971215073
*/
