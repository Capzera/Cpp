#include<bits/stdc++.h>
using namespace std;

int n,c;
int w[101],v[101],bestx[101];
int bestv=0,total=1;
struct node        //队列中的结点类型
{
    int no;            //结点编号，从1开始 
    int i;            //当前结点在搜索空间中的层次 
    int w;            //当前结点的总重量 
    int v;            //当前结点的总价值 
    int x[101];    //当前结点包含的解向量 
    double ub;        //上界 
};

void bound(node &e)        //计算分支结点e的上界 
{
    int i=e.i+1;        //考虑结点e的余下物品
    int sumw=e.w;
    double sumv=e.v;
    while((sumw+w[i]<=c)&&i<=n) 
    {
        sumw+=w[i];
        sumv+=v[i];
        i++;
    }
    if(i<=n)            //余下物品只能部分装入 
    e.ub=sumv+(c-sumw)*v[i]/w[i];
    else e.ub=sumv; 
} 

void enqueue(node e,queue<node> &q)
//结点e进队qu 
{
    if(e.i==n)                //到达叶子节点，不在扩展对应一个解 
    {
        if(e.v>bestv)        //找到更大价值的解 
        {
            bestv=e.v;
            for(int j=1;j<=n;j++)
            bestx[j]=e.x[j];
        }
    }
    else q.push(e);        //非叶子结点进队
} 

void bfs()
{
    int j;
    node e,e1,e2;
    queue<node> q;
    e.i=0;
    e.w=0;
    e.v=0;
    e.no=total++; 
    for(j=1;j<=n;j++)
    e.x[j]=0;
    bound(e);
    q.push(e);   
    while(!q.empty())
    {
        e=q.front();q.pop();    //出队结点e 
        if(e.w+w[e.i+1]<=c)        //剪枝，检查左孩子结点 
        {
            e1.no=total++;        //建立左孩子结点 
            e1.i=e.i+1;
            e1.w=e.w+w[e1.i];
            e1.v=e.v+v[e1.i];
            for(j=1;j<=n;j++)
            e1.x[j]=e.x[j];
            e1.x[e1.i]=1;
            bound(e1);        //求左孩子的上界 
            enqueue(e1,q);    //左孩子结点进队 
        }
        e2.no=total++;
        e2.i=e.i+1;
        e2.w=e.w;
        e2.v=e.v; 
        for(j=1;j<=n;j++)
            e2.x[j]=e.x[j];
        e2.x[e2.i]=0;
        bound(e2);
        if(e2.ub>bestv)        //若右孩子结点可行，则进队，否则被剪枝 
        enqueue(e2,q);    
    }
} 

int main() {
    cout<<"请输入物品的个数："<<endl;
    cin>>n;
    cout<<"请输入每个物品的重量及价值:"<<endl;
    for(int i = 1; i <= n; i++) cin>>w[i]>>v[i];
    cout<<"请输入背包的容量："<<endl;
    cin>>c;
    bfs();
    cout<<"最优值是:"<<bestv<<endl;
    for(int i=1;i<=n;i++) cout<<bestx[i]<<" ";
    system("pause");
    return 0;
} 