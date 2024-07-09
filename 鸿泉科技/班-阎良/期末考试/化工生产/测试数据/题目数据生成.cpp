#include <bits/stdc++.h>
using namespace std;
void print(long long x) {
	cout << x << " ";
}
long long mrand(long long x, long long y) {
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
	print(ans);
	return ans;
}
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//┌------------Template Dividing Line------------┐ 
	long long n;
    n = mrand(100000000000, 1000000000000); 
    long long x = n, xin = 0;
    while (n > 4) {
        xin = n / 5 * 2;
        x += xin;
        n = n % 5 + xin;
    }	
    //└------------Template Dividing Line------------┘ 
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//┌------------Template Dividing Line------------┐ 
	cout << x << endl;			
	//└------------Template Dividing Line------------┘ 
}
	// STEP 4 : Modify the cases number
	//┌------------Template Dividing Line------------┐ 
	int ll = 10, rr = 10; //Test Cases Number
	//└------------Template Dividing Line------------┘ 
int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
