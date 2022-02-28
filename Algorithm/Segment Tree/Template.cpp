#include<bits/stdc++.h>
#define MAX 10000
using namespace std;
int a[MAX],ans[MAX<<2],lazy[MAX<<2],n;
void pushup(int rt){
	ans[rt] = ans[rt<<1] + ans[rt<<1|1];	//查询区间和
	//ans[rt] = max(ans[rt<<1],ans[rt<<1|1]);	查询区间最值
}
// build（1，n, 1)
void build(int l,int r,int rt){
	if(l == r){
		ans[rt] = a[l]; //或初始化为0
		return;
	}
	int mid = (l + r) >> 1;
	build(l, mid, rt<<1);	//左子树递归
	build(mid+1, r, rt<<1|1);	//右子树递归
	pushup(rt);
}

void pushdown(int rt, int l){
	if(lazy[rt]){
		lazy[rt << 1]+=lazy[rt];
		lazy[rt << 1|1]+=lazy[rt];
		ans[rt << 1]+=lazy[rt]*(l-l/2);
		ans[rt << 1|1]+=lazy[rt]*(l/2);
		lazy[rt]=0;
	}
}

void add(int L,int R,int l,int C,int rt){
	if(L==R)
    {
		ans[rt]=C;
		return;
	}
	int mid=(L+R)>>1;
	pushdown(rt,R-L+1);
	if(l <= mid) add(L,mid,l,C,rt<<1);
	else add(mid+1,R,l,C,rt<<1|1);
	pushup(rt);
}

void update(int L,int R,int C, int l, int r, int rt){
	if(L>=l&&R<=r)
    {
		ans[rt]=C*(R-L+1);
		lazy[rt]=C;
		return;
	}
	int mid = (L+R) >> 1;
	pushdown(rt, R-L+1);
	if(r<=mid) update(L, mid, C, l, r, rt<<1);
	else if(r>mid) update(mid+1, R, C, l, r, rt<<1|1);
	else
    {
		update(L, mid, C, l, r, rt<<1);
		update(mid+1, R, C ,l, r, rt<<1|1);
	}
	pushup(rt);
}

int query(int L, int R, int l, int r, int rt){
	if(L >= l && R <= r)
	return ans[rt];
	int mid = (L+R)>>1;
	pushdown(rt, R-L+1);
	if(r <= mid)
	return query(L, mid, l, r, rt<<1);
	else if(l > mid)
	return query(mid+1, R, l, r, rt<<1|1);
	else return query(L,mid,l,mid,rt<<1)+query(mid+1,R,mid+1,r,rt<<1|1);
	//查询区间最值
	//return max(query(L, mid, l, mid, rt<<1),query(mid+1, R, mid+1, r, rt<<1|1));
}
int main()
{
    system("pause");
	return 0;
}