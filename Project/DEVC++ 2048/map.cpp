#include <iostream>
#include "map.h"
#include "color.h"

using namespace std;

Map::Map(vector<vector<int>> g,int s){
    this->grid=g;
    this->score=s;
}

int Map::getlen(int num){
    if (!num) return 0;
    if (num<10) return 1;
    if (num<100) return 2;
    if (num<1000) return 3;
    if (num<10000) return 4;
    else return 5;
}

void Map::printmap(){
    HideCursor();
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++){
            int len=getlen(grid[i][j]);
            int Space=(7-len)/2;
            Pos(8*j+41,4*i+2);
            for (int kk=0;kk<Space;kk++) cout<<" ";
            if (grid[i][j]) {
                    if (i==x&&j==y) rgb_set(255,255,0);
                    cout<<grid[i][j];
                    if (i==x&&j==y) rgb_set(255,255,255);
            }
            for (int kk=0;kk<7-Space-len;kk++) cout<<" ";
        }
    }
    Pos(46,17);
    cout<<score;
    Sleep(20);
}

void Map::change(vector<vector<int>> g,int s){
    this->grid=g;
    this->score=s;
}

void Map::change(vector<vector<int>> g){
    this->grid=g;
}

void Map::setnew(int a,int b){
    this->x=a;
    this->y=b;
}

void Map::drawmap(){
    HideCursor();
    system("cls");
    for (int i=0;i<4;i++) {
        if (i==0) cout<<"\t\t\t\t\t©°©¤©¤©¤©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©´"<<endl;
        else cout<<"\t\t\t\t\t©À©¤©¤©¤©¤©¤©¤©¤©à©¤©¤©¤©¤©¤©¤©¤©à©¤©¤©¤©¤©¤©¤©¤©à©¤©¤©¤©¤©¤©¤©¤©È"<<endl;
        cout<<"\t\t\t\t\t©¦       ©¦       ©¦       ©¦       ©¦"<<endl;
        for (int j=0;j<4;j++){
            if (!j) cout<<"\t\t\t\t\t";
            cout<<"©¦";
            for (int k=0;k<7;k++) cout<<" ";
        }
        cout<<"©¦"<<endl;
        cout<<"\t\t\t\t\t©¦       ©¦       ©¦       ©¦       ©¦"<<endl;
    }
    cout<<"\t\t\t\t\t©¸©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¼"<<endl;
    cout<<"\t\t\t\t\tµÃ·Ö£º"<<endl;
    Sleep(10);
}

