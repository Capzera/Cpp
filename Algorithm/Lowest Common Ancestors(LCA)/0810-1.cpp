#include <bits/stdc++.h>
using namespace std;

int main() {
	
	return 0;
}
RMQ问题(Range Maximum/Minimun Query)
创建时间，查询时间
1. 朴素求解(O(n), O(n))
	优点： 代码短，好写，速度快
	缺点：时间效率极低


2. 线段树(O(n), O(log n))
	优点：构造时间效率高，查询时间效率不低，可复用性强，适用问题范围广，还适用于区间求和问题
	缺点：代码极其复杂难写
3. ST表(O(nlogn) O(1))
	优点：代码经典，模板性强，查询效率极高
	缺点：ST表被写好后不可更新，空间消耗巨大
前缀和， 差分
Sparse adj. 稀疏的，稀少的，零落的
ST表(Sparse Table)，同名同意稀疏表
倍增算法(Multiple) + DP

f(l, r)
st[i][j] 从下标i开始向右数2^j个数字的最大值
st[i][0] 从下标i开始向右数2^0=1个数字的最大值

lg[N];
st[N][25] // st表的定义

st[i][0] = a[i]; // st表转移的边界
st[i][j] = max(st[i][j - 1], st[i + (1 << j - 1)][j - 1]); // st表的状态转移方程

对[l, r] 做偏二分

（7， 33）  len = 33 - 7 + 1 = 27




k = lg(r - l + 1)?  k = 4
max([L, R]) = max(st[l][k])
st[l][k] = (7, 23)的最大值
st[r - (1 << k) + 1][k] = (18, 33)的最大值

int query(int l, int r) {
	int k = lg[r - l + 1];
	return max(st[l][k], st[r - (1 << k) + 1][k]);
}






st[i][0] 边界
st[i][j] 从i开始向右的2^j个数的最大值
st[i][j - 1] 从i开始向右的2^(j - 1)个数的最大值，即从i起向右的2^j的二分点
st[i + (1 << j - 1)][j - 1] 从 i向右的2^j的二分点处向右延伸2^(j - 1) 个数的最大值


lg[0] = -1;
for (int i = 1; i <= N; i++) lg[i] = lg[i >> 1] + 1;
for (int j = 1; j <= lg[N]; j++) {
	for (int i = 1; i <= n - (1 << j) + 1; i++) {
		st[i][j] = max(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
	}
}

while (q--) {
	cin >> l >> r;
	cout << query(l, r) << ednl
}
