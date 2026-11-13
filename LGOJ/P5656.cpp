#include <bits/stdc++.h>
using namespace std;
long long t, x, y, a, b, c;
void exgcd(long long a, long long b) {
	if (!b) {
		x = 1, y = 0;
		return;
	}
	exgcd(b, a % b);
	long long tx = x;
	x = y;
	y = tx - a / b * y;
}
void solve() {
	scanf("%lld %lld %lld", &a, &b, &c);
	long long g = __gcd(a, b);
	if (c % g) {
		puts("-1");
		return ;
	}
	a /= g, b /= g, c /= g; // 使得a, b互质
	exgcd(a, b); // 解ax + by = 1的特解
	x *= c, y *= c, x = (x % b + b) % b; // x为最小正整数解
	if (!x) x = b; // xmin
	y = (c - a * x) / b; // ymax
	if (y <= 0) { // 无正整数解
		printf("%lld %lld\n", x, y % a + a);
		return;
	}
	long long xmn = x, ymx = y, ymn = y % a ? y % a : a, xmx = (c - b * ymn) / a;
	printf("%lld %lld %lld %lld %lld\n", (xmx - xmn) / b + 1, xmn, ymn, xmx, ymx);
	
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
/*
x + 6y = 2
x = 2, y = 0	
ax + by = gcd(a, b)
解得x0, y0;

通解x, y
ax0 * q + by * q = gcd(a, b) * q = c
q = c / gcd(a, b) = c / d
x = x0 * c / d, y = y0 * c / d;
x, y为方程ax + by = c的一组特解，但不保证x, y为正整数解
方程转化为
a(x + m) + b(y + n) = c
展开得
ax + am + by + bn = c
因为有
ax + by = c
所以有
am + bn = 0，即am + bn = 0的一组关于(m, n)的解，可以和特解配合使用求出所有整数解
设m = t * b / d, n = -t * a / d
am + bn = 0
左边 a * t * b / d + (-t * b * a / d) = 0恒成立
任取一个t，可以计算出相应的m, n，可以计算出相应的x, y，即该解为方程的通解
x = x0 + tb / d
y = y0 - ta / d
其中x0, y0是方程的特解
通过观察可以得出，t增大时，x增大，y减小
设tb / d 为tx, -ta / d为ty
x = x0 + tx
y = y0 + ty

设xmin = x0 + ktx >= 1
k * tx >= 1 - x0
因为这里要求x是最小值，所以t必然是负数，因为t = 0时x = x0，所以想要找到最小正整数解，t必小于0
所以tx < 0
k >= ceil((1 - x0) / tx);
xmin对应的为ymax
所以ymax = (c - a * xmin) / b
当判断出ymax < 0时，方程无正整数解，进入情况2
	情况2：
	ymin = ceil((1 - y0) / ty)
	xmax = (c - b * ymin) / a
	cout << xmin << " " << ymin
*/
