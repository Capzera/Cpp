#include <bits/stdc++.h>
using namespace std;
int main() {
    long long sum = 0;
    //*********
    int i;
    for (i = 1 ; i <= 100; i++) {
        sum += i * i;
    }
    //*********
    cout << "Sum = " << sum << endl;
    return 0;
}
/*
<求和>
1 + 2 + 3 + ... + 100
<奇数求和>
1 + 3 + 5 + 7 +... + 99
<平方求和>
求 1 + 4 + 9 + 16 + 25 + ... + 100 ^ 2
<水仙花数>
有这样的3位数(100 - 999),它的每一位的立方之和等于它自身
abc = a * a * a + b * b * b + c * c * c;
输出这样的3位数
数组(Array)
一组 相同类型 的值
类型 变量名[长度]
int nums[100];
nums[0] nums[1]
int i = 5;
nums[i]

操作符
int a = 5;
a++;
++a;



循环结构(Loop)
for (初值; 循环结束条件; 步长) {

}
求 1 + 2 + 3 + ... + 100

a = 6, b = 10
a += a + b;
a +=  a = a +



*/
