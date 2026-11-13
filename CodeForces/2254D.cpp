#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 2e5 + 5;
int t, n, b[N];
void solve() {
	map<int, int> mp;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> b[i];
		mp[b[i]]++;
	}
	vector<pair<int, int> > B(mp.begin(), mp.end());
	if (B[0].first) {cout << "-1\n"; return;}
	int k = B.size(), lst = 0;
	for (int i = 0; i < k - 1; i++) {
		if ((B[i + 1].first - B[i].first) % B[i].second) {cout << "-1\n"; return;}
		int A = (B[i + 1].first - B[i].first) / B[i].second;
		if (A <= lst) {cout << "-1\n"; return;}
		mp[B[i].first] = lst = A;
	}
	mp[B[k - 1].first] = lst + 1;
	for (int i = 1; i <= n; i++) cout << mp[b[i]] << ' ';
	cout << '\n';
}
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) solve();
	return 0;
}
//
//2 5 2 5 6
//0 4 0 4 14
//
//
//性质1： 由于b[i] 是 a 数组中所有小于 a[i] 元素的和
//如果a[j] < a[i]  ==>  b[i] 一定包含了小于 a[j] 的和
//===> b[i] > b[j];
//如果 a[i] >= a[j] ==> b[i] >= b[j]
//
//换种说法：a数组每个元素的rank值必然和b数组相等
//
//结论1 ： b 数组的最小值 ==> 对应的a[i] 也是a数组的最小值 ==> b数组必然包含 0
//
//应该对B数组排序 + 统计B数组每个元素出现的次数 + 去重
//
//重新将B数组写作为 B[1], B[2] ... B[K]。 令去重后的B数组共k个元素
//
//A数组应该出现 K 种不同的元素
//
//∵ B[1] < B[2] < ... < B[K]
//设 cnt[i] 表示 B[i] 的出现次数
//∴ A[1] < A[2] < ... < A[K]
//
//
//B[1] = 0
//B[2] = B[1] + A[1] * cnt[1] ==> A[1] = (B[2] - B[1]) / cnt[1];
//B[3] = B[2] + A[2] * cnt[2] ==> A[2] = (B[3] - B[2]) / cnt[2];
//B[4] = B[3] + A[3] * cnt[3] ==> A[3] = (B[4] - B[3]) / cnt[3];
//结论2：(B[i + 1] - B[i]) 如果不是 cnt[i] 的倍数，无解
//A[i] = (B[i + 1] - B[i]) / cnt[i];
//结论3：如果计算出的A[i] <= A[i - 1]，无解
//
//A[k] = (B[k + 1] - B[k]) / cnt[K]; --->
//A[k] = A[k - 1] + 1 ----> 末尾元素使得本题字典序最small
//
//A， B 均是已排序数组 ---> 未排序数组
//B[i] -> 最终答案a[]
//
//mp[0] = 2
//mp[4] = 5
//mp[14] = 6
//for (int i = 1; i <= n; i++) cout << mp[b[i]] << " ";

