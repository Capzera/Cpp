#include <bits/stdc++.h>
using namespace std;

vector<int> Tree;
int n;

void build(int node,int l,int r,vector<int> &nums) {
		if (l==r) {
			Tree[node]=nums[l];
			return;
		}
		int mid=l+(r-l)/2;
		build(node*2+1,l,mid,nums);
		build(node*2+2,mid+1,r,nums);
		Tree[node]=Tree[node*2+1]+Tree[node*2+2];
	}

int range(int l,int r,int node,int left,int right) {
		if (l==left&&r==right) return Tree[node];
		int mid=left+(right-left)/2;
		if (r<=mid) return range(l,r,node*2+1,left,mid);
		else if (l>mid) return range(l,r,node*2+2,mid+1,right);
		else return range(l,mid,node*2+1,left,mid)+range(mid+1,r,node*2+2,mid+1,right);
	}
int main() {
    cin >> n;
    vector<int> nums(n * 2 - 1);
    vector<vector<int> > dp(n * 2 - 1, vector<int>(n * 2 - 1));
    for (int i = 0; i < n; i++) cin >> nums[i];
    for (int i = n; i < 2 * n - 1; i++) nums[i] = nums[i - n];
    Tree.resize(n * 8);
    build(0, 0, 2 * n - 1, nums);
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i < 2 * n - len; i++) {
            int j = i + len - 1;
            for (int k = i; k < j; k++) {
                int value = range(i, j, 0, 0, 2 * n - 1);
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j] + value);
            }
        }
    }
    cout << max(dp[0][n - 1], dp[n - 1][2 * n - 1]);
    system("pause");
    return 0;
}
/*
4 5 9 4
4 9 18 22
*/