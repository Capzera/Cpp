#include <bits/stdc++.h>
using namespace std;
int n, a[100010], tmp[100010];
void bubble_sort() { // 冒泡排序
	for (int i = 1; i < n; i++) { // 第i轮
		bool flag = 1;
		for (int j = 1; j <= n - i; j++) {
			if (a[j] > a[j + 1]) {
				swap(a[j], a[j + 1]);
				flag = 0;
			}
		}
		if (flag) break;
	}
}
void selection_sort() { // 选择排序
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
void insertion_sort() { // 插入排序 7 1 5 2 9 8 3 0 6 4
	for (int i = 2; i <= n; i++) {
		int key = a[i], j = i - 1;
		while (j > 0 && a[j] > key) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
}

void quick_sort(int l, int r) { // 快速排序 7 5 10 1 4 6 2 11 8 3 9
	if (l >= r) return; // 递归终止量
	int i = l, j = r, pivot = a[(l + r) / 2]; // 左右指针，基准值
	while (i <= j) { // 保证左右指针正确
		while (a[i] < pivot) i++; // 在左侧找到一个大于等于基准值的值
		while (a[j] > pivot) j--; // 在右侧找到一个小于等于基准值的值
		if (i <= j) swap(a[i++], a[j--]); // 如果指针合法，则交换，并为下一次找值做准备
	}
	quick_sort(l, j), quick_sort(i, r); // 根据基准值最终划分的左右区间，进行递归
}

void merge_sort(int l, int r) { // 归并排序
	if (l == r) return; // 只有一个元素时，不需要排序
	
	int mid = (l + r) / 2; // 中点，二分点
	merge_sort(l, mid); // 对左半边排序
	merge_sort(mid + 1, r); // 对右半边排序
	
	int i = l, j = mid + 1, k = 0; // 三指针，指向"三"个数组
	while (i <= mid && j <= r) {
		tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++]; // 把其中一个数组取完
	}
	while (i <= mid) tmp[k++] = a[i++]; // 如果左半边有剩余，直接承接
	while (j <= r) tmp[k++] = a[j++]; // 如果右半边有剩余，直接承接
	
	for (int i = l; i <= r; i++) { // 遍历原数组
		a[i] = tmp[i - l];
	}
}

void counting_sort() { // 计数排序
	
}
void bucket_sort() { // 桶排序
	
}

void shell_sort() { // 希尔排序
	
}
void heap_sort() { // 堆排序
	
}

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	merge_sort(1, n);
	for (int i = 1; i <= n; i++) cout << a[i] << " ";
	return 0;
}
/*
sort 归并 + 快速 + 堆
1. 当数据量小于等于16时，选用归并排序
2. 当数据量大于16时，首选快速排序。
3. 当快速排序的递归调用过深，转用堆排序
*/
