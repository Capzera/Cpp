同余式

性质一：同余式的自反性
a ≡ a (mod p)

性质二：同余式的对称性
a ≡ b(mod p) <=> b ≡ a(mod p)

性质三：同余式的传递性
a ≡ b (mod p) 同时, b ≡ c (mod p) ，所以 a ≡ c(mod p)

性质四：同余式的加法原理
a ≡ b(mod p)，同时 c ≡ d(mod p)
所以 
a + c ≡ b + d (mod p)
a + d ≡ b + c (mod p)

性质五：同余式的乘法原理
a ≡ b (mod p)， 同时 c ≡ d(mod p)
ac ≡ bd (mod p)，同时 ad ≡ bc(mod p)

性质六：模因子特性
设d >= 1且 d | p，若 a ≡ b(mod p)
所以，一定存在 a ≡ b(mod d)

性质七：逆元特性
若 p >= 1，且gcd(a, p) == 1，则存在x使得ax ≡ 1(mod p)
x是a在模p的意义上的乘法逆元

裴蜀定理(Bézout's Identity)
裴蜀等式
ax + by = k, a, b, k为定值，x, y是方程的未知数
设d | gcd(a, b)，则方程ax + by = d 有整数解
	
证明(性质七)：
	逆元成立：ax≡1(mod p)
	ax % p = 1
	ax ÷ p = y …… 1
	py + 1 = ax
	ax - py = 1 <=> ax + py = 1
	根据裴蜀定理的存在性理论
	1 | gcd(a, p) == gcd(a, p) == 1

费马小定理(Fermat's Little Theorem))
当p是个质数，gcd(a, p) == 1，的情况下
a满足
a^(p-1) ≡ 1 (mod p)
a * a^(p-2) ≡ 1 (mod p)
a * x ≡ 1(mod p)
a^-1 ≡ a^(p-2) (mod p)
a的逆元等于a ^ (p - 2)
逆元优化，1/i
	
3x≡1 (mod 8)

扩展欧几里得算法(Extended Euclidean Algorithm)
我们需要求逆元，但是p不是个质数
欧几里得算法(Euclidean Algorithm) --> 辗转相除法 --> gcd(a, b)
	
long long gcd(long long a, long long b) {
	if (a % b == 0) return b;
	return gcd(b, a % b);
}
3x ≡ 1 (mod 8)
ax + py = 1
3x + 8y = 1
gcd(3, 8) 3x + 8y = 1  x = 3, y = -1
gcd(8, 3) 8x + 3y = 1  x = -1, y = 3
	8 - 2 * 3 = 2
gcd(3, 2) 3x + 2y = 1  x = 1, y = -1
gcd(2, 1) 2x + 1y = 1  x = 0, y = 1
gcd(1, 0) x = 1	,      x = 1, y = 0
	
x = y1
y = x1 - a / b * y1
当前这一层的x, y是否可以利用下一层的x1, y1来求得

