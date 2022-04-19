#include<iostream>
#include "map.h"
#include "color.h"

using namespace std;


Map::Map(vector<vector<int>> g,int s){
    this->grid=g;
    this->score=s;
}

int Map::getlen(int num){
    if (num==0) return 0;
    if (num<10) return 1;
    if (num<100) return 2;
    if (num<1000) return 3;
    if (num<10000) return 4;
    else return 5;
}

void Map::printmap(){
    for (int i=0;i<4;i++) {
        if (i==0) cout<<"\t\t\t\t\t┌───────┬───────┬───────┬───────┐"<<endl;
        else cout<<"\t\t\t\t\t├───────┼───────┼───────┼───────┤"<<endl;
        cout<<"\t\t\t\t\t│       │       │       │       │"<<endl;
        for (int j=0;j<4;j++){
            if (!j) cout<<"\t\t\t\t\t";
            cout<<"│";
            int len=getlen(grid[i][j]);
            int space1=(7-len)/2;
            for (int k=0;k<space1;k++) cout<<" ";
            if (grid[i][j]) {
                    if (i==x&&j==y) rgb_set(255,255,0);
                    cout<<grid[i][j];
                    if (i==x&&j==y) rgb_set(255,255,255);
            }
            for (int k=0;k<7-len-space1;k++) cout<<" ";
        }
        cout<<"│"<<endl;
        cout<<"\t\t\t\t\t│       │       │       │       │"<<endl;
    }
    cout<<"\t\t\t\t\t└───────┴───────┴───────┴───────┘"<<endl;
    cout<<"\t\t\t\t\t得分："<<score<<endl;
}

void Map::change(vector<vector<int>> g,int s){
    this->grid=g;
    this->score=s;
}

void Map::setnew(int a,int b){
    this->x=a;
    this->y=b;
}
