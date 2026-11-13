#include <iostream>
using namespace std;
const int NN = 11;
const int KK = 1e5 + 1;
long long N, K, mod = 1e9 + 7, dp[NN][KK];
int main() {
	cin >> N >> K;
	long long ans = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= K; j++) dp[i][j] = ans;
		for (int j = 1; j << 1 <= K; j++) {
			for (int k = j << 1; k <= K; k += j) {
				dp[i][k] = (dp[i][k] - dp[i - 1][j] + mod) % mod;
			}
			ans = 0;
		}
		for (int j = 1; j <= K; j++) ans = (ans + dp[i][j]) % mod;
	}
	cout << ans;
	return 0;
}
