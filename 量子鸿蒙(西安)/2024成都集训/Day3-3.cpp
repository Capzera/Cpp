#include <bits/stdc++.h>
using namespace std;
int main() {
	动态数组的定义：
	vector<数据类型> 数组名; // 无长度定义
	vector<数据类型> 数组名(长度); // 带长无初值定义
	vector<数据类型> 数组名(长度，值); // 带长有初值定义
	vector<数据类型> 数组名(数组名); // 拷贝定义
	vector<int> a;//定义了一个数组，叫a，类型是整数类型
	vector<double> b(100); // b[0] -- b[99]
	vector<int> c(1000, 1);//定义一个数组c，int类型，长度1000，所有初始值均为1
	
	vector<int> z = {1, 5, 2, 4, 3}; // 创建数组z
	vector<int> d(z); // 拷贝z进d数组
	
	动态数组的访问：
	通过下标访问，动态数组创建时下标从0开始
	vector<int> a(1000); a[0] -- a[999];
	a[-1] a[1000]---> Running Error RE 运行时错误
	vector<int> a; // 没有任何空间
	a[0] 会导致RE。
	
	动态数组的遍历：
	数组名.size()获取数组长度
	int n = 数组名.size();
	for (int i = 0; i < n; i++) {
		// 适用于对循环变量i有操作的
	}
	/&在自动遍历中表示取地址，不需要额外开辟空间也能访问变量的值
	for (auto& x : 数组名) {
		x依次按照顺序替换为数组的每一个值
		适用于不太关心循环遍历i的情况使用
	}
	
	动态数组的内置函数：
	size_t size(); // 求动态数组的长度
	size_t length(); // 求动态数组的长度
	void push_back(typename x);
	// 向vector数组的末尾添加一个数x，导致数组长度变长
	void pop_back();
	// 删除vector的末尾的一个数，导致数组长度变短
	vector<typename>::iterator begin();
	// 获取vector的起始迭代器
	vector<typename>::iterator end();
	// 获取vector的终止迭代器
	void reverse();
	// 翻转整个或部分数组
	void iota();
	// 自动填充数组
	void sort();
	// 数组自动排序
	void resize(int x);
	void resize(int x, int val);
	// 重设数组长度
	
	会导致计算机出现RE（运行时错误，Running Error）
	除以0， 下标越界，访问异常空间
	return 0;
}
/*
STL 标准模板库
线性容器：string字符串 vector矢量 deque双端队列 List链表
关联容器：map哈希表 set集合 unordered_map无序哈希表 unordered_set无序集合
		  multiple_map多重哈希表 multiple_set多重集合
容器适配器：stack栈 queue队列 priority_queue优先级队列(堆) 


vector 矢量——有方向的量
动态数组
定义
vector<数据类型> 数组名
访问
遍历
内置函数
*/
