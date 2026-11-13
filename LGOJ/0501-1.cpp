#include <bits/stdc++.h>
using namespace std;
int f(int n) {
	if (n == 1) return 1;
	return f(n - 1) + n;
}
long long n;
int main() {
	cin >> n;
	cout << f(n);
	return 0;
}

n = 6

cout << f(6);



在递归中，之前未完成的函数被丢到栈
f(6) : return 15 + 6;










递（传递，从上往下）   归（回归，从下往上）
1. 递归  函数自己调自己
	代码极其简单，思路极其困难
2. 内层函数执行时，外层函数没结束


如何去设计一个递归的代码：
1. 状态量
	我在哪(Where)
2. 终止量
	我要去哪(What)
3. 过程量
	怎么做(How)

xxx(现实) -----> (第一层梦境) ------>  (第二层梦境)  ------> (第三层梦境)

int state[状态1][状态2]

int fun(状态量) {
	if (状态 == 终止情况) {
		终止量;
		return;
	}
	if (state[状态] != 初值) {
		return state[状态];
	}
	过程量;
	return state[状态] = 当前状态下的答案;
}


1. 外部设抽象函数

1 + 2 + ... + n
设 1-n 的和 f(n);
f(1) 
2. 求递推公式

1-n 的和  <---  1-(n-1)的和 + n
f(n) = f(n - 1) + n;


