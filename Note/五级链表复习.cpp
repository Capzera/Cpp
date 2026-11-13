链表->数据结构
数组 -> 查改
增 删 查 改
链表-> 增删
1. 链表如何定义
2. 链表怎么使用
3. 链表有什么特殊的操作

ListNode {
	int val; // val表示一个点自身要存什么值
	ListNode *prev = nullptr, *next = nullptr// 存上一个或者下一个点的位置
	// 不初始化
	ListNode() {}
	// 初始化值，创建一个点的时候自带一个值
	ListNode(int _v) : val(_v) {}
	// 初始化指针
	ListNode(ListNode *_n) : val(0), next(_n) {}
	// 既初始化值，又初始化指针
	ListNode(int _v, ListNode *_p, ListNode *_n) : val(_v), prev(_p), next(_n) {}
}

头指针，头结点
头结点，指向链表第一个数据结点的结点
头指针，指向头结点的指针
尾结点

ListNode *dummy = new ListNode(-1, nullptr, nullptr);
// 我要向链表中插入一个值为t 的结点
ListNode *p = new ListNode(t)
p->next = dummy->next;
dummy->next->prev = p;
dummy->next = p;
p->prev = dummy;


ListNode *q = new ListNode(t);
q->prev = tail->prev;
tail->prev->next = q;
q->next = tail;
tail->prev = q;

if (dummy->next) dummy->next = dummy->next->next;
if (tail->prev) tail->prev = tail->prev->prev;


请删除链表中第k个结点，保证k不超过链表的长度
链表能不能随机访问
ListNode *p = dummy;
int k;
cin >> k;
想要删除第k个点需要定位到第k - 1 个点
ListNode *p = dummy; // 设置循环初值
k--; // 先让k - 1，让我们找到第k - 1号点
while (p) { // 遍历，把p定位到k - 1号
	p = p->next, k--;
}
// k - 1号的下一个为k + 1号，相当于k在链表中被跳过了
p->next = p->next->next


ListNode *p = new ListNode;
p->val = t;
p->next = nullptr;
p->prev = nullptr;


ListNode *p = new ListNode(t, nullptr, nullptr);

void qsort(int l, int r) {
	if (l >= r) return;
	int i = l, j = r, pivot = a[l + r >> 1]; // i,j双指针，pivot是基准值，随便取
	while (i <= j) { // 保证双指针的首尾性质
		while (a[j] >= pivot) j--; // 基准值右侧找到小于基准值的值
		while (a[i] <= pivot) i++; // 基准值左侧找到大于基准值的值
		if (i <= j) swap(a[i++], a[j--]); // 只要指针没炸，就把他们交换
	}
	qsort(l, j), qsort(i, r); // j跑到左边多一点，i跑到右边多一点[l, j], [i, r];
}
看坑
有没有双指针
双指针内循环不计
for (int i = 1, j = 1; i <= n; i++) {
	while (j <= n && a[j] >= V) {
		j++;
	}
	if (j > n) break;
	ans++;
}
for嵌套，复杂度相乘
for并列， 复杂度相加
代5进去

logn
7 21
6 = 13
5 = 8
4 = 3 + 2
3 = 2 + 1
2 = 1 + 0
