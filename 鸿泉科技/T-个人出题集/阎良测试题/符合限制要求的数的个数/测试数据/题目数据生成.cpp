#include <bits/stdc++.h>
using namespace std;
void print(long long x) {
	cout << x << " ";
}
long long mrand(long long x, long long y, int p = 0) {
	long long a = INT_MIN, b = INT_MIN, c = INT_MIN, d = INT_MIN, ans = LLONG_MIN;
	long long eps1 = 1000000000000;
	while (ans < x || ans > y) {
		if (y <= 1e4) {
			ans = rand() % y;
		}
		else if (y <= 1e5) {
			a = rand() % 100;
			b = rand() % 1000;
			ans = a * 1000 + b;
		}
		else if (y <= 1e6) {
			a = rand() % 1000;
			b = rand() % 1000;
			ans = a * 1000 + b;
		}
		else if (y <= 1e9) {
			a = rand() % 1000;
			b = rand() % 1000;
			c = rand() % 1000;
			ans = a * 1e6 + b * 1e3 + c;
		}
		else if (y <= eps1) {
			a = rand() % 1000;
			b = rand() % 1000;
			c = rand() % 1000;
			d = rand() % 1000;
			ans = a * 1e9 + b * 1e6 + c * 1e3 + d;
		}
	}
	if (!p) print(ans);
	return ans;
}
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//┌------------Template Dividing Line------------┐ 
	int a, b, c, x, y;
	vector<int> ans;
	int n = mrand(5000, 10000, 1);
	for (int i = 0 ; i < n; i++) {
		a = mrand(1, 100000);
		b = mrand(a + 1, 10000000);
		c = mrand(b + 1, 1000000000);
		x = mrand(1, 100000);
		y = mrand(1, 100000);
		a += x, c -= y;
		cout << endl;
		int res = max(0, c - a + 1 - (b >= a && b <= c));
		ans.push_back(res);
	}
	cout << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
    //└------------Template Dividing Line------------┘ 
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//┌------------Template Dividing Line------------┐ 
	for (auto& x : ans) {
		cout << x << endl;
	}			
	//└------------Template Dividing Line------------┘ 
}
	// STEP 4 : Modify the cases number
	//┌------------Template Dividing Line------------┐ 
	int ll = 2, rr = 10; //Test Cases Number
	//└------------Template Dividing Line------------┘ 
int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 

