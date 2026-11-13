#include <bits/stdc++.h>
using namespace std;
char ans[50];
int main() {
	// 第一组选择题
	ans[1] = 'D';
	ans[2] = 'B';
	ans[3] = 'D';
	ans[4] = 'C';
	ans[5] = 'D';
	//第二组选择题
	ans[6] = 'A';
	ans[7] = 'D';
	ans[8] = 'D';
	ans[9] = 'A';
	ans[10] = 'C';
	//第三组选择题
	ans[11] = 'B';
	ans[12] = 'D';
	ans[13] = 'C';
	ans[14] = 'A';
	ans[15] = 'B';
	for (int i = 1; i <= 15; i++) cout << ans[i];
	return 0;
}
