#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
struct node {
	long long w, v;
	double price;
	void read() {
		cin >> w >> v;
		price = v / w; // 单位重量的价格计算
	}
	
	bool operator < (const struct node& rhs) const {
		return price > rhs.price;
	}
} a[N];
long long n, m;
double ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n && m > 0; i++) {
		int among = min(a[i].w, m); // 当前物品可以拿取的量
		ans += among * a[i].price; // 数量 × 单位价格
		m -= among;
	}
	printf("%.2lf", ans);
	return 0;
}
