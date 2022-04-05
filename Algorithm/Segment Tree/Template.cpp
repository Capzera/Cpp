#include<bits/stdc++.h>
using namespace std;
class SegmentTree {
public:
	SegmentTree(vector<int>& nums) : n(nums.size()),Tree(nums.size()*4){
		build(0,0,n-1,nums);
	}

	void update(int index,int val) {
		change(index,val,0,0,n-1);
	}

	void sumRange(int l,int r){
		cout<<range(l,r,0,0,n-1)<<endl;
	}
private:
	vector<int> Tree;
	int n;
	void build(int node,int l,int r,vector<int> &nums){
		if (l==r) {
			Tree[node]=nums[l];
			return;
		}
		int mid=l+(r-l)/2;
		build(node*2+1,l,mid,nums);
		build(node*2+2,mid+1,r,nums);
		Tree[node]=Tree[node*2+1]+Tree[node*2+2];
	}

	void change(int index,int val,int node,int l,int r){
		if (l==r) {
			Tree[node]=val;
			return;
		}
		int mid=l+(r-l)/2;
		if (index<=mid) change(index,val,node*2+1,l,mid);
		else change(index,val,node*2+2,mid+1,r);
		Tree[node]=Tree[node*2+1]+Tree[node*2+2];
	}

	int range(int l,int r,int node,int left,int right) {
		if (l==left&&r==right) return Tree[node];
		int mid=left+(right-left)/2;
		if (r<=mid) return range(l,r,node*2+1,left,mid);
		else if (l>mid) return range(l,r,node*2+2,mid+1,right);
		else return range(l,mid,node*2+1,left,mid)+range(mid+1,r,node*2+2,mid+1,right);
	}
};
int main(){
	int n,q;
	cin>>n>>q;
	vector<int> nums(n);
	for (int i=0;i<n;i++) cin>>nums[i];
	SegmentTree T(nums);
	while (q--) {
		int c,x,y;
		cin>>c>>x>>y;
		if (c==1) T.update(x,y);
		else T.sumRange(x,y);
	}
	system("pause");
	return 0;
}