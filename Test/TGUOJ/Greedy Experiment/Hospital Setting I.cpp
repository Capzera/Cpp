#include<bits/stdc++.h>
using namespace std;
class Graph{
public:
    Graph(int);
    void hospital();
private:
    int vnum;//µãÊý
    vector<vector<int>> edge;//±£´æ±ß
};

Graph::Graph(int n){
    int st,ed,wt;
    this->vnum=n;
    this->edge=vector<vector<int>> (n,vector<int>(n,100000));
    while (1){
        cin>>st>>ed>>wt;
        if (st+ed+wt==0) break;
        edge[st-1][ed-1]=min(edge[st-1][ed-1],wt);
        edge[ed-1][st-1]=min(edge[ed-1][st-1],wt);
    }
}
void Graph::hospital(){
    int i,j,k,s;
    for (k=0;k<vnum;k++) {
        for (i=0;i<vnum;i++) {
            for (j=0;j<vnum;j++) {
                edge[i][j]=min(edge[i][j],edge[i][k]+edge[k][j]);
            }
        }
    }
    int mindis=INT_MAX,dis=mindis;
    k=0;
    for (j=0;j<vnum;j++){
        dis=0;
        for (i=0;i<vnum;i++) {
            dis+=edge[i][j];
        }
        if (dis<mindis){
            mindis=dis;
            k=j;
        }
    }
    cout<<k+1<<endl;
}

int main(){
    int n;
    cin>>n;
    Graph g(n);
    g.hospital();
    system("pause");
    return 0;
}