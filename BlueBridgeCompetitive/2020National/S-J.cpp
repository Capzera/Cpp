#include <bits/stdc++.h>
using namespace std;
#define mod 1000000007
int main() {
    int m, n, w;
    cin >> m >> n >> w;
    int r1, c1, w1, r2, c2, w2, maxv = max(m, max(n, w));
    cin >> r1 >> c1 >> w1 >> r2 >> c2 >> w2;
    if (r1 > r2 || c1 > c2 || w1 > w2) {
        swap(r1, r2), swap(c1, c2), swap(w1, w2);
    }
    vector<int> prime, st(maxv + 1);
    vector<vector<long long>> dp(maxv + 1, vector<long long>(maxv + 1));
    vector<vector<long long>> f(m / 2 + n / 2 + w / 2 + 1, vector<long long> (maxv / 2 + 1));
    for (int i = 2; i <= maxv; i++) {
        if (!st[i]) prime.push_back(i);
        for (int j = 0; j < prime.size() && prime[j] * i <= maxv; j++)
            st[prime[j] * i] = 1;
    }
    dp[1][0] = 1;
    for (int i = 1; i <= maxv; i++)
        for (int j = 1; j <= (i - 1) / 2; j++)
            for (int k = 0; k < prime.size() && prime[k] < i; k++)
                dp[i][j] = (dp[i][j] + dp[i - prime[k]][j - 1]) % mod;
    for (int i = 1; i < f.size(); i++) f[i][1] = 1;
    for (int i = 2; i < f.size(); i++)
        for (int j = 2; j < f[i].size(); j++)
            f[i][j] = (f[i - 1][j] + f[i][j - 1]) % mod;
    auto solve =[&](int r, int c, int h) -> long long {
        long long ans = 0;
        int tx = r / 2, ty = c / 2, tz = h / 2;
        vector<int> dp2(tx + ty + tz + 1);
        for (int i = 0; i <= tx; i++) {
            for (int j = 0; j <= ty; j++) {
                dp2[i + j] = (dp2[i + j] + dp[r][i] * dp[c][j] % mod * f[i + j][i] % mod) % mod;
            }
        }
        for (int i = 0; i <= tx + ty; i++) {
            for (int j = 0; j <= tz; j++) {
                ans = (ans + dp2[i] * dp[h][j] % mod * f[i + j][j] % mod) % mod; 
            }
        }
        return ans;
    };
    long long ans1 = solve(m, n, w);
    long long ans2 = solve(r1, c1, w1) * solve(m - r1 + 1, n - c1 + 1, w - w1 + 1) % mod;
    long long ans3 = solve(r2, c2, w1) * solve(m - r2 + 1, n - c2 + 1, w - w2 + 1) % mod;
    long long ans4 = (solve(r1, c1, w1) * solve(r2 - r1 + 1, c2 - c1 + 1, w2 - w1 + 1) % mod) * solve(m - r2 + 1, n - c2 + 1, w - w2 + 1) % mod;
    cout << ans1 << " " << ans2 << " " << ans3 << " " << ans4 << endl;
    cout << (ans1 - ans2 - ans3 + ans4 + mod) % mod << endl;
    system("pause");
    return 0;
}
/*
起点~终点 - 起点~r1c1w1 - 起点~r2c2w2 + r1c1w1 ~ r2c2w2;
const int N = 1e5+5;
const ll mod = 1e9+7;
int p[505], m;
ll c[1505][505], dp2[1505], dp[1005][505];
void C(){  //C_A^B
	for(int i = 0; i < 1505; i++) c[i][0] = 1;
	for(int i = 1; i < 1505; i++)
		for(int j = 1; j < 505; j++)
			c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % mod;
}
ll calc(int r, int cc, int w){
	ll ans = 0;
	int sr = r / 2;  //走r行最多需要r/2次，每次走两行 
	int sc = cc / 2;
	int sw = w / 2;
	memset(dp2, 0, sizeof(dp2));
	for(int i = 0; i <= sr; i++){  //r行走几次 
		for(int j = 0; j <= sc; j++){  //c列走几次 
			dp2[i + j] = (dp2[i + j] + dp[r][i] * dp[cc][j] % mod * c[i + j][i] % mod) % mod;
//			cout<<dp2[i + j]<<endl; 
		}
	}
	for(int i = 0; i <= sr + sc; i++){
		for(int j = 0; j <= sw; j++){
			ans = (ans + dp2[i] * dp[w][j] % mod * c[i + j][j] % mod) % mod;
//			cout<<ans<<endl;
		}
	}
	return ans;
}
int main(){ 
	int r, c, w;
	cin>>r>>c>>w;
	int r1, c1, w1, r2, c2, w2;
	cin>>r1>>c1>>w1>>r2>>c2>>w2;
	C();
	int n = max(max(r, c), w); 
	ll ans;
	ans = calc(r, c, w) % mod;
//	cout<<ans<<endl;
	ans -= calc(r1, c1, w1) * calc(r - r1, c - c1, w - w1) % mod;  //从r1到r的方案数 
//	cout<<ans<<endl;
	ans -= calc(r2, c2, w2) * calc(r - r2, c - c2, w - w2) % mod;  //从r2到r的方案数 
//	cout<<ans<<endl;
	if(r1 >= r2 && c1 >= c2 && w1 >= w2)
		ans += calc(r2, c2, w2) * calc(r1 - r2, c1 - c2, w1 - w2) % mod * calc(r - r1, c - c1, w - w1) % mod;
	else if(r1 <= r2 && c1 <= c2 && w1 <= w2)
		ans += calc(r1, c1, w1) * calc(r2 - r1, c2 - c1, w2 - w1) % mod * calc(r - r2, c - c2, w - w2) % mod;
	cout<<(ans % mod + mod) % mod<<endl;
	return 0;
} 
5 6 1
3 4 1 1 2 1
*/