#include<bits/stdc++.h>
using namespace std;
class Graph{
public:
    Graph(int);
    bool findver(int&);
    bool insedge(int,int,int);
    void hospital();
private:
    int vnum;//点数
    int eNum;//边数
    vector<int> vertex;//保存顶点
    vector<vector<int>> edge;//保存边
    bool getverpoint(int&,int&);//判断顶点是否出现
};

bool Graph::getverpoint(int& vertex,int& i){
        for (i=0;i<vnum;i++) {
            if (vertex==this->vertex[i]) return true;
        }
        return false;
}

Graph::Graph(int n){
    int e;
    int st,ed,wt;
    this->vnum=n;
    this->vertex=vector<int>(n);
    this->eNum=0;
    for (int i=0;i<n;i++) {
        vertex[i]=i+1;
    }
    this->edge=vector<vector<int>> (n,vector<int>(n));
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++) {
            if (i!=j) edge[i][j]=100000;
        }
    }
    while (1){
        cin>>st>>ed>>wt;
        if (st+ed+wt==0) break;
        insedge(st,ed,wt);
    }
}

bool Graph::findver(int& vertex){
    for (int i=0;i<vnum;i++) {
        if (vertex==this->vertex[i]) return true;
    }
    return false;
}

bool Graph::insedge(int st,int ed,int wt){
    int j1=0,j2=0;
    if (getverpoint(st,j1)&&getverpoint(ed,j2)){
        edge[j1][j2]=edge[j2][j1]=wt;
        this->eNum+=2;
        return true;
    }
    return false;
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