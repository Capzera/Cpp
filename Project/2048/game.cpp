#include "game.h"
#include "time.h"
#include "windows.h"
#include<conio.h>

using namespace std;

Map m;

game::game(){
    this->grid=vector<vector<int>> (4,vector<int>(4,0));
    srand(time(NULL));
    culempty();
    this->score=0;
}

void game::culempty(){
    New.clear();
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++) {
            if (!grid[i][j])
                New.push_back({i,j});
        }
    }
}

void game::NewNumber(){
    culempty();
    if (New.empty()) return;
    int index=rand()%int(New.size());
    int what=rand()%10;
    x=New[index].first;
    y=New[index].second;
    if (!what) grid[x][y]=4;
    else grid[x][y]=2;
    m.setnew(x,y);
    m.change(grid,score);
    m.printmap();
}

int game::use(char ops) {
    if (ops>96) ops-=32;
    int ans=0;
    switch(ops) {
        case 'W':
            for (int i=0;i<4;i++) {
                vector<int> d(4,0);
                int k=0;
                for (int j=0;j<4;j++)
                    if (grid[j][i])
                        d[k++]=grid[j][i];
                if (d[0]==d[1]&&d[1]) {
                    d[0]*=2;
                    ans+=d[0];
                    d[1]=0;
                    if (d[2]==d[3]&&d[3]) {
                        d[1]=d[2]*2;
                        ans+=d[1];
                        d[2]=d[3]=0;
                    }
                    else {
                        d[1]=d[2];
                        d[2]=d[3];
                    }
                }
                else if (d[1]==d[2]&&d[2]) {
                    d[1]*=2;
                    ans+=d[1];
                    d[2]=0;
                    if (d[3]) swap(d[2],d[3]);
                }
                else if (d[2]==d[3]&&d[2]){
                    d[2]*=2;
                    ans+=d[2];
                    d[3]=0;
                }
                for (int j=0;j<4;j++) grid[j][i]=d[j];
            }
        break;
        case 'A':
            for (int i=0;i<4;i++) {
                vector<int> d(4,0);
                int k=0;
                for (int j=0;j<4;j++)
                    if (grid[i][j])
                        d[k++]=grid[i][j];
                if (d[0]==d[1]&&d[1]) {
                    d[0]*=2;
                    ans+=d[0];
                    d[1]=0;
                    if (d[2]==d[3]&&d[3]) {
                        d[1]=d[2]*2;
                        ans+=d[1];
                        d[2]=d[3]=0;
                    }
                    else {
                        d[1]=d[2];
                        d[2]=d[3];
                    }
                }
                else if (d[1]==d[2]&&d[2]) {
                    d[1]*=2;
                    ans+=d[1];
                    d[2]=0;
                    if (d[3]) swap(d[2],d[3]);
                }
                else if (d[2]==d[3]&&d[2]){
                    d[2]*=2;
                    ans+=d[2];
                    d[3]=0;
                }
                for (int j=0;j<4;j++) grid[i][j]=d[j];
            }
        break;
        case 'S':
            for (int i=0;i<4;i++) {
                vector<int> d(4,0);
                int k=3;
                for (int j=3;j>=0;j--)
                    if (grid[j][i])
                        d[k--]=grid[j][i];
                if (d[3]==d[2]&&d[2]) {
                    d[3]*=2;
                    ans+=d[3];
                    d[2]=0;
                    if (d[1]==d[0]&&d[0]) {
                        d[2]=d[1]*2;
                        ans+=d[2];
                        d[1]=d[0]=0;
                    }
                    else {
                        d[2]=d[1];
                        d[1]=d[0];
                    }
                }
                else if (d[1]==d[2]&&d[1]) {
                    d[2]*=2;
                    ans+=d[2];
                    d[1]=0;
                    if (d[0]) swap(d[1],d[0]);
                }
                else if (d[1]==d[0]&&d[0]){
                    d[1]*=2;
                    ans+=d[1];
                    d[0]=0;
                }
                for (int j=0;j<4;j++) grid[j][i]=d[j];
            }
        break;
        case 'D':
            for (int i=0;i<4;i++) {
                vector<int> d(4,0);
                int k=3;
                for (int j=3;j>=0;j--)
                    if (grid[i][j])
                        d[k--]=grid[i][j];
                if (d[3]==d[2]&&d[2]) {
                    d[3]*=2;
                    ans+=d[3];
                    d[2]=0;
                    if (d[1]==d[0]&&d[0]) {
                        d[2]=d[1]*2;
                        ans+=d[2];
                        d[1]=d[0]=0;
                    }
                    else {
                        d[2]=d[1];
                        d[1]=d[0];
                    }
                }
                else if (d[1]==d[2]&&d[1]) {
                    d[2]*=2;
                    ans+=d[2];
                    d[1]=0;
                    if (d[0]) swap(d[1],d[0]);
                }
                else if (d[1]==d[0]&&d[0]){
                    d[1]*=2;
                    ans+=d[1];
                    d[0]=0;
                }
                for (int j=0;j<4;j++) grid[i][j]=d[j];
            }
        break;
    }
    return ans;
}

void game::play(int &x){
    check();
    if (operable.empty()) {
        system("cls");
        m.printmap();
        over();
        x=3;
        return;
    }
    char c='1';
    while (operable.find(c)==operable.end()) {
        c=getch();
    }
    int alter=use(c);
    m.change(grid,score+alter);
    score+=alter;
    Sleep(50);
    system("cls");
    m.printmap();
}

void game::check(){
    this->operable.clear();
    bool U=0,D=0,L=0,R=0;
    for (int i=0;i<4;i++) {
        for (int j=0;j<3;j++) {
            if (!grid[j][i]&&grid[j+1][i]) U=1;//上有0位
            if (!grid[j+1][i]&&grid[j][i]) D=1;//下有0位
            if (!grid[i][j]&&grid[i][j+1]) L=1;//左有0位
            if (!grid[i][j+1]&&grid[i][j]) R=1;//右有0位
            if (grid[j][i]==grid[j+1][i]&&grid[j+1][i])//同列相等
                U=D=1;
            if (grid[i][j]==grid[i][j+1]&&grid[i][j+1])//同行相等
                L=R=1;
            if (U) {
                operable.insert('W');
                operable.insert('w');
            }
            if (D) {
                operable.insert('S');
                operable.insert('s');
            }
            if (L) {
                operable.insert('A');
                operable.insert('a');
            }
            if (R) {
                operable.insert('D');
                operable.insert('d');
            }
        }
    }
}

void game::over(){
    Pos(48, 8);    cout << "无路可走，游戏结束!\n\n\n\n\n\n\n\n\n\n\n\n";
}
