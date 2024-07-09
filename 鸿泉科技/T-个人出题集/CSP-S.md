# CSP-S 题目

### 一.单项选择题（第 1 ~15 小题，每小题 2 分，共 30 分。下列每题给出的四个选项中，只有一个选项最符合试题要求）

1.以下哪个不属于线性结构（ ）

A.一维数组                   B. 二维数组                                       C.队列                                        D.栈

2.冒泡排序过程中，如果某一趟遍历后没有发生任何交换，说明什么（ ）。

A. 排序已经完成

C. 需要继续遍历

B. 排序出现错误

D. 无法确定排序状态

3.在$Linux$系统中，mkdir test指令的作用是（）

A.删除test文件夹里面的文件

B.删除test文件夹

C.创建test文件夹

D.复制test文件夹

4.若一颗二叉树的中序遍历序列为` DGBAECF`，后序遍历序列为 `GDBEFCA`，则前序遍历为（）。

A.`BDGACFE `                               B.`ACEBDGF`

C.`ABGDEFC    `                               D.`ABDGCEF`

5.假设一个二叉树拥有`2024`个结点，那么这个树至少有（）层。

A.8

B.9

C.10

D.11

6.数据序列`{2，1，4，9，8，10，6，20}`只能是（）进行两次排序的结果。

A. 插入排序

B. 冒泡排序

C. 选择排序

D. 快速排序

7.假设`1，2，3，4，5，6，7`为入栈顺序，则下列出栈顺序不对的是（）

A.`1 3 2 7 6 5 4 `                                B. `4 5 3 6 2 7 1`

C.`5 7 6 4 3 2 1   `                                D. `3 4 6 2 7 5 1`

8.下列地址中，属于子网`86.32.0.0/12`的地址是()

A.`86.33.224.123`                                B.`86.79.65.126`

C.`86.79.65.216`                                  D.`86.68.206.154`

9.总共有`5`个不同的元素进栈，能得到（）种不同的出栈序列。

A.    42           B.41           C.132.          D.130

10.有一个无向图如下所示，请你使用`Dijkstra`算法求从顶点`1`到其他顶点的最短路径，依次得到的各最短路径的目标点是（）

![graph (CSP-S(3).assets/graph (2).png)](graph (2).png)

 A.`1,3,4,2,5,6`                                                                     B.`1,4,2,3,5,6`

C.`1,2,3,5,4,6`                                                                      D.`1,3,2,4,5,6`

11 .现有一个`8GB`的U盘，最多能够存储（）张分辨率`1920\*1080`的`32`位图像。

A.`1034`        B.`1035 `         C.`1036`       D.`1037`   

12 

```
bool A(long long num)
{
	if(num==1)
	{
		return 0;
	}
	if(num==2||num==3)
	{
		return 1;
	}
	if(num%6!=1&&num%6!=5)
	return 0;
	for(long long i=5;i*i<=num;i+=6)
	{
		if(num%i==0||num%(i+2)==0)
		{
			return 0;
		}
	}
	return 1;
}
```

A.$O(logn)$                           B.$O(n)$                            C.$O(n^\frac{1}{2})$                          D.$O(N^2)$

13.假设某计算机使用八位补码进行计算, 现在有如下代码

```
int x = 10, y - 15;
int z = x - y;
```

当程序运行完成后，此时计算机存储`z` 的值为（ ）。

A.`1001 1111`

B.`1001 1000`

C.`1001 1001`

D.`0001 1010`

14.以下是一串 C++ 代码，请写出它编译运行后的结果（ ）。

```cpp
#include <iostream>
using namespace std;
int main() {
	int numOfBalls = 3;
	int numOfBoxes = 4;
	int totalWays = 1;
	for (int i = 0; i < numOfBalls; ++i) {
		totalWays \*= numOfBoxes;
	}
	cout « ” 总共有” « totalWays « ” 种不同的放法。” « endl;
	return 0;
}
```

A. 总共有 `48` 种不同的放法。

C. 总共有 `16` 种不同的放法。

B. 总共有 `64` 种不同的放法。

D. 总共有 `60` 种不同的放法。

15.设一批产品共有`10`件，其中`4`件残次品，现在从中不放回抽`3`次，则恰好有`2`件残次品的概率为（）

A.$\frac{3}{10}$                                 B.$\frac{36}{125}$                                       C.$\frac{1}{2}$                                D.$\frac{13}{25}$



### 二、阅读程序（程序输入不超过数组或字符串定义的范围；判断题正确填 √ ，错误填 × ；除特殊说明外，判断题 1.5 分，选择题 3 分，共计 40 分）

### (1)

```cpp
#include <iostream>
using namespace std;
const int N = 20;
int f[N][N];
int fun(int n, int m) {
	if (n < 2 || m < 2) return 1;
	if (f[n][m] != -1) return f[n][m];
	int res = 0;
	for (int i = 0; i < m; i += 2) {
		res += fun(n - 1, i);
	}
	f[n][m] = res;
	return res;
}
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			f[i][j] = -1;
		}
	}
	cout << fun(n, m);
	return 0;
}
```

#### 判断题

​	16. $fun$函数中， $m$的值不可能是奇数。（）

​	17. 若将第`9`行的$<$改为$<=$,程序的输出结果可能不会改变。（）

​	18. 若将第`7`行和第`12`行删除，程序运行的结果不变。（）

​	19. 添加合适头文件后，将`18`至`22`行替换成$memset(f, 255, sizeof(f))$,也可以起到同样的效果。

#### 选择题

​	20. 若输入的数据为4 8，则输出为（）。

A. 7   B. 8    C.15    D.16

​	21. 最坏情况下，此程序的时间复杂度是（）。

A. $O(m^2n)$ B. $O(nm!)$ C.$O(n^2 )$ D.$O(n^2m)$

#### 解析

```cpp
1)F 输入的m可以是奇数。
2)T 若输入的n,m 直接满足第6行，修正后的程序与源程序输出结果相同
3)T 这两行的程序是记忆化，它的作用是提高程序运行的效率。删去后不影响程序的结果。
4)T 考察memset函数，memset(f, 255, sizeof(f))实际上就是将f里每个元素都初始化成-1。
5)B f(4, 8) = f(3, 0) + f(3, 2) + f(3, 4) + f(3, 6) = 1 + 1 + 2 + 4 = 8。
6)A 最坏情况下。拆分一次，需要计算m/2次， 随后再去考虑m/2个算式，对于其每个算式继续拆分，
又产生m/4项式，一直拆分。最多拆分n-1次。所以最坏复杂度为O(m2n)
```







### (2)

```cpp
/*
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
代码块粘贴在此
*/
```



22. `5| 6`结果为`6`。(	)
23. $sizeof(f)$返回值为`4`。（）
24. `1 << 18`表示$2^{17}$。（）
25. 此程序的时间复杂度度为（）

​	A. $O(n)$		B.$O(2^n)$		C. $O(log2 n^2 )$		D.$O(n * 2^n)$

26. 若输入为

​		`4 10 `

​		`5  6  3  7 `

​	则输出为（）

​	A. 1		B.3		C. 5		D.6

27. 若输入为

​	`5 12`

`	11 12 36 56 8`

则输出为（）

A. 4		B.5		C .6		D.7





## （3）

```cpp
#include <iostream>
#include <queue>
using namespace std;
priority_queue<int, vector<int>> q1, q2;
int sum;
int main()  {
	int n, x, y;
	cin >> n >> y;
	q1.push(y);
	sum = y;
	for (int i = 2; i <= n; i++) {
		cin >> x;
		if (x > q1.top()) q2.push(-x);
		else q1.push(x);
		while (abs((int)q2.size() - (int)q1.size()) > 1) {
			if (q1.size() > q2.size()) 
				q2.push(-q1.top()), q1.pop();
 			else q1.push(-q2.top()), q2.pop();
		}
		if (q1.size() > q2.size() && i & 1)
			sum += q1.top();
		else if (q2.size() > q1.size() && i & 1) 
			sum -= q2.top();
	}
	cout << sum << endl;
	return 0;
}
```

 

#### 判断题

28. 删除第 23 行的强制类型转换，程序的输出保持不变（		）
29. 颠倒输入数组的顺序（即从n a1,a2,...,an变为n an,an-1,...,a1），程序的输出保持不变（		）
30. 当输入为7 18 6 -255 8 -666 -666 -888 时，输出为-239（		）

#### 单选题

31.当输入n个数，将输入数组中每个数的值+100，原输出与现输出的大小关系为（	）

​		A. 现输出比原输出大n * 100

​		B. 现输出比原输出大⌈n / 2⌉ * 100

​		C. 现输出比原输出大⌊n / 2⌋ * 100

​		D. 现输出比原输出大100

32.将第20行的 > 换成 >=，原输出与现输出的大小关系为（		）

​	A. 一定不变

​	B. 一定小于等于且不一定小于

​	C. 一定大于等于且不一定大于

​	D. 以上三种情况都不对

33. 当输入为10001个数，第一个数是10000，后10000个数是从10000到1，程序的输出为（		）

​	A. 25005000 		B. 37500000 		C. 37502500 		D. 37507500

 

#### 解析：

```
代码是利用对顶堆对输入的前奇数项输出中位数。把小顶堆换成了负数形式的大顶堆。
1. ×。size_type无法利用abs函数，程序无法输出。需要强制类型转换。
2. ×。奇数项不变，偶数项不一定。
3. ×。中位数分别为18,6,6,-255，加和为-225
4. B. 奇数项，上取整。
5. A. 对顶堆不受影响。
6. C. 10000+9999+...+5001=37502500
```



### 三、完善程序（单选题，每小题 3 分，共计 30 分）

#### （1）**(奇数区间)**给定一个长度为 `n` 的数列：$a_1,a_2,⋯a_n$，如果其中一段连续的子序列 $a_i,a_{i+1},⋯a_j(i≤j)$中，奇数比偶数多，我们就称这个区间`[i,j]` 是奇数区间。求给定的`n`个数中有多少个奇数区间。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int off = 1e6 + 1;
const int maxn = 1e6 + 5;
int a[maxn], s[maxn], c[2 * maxn];
int n; 
long long ans;
int lowbit(int x){
	return x & -x;
}
long long getSum(int x){
	long long res = 0;
	while(x > 0){
		res += c[x];
		①     ;
	}
	return res;
}
void add(int x, int k){
	while(     ②     ){
		c[x] += k;
		③     ;
	}
}
int main(){
	cin >> n;
	add(off, 1);
	for(int i = 1; i <= n; i++){
		cin >> a[i];
		s[i] =      ④     ;
		ans += getSum(     ⑤     );
		add(s[i] + off,1);
	}
	cout << ans << endl; 
	return 0;
}
```

34. ①处应该填（ A ）

A. x -= lowbit(x)

B. x = lowbit(x)

C. x += lowbit(x)

D. x--

 

35. ②处应该填（ D ）

A. $x < off$

B. $x <= n$

C. $x != 0$

D.$x <= 2 * off$

 

36. ③处应该填（ C ）

A. x -= lowbit(x)

B. x = lowbit(x)

C. x += lowbit(x)

D. x++$

 

37. ④处应该填（ C ）

A. $s[i - 1] + a[i] % 2 ? 1 : -1$

B. $s[i - 1] + a[i] % 2 == 1 ? -1 : 1$

C. $s[i - 1] + a[i] % 2 != 0 ? 1 : -1$

D. $s[i - 1] + !a[i] % 2 ? 1 : -1$

 

38. ⑤处应该填（ B ）

A. $0$

B. $s[i] + off - 1$

C. $s[i - 1]$

D. $s[i] + off$

 

#### （2）**（最近公共祖先）**已知一棵有根的多叉树，每次询问下求给定的两个点的最近公共祖先。其中输入的第一行`3`个整数为结点个数，询问次数，根节点序号。最近公共祖先：在一棵有根树中，一个结点的祖先结点是指它本身或者它父节点的祖先。对于任意两个结点的公共祖先中距离两者最近的结点。

```cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 500010;
vector<int> edge[MAXN];
int depth[MAXN], lg[MAXN];
int pa[MAXN][22];
int n, m, r, x, y;
void dfs(int fa, int sn, int dep) {
	depth[sn] = dep;
	pa[sn][0] = fa;
	for(int i = 1;      ①     ; i++) {
		pa[sn][i] = pa[pa[sn][i - 1]][i - 1];
	}
	for(int i = 0; i < edge[sn].size(); i++) {
		int fn = edge[sn][i];
		if (fn == fa) {
			continue;
		}
		dfs(     ②     );
	}
}
int solve(int x, int y) {
	if (depth[x] < depth[y]) {
		swap(x, y);	
	}
	while(depth[x] > depth[y]) {
		x =      ③     ;
	} 
	if(x == y) {
		return x;	
	}
	for(int k = lg[depth[x]] - 1; k >= 0; --k) {
		if(     ④     ) {
			x = pa[x][k], y = pa[y][k];
		}
	}  
	return pa[x][0];
}
int main() {
	cin >> n >> m >> r;
	for(int i = 1; i <= n; i++)
		lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
	for (int i = 0; i < n - 1; i++) {
		cin >> x >> y;
		edge[x].push_back(y);
		edge[y].push_back(x);
	}
	dfs(     ⑤     );
	while (m--) {
		cin >> x >> y;
		cout << solve(x, y) << endl;
	}
	return 0;
}
```

39. ①处应该填（ D ）

A. $i < pa[sn].size()$

B.$ i <= lg[depth[fn]$

C. $i < lg[dep]$

D.$ i <= lg[depth[sn]]$

 

40. ②处应该填（ A ）

A. $sn, fn, dep + 1$

B. $fn, sn, dep + 1$

C. $sn, fn, dep$

D.$ fn, sn, dep$

 

41. ③处应该填（ B ）

A. $lg[depth[x] - depth[y]]$

B. $pa[x][lg[depth[x] - depth[y]] - 1]$

C. $pa[y][lg[depth[x]$

D. $pa[y][lg[depth[y] - depth[x]]$

 

42. ④处应该填（ C ）

A. $pa[k][y] == pa[k][x]$

B. $!pa[x$][k]

C. $pa[x][k] != pa[y$][k]

D. $pa[x][k] == pa[y][k]$

 

43. ⑤处应该填（ B ）

A. $r, 0, 0$

B. $0, r, 0$

C. $r, 0, 1$

D. $0, r, 1$