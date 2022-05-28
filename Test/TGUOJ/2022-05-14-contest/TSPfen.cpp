#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> a;
vector<int> bestx,minout;
int n,edgenum,s,e,w,sum,bestc = INT_MAX;
class Node{
public:
    int t,cost,lcost;
    vector<int> x;
    Node(){x.resize(n);};//默认构造
    Node(int val,int n){//构造根节点
        t=cost=0;
        lcost=val;
        x.resize(n);
    }
    Node(Node node ,int i){//拷贝构造，根据父亲节点生成子节点
        t = node.t+1;
        x.resize(n);
        for(int i=0;i<n;i++) x[i] = node.x[i];
        swap(x[t], x[i]);
        cost = node.cost + a[x[t]][x[t-1]];
        lcost = cost;
        for(int i=t;i<n;i++) lcost += minout[i];
    }
    bool Judge(){ //约束函数和限界函数
        if(t==n-1){      //叶子节点
            if(a[x[t-1]][x[t]]!=INT_MAX && a[x[t]][x[0]]!=INT_MAX&&cost + a[x[t]][x[0]]< bestc) {
                cost += a[x[t]][x[0]];
                return true;
            }
        }
        else {           //非叶子节点
            if(a[x[t]][x[t-1]]!=INT_MAX)
                if(lcost < bestc) return true;
        }
        return false;
    }
    bool operator < (const Node &a) const {
        return lcost > a.lcost;
    }
};
priority_queue<Node> q;

void Branchbound(){//分支限界法
    //初始化，根节点入队
    Node a(sum,n);
    for(int i=0;i<n;i++) a.x[i]=i+1;
    q.push(a);
    while(!q.empty()){           //队列不为空的话
        for(int i = q.top().t+ 1;i<n;i++){
            Node node(q.top(),i);
            if(node.Judge()) q.push(node);
            else node.x.resize(0);
        }
        if(q.top().t==n-1){       //根节点位于优先队列头则直接更新最优值，结束循环
            bestc = q.top().cost;
            bestx = q.top().x;
            break;
        }
        q.pop();
    }
}
int main() {
    cin>>n>>edgenum;
    a=vector<vector<int>> (n+1,vector<int> (n+1,INT_MAX));
    //输入边和相应的权重
    for(int i=0;i<edgenum;i++){
        cin>>s>>e>>w;
        a[s][e] = w;
        a[e][s] = w;
    }
    //初始化minout数组
    minout=vector<int> (n);
    for(int i=1;i<=n;i++) {
        int minv = INT_MAX;
        for(int j=1;j<=n;j++) minv=min(minv,a[i][j]);//寻找当前结点的最小花费
        minout[i-1] = minv;
        sum += minv;
    }
    Branchbound();
    cout<<bestc<<endl;
    for(int i=0;i<n;i++)cout<<bestx[i]<<" ";
    system("pause");
    return 0;
}