/*
给出一棵二叉树的中序与后序排列。求出它的先序排列。（约定树结点用不同的大写字母表示，且二叉树的节点个数 ≤8）
*/
#include <bits/stdc++.h>
using namespace std;

int main() {
    string mid, post;
    cin >> mid >> post;
    function<void(int, int, int, int)> dfs = [&](int l1, int l2, int r1, int r2) -> void {
        //先输出当前最后一个结点作为根节点
        cout << post[r2];
        //查询到根结点在中序遍历中的位置
        int m = mid.find(post[r2]);
        //dfs左子树
        if (m > l1) dfs(l1, m - 1, r1, r1 + m - 1 - l1);
        //dfs右子树
        if (m < l2) dfs(m + 1, l2, r1 + m - l1, r2 - 1);
    };
    dfs(0, mid.size() - 1, 0, post.size() - 1);
    system("pause");
    return 0;
}