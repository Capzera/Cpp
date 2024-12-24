#include <iostream>
using namespace std;
int main() {
	long long k, a, q, mod = 722733748;
	cin >> k >> a >> q;
	long long human = a, ans = a;
	for (int i = 1; i < k; i++) {
		human = human * q % mod;
		ans = ans * human % mod;
	}
	cout << ans << endl;
	return 0;
}
