#include <bits/stdc++.h>
using namespace std;
long long ans;
int n;
bool isPrime(long long x) {
	if (x == 1) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	long long n = sqrt(x);
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
void dfs(int idx) {
	if (idx == n) {
		cout << ans << endl;
		return;
	}
	for (int i = 1; i <= 9; i++) {
		long long nxt = ans * 10 + i;
		if (isPrime(nxt) == 0) continue;
		ans = nxt;
		dfs(idx + 1);
		ans /= 10;
	}
}
int main() {
	cin >> n;
	dfs(0);
	return 0;
}


class Student {
private:
	int n;
public:
	Student();
	void show();
	~Student();
};
/*

google 认证C++代码风格

*/
