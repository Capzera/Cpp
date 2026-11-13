#include <bits/stdc++.h>
using namespace std;
int n, a[100010];
void Bubble_sort() {
	for (int i = 1; i < n; i++) {
		int flag = 1;
		for (int j = 1; j <= n - i; j++) {
			if (a[j] > a[j + 1]) {
				flag = 0;
				swap(a[j], a[j + 1]);
			}
		}
		if (flag) break;
	}
}
void Selection_sort() {
	for (int i = 1; i < n; i++) {
		int k = i;
		for (int j = i + 1; j <= n; j++) {
			if (a[j] < a[k]) {
				k = j;
			}
		}
		swap(a[i], a[k]);
	}
}
void Insertion_sort() {
	for (int i = 2; i <= n; i++) {
		int key = a[i], j = i - 1;
		while (j && a[j] > key) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
}

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	Insertion_sort();
	for (int i = 1; i <= n; i++) printf("%d ", a[i]);
	return 0;
}

将排序的数据分成两个区域：待排序区，已排序区，每次从待排序区抽一张牌
插入到已排序的正确位置上，最终完成排序



[0, 1, 2, 3, 5, 6, 7, 8, 9] [6 4]

for (int i = 2; i <= n; i++) {
	当前我挑出的a[i] 即将插到手牌a[1..i -1]
}

1. 定位
2. 右推
如果a[j] > key

//
//在第i轮选择数组中第i小的元素放在数组的第i个位置上
//
//1. 外层执行共计 n - 1轮，从1~n-1
//2. 数组下标范围[i..n] 找到最小值
//3. 使得 a[i] 成为擂主，让[i + 1..n] 分别去和擂主比较
//4. swap(a[i], a[k]);




//
//
//1. 对一组待排序的数据执行 n - 1 轮
//2. 第 i 轮从数组的下标1开始，执行到 n - i
//3. 如果发现a[i] > a[i + 1] 执行交换
//4. 直到n - 1轮结束，数据必然有序
