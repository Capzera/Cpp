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
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            if (!grid[i][j])
                New.push_back({i,j});
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

bool game::Move(char ops){
    switch(ops) {
        case 'W':
            for (int j=0;j<4;j++) {
                vector<int> tmp(4,0);
                for (int i=0;i<4;i++)
                    tmp[i]=grid[i][j];
                for (int i=0;i<3;i++) {
                    if (!tmp[i]&&tmp[i+1]) return true;
                    if (tmp[i]==tmp[i+1]&&tmp[i]&&!index[i][j]) return true;
                }
            }
        break;
        case 'S':
            for (int j=0;j<4;j++) {
                vector<int> tmp(4,0);
                for (int i=0;i<4;i++)
                    tmp[i]=grid[i][j];
                for (int i=3;i>0;i--) {
                    if (!tmp[i]&&tmp[i-1]) return true;
                    if (!index[i][j]&&tmp[i]&&tmp[i]==tmp[i-1]) return true;
                }
            }
        break;
        case 'A':
            for (int i=0;i<4;i++) {
                vector<int> tmp(grid[i]);
                for (int j=0;j<3;j++) {
                    if (!tmp[j]&&tmp[j+1]) return true;
                    if (!index[i][j]&&tmp[j]&&tmp[j]==tmp[j+1]) return true;
                }
            }
        break;
        case 'D':
            for (int i=0;i<4;i++) {
                vector<int> tmp(grid[i]);
                for (int j=3;j>0;j--) {
                    if (!tmp[j]&&tmp[j-1]) return true;
                    if (!index[i][j]&&tmp[j]&&tmp[j]==tmp[j-1]) return true;
                }
            }
        break;
    }
    return false;
}

int game::use2(char ops){
    if (ops>96) ops-=32;
    int ans=0;
    index=vector<vector<bool>> (4,vector<bool>(4,false));
    switch (ops) {
        case 'W':
            while (Move('W')){
                for (int j=0;j<4;j++) {//扫描列
                    for (int i=0;i<3;i++) {
                        if (!index[i][j]&&grid[i][j]==grid[i+1][j]) {
                            index[i][j]=1;
                            grid[i][j]*=2;
                            ans+=grid[i][j];
                            grid[i+1][j]=0;
                        }
                        else if (!grid[i][j]&&grid[i+1][j]) {
                            swap(grid[i][j],grid[i+1][j]);
                            swap(index[i][j],index[i+1][j]);
                        }
                        else if (grid[i][j]&&grid[i+1][j]&&!index[i][j]&&grid[i][j]!=grid[i+1][j])
                            index[i][j]=1;
                    }
                }
                m.change(grid);
                m.printmap();
            }
        break;
        case 'S':
            while (Move('S')){
                for (int j=0;j<4;j++) {//扫描列
                    for (int i=3;i>0;i--) {
                        if (!index[i][j]&&grid[i][j]==grid[i-1][j]) {
                            index[i][j]=1;
                            grid[i][j]*=2;
                            ans+=grid[i][j];
                            grid[i-1][j]=0;
                        }
                        else if (!grid[i][j]&&grid[i-1][j]) {
                            swap(grid[i][j],grid[i-1][j]);
                            swap(index[i][j],index[i-1][j]);
                        }
                        else if (grid[i][j]&&grid[i-1][j]&&!index[i][j]&&grid[i][j]!=grid[i-1][j])
                            index[i][j]=1;
                    }
                }
                m.change(grid);
                m.printmap();
            }
        break;
        case 'A':
            while (Move('A')){
                for (int i=0;i<4;i++) {//扫描行
                    for (int j=0;j<3;j++) {
                        if (!index[i][j]&&grid[i][j]==grid[i][j+1]){
                            index[i][j]=1;
                            grid[i][j]*=2;
                            ans+=grid[i][j];
                            grid[i][j+1]=0;
                        }
                        else if (!grid[i][j]&&grid[i][j+1]) {
                            swap(grid[i][j],grid[i][j+1]);
                            swap(index[i][j],index[i][j+1]);
                        }
                        else if (grid[i][j]&&grid[i][j+1]&&!index[i][j]&&grid[i][j]!=grid[i][j+1])
                            index[i][j]=1;
                    }
                }
                m.change(grid);
                m.printmap();
            }
        break;
        case 'D':
            while (Move('D')){
                for (int i=0;i<4;i++) {//扫描行
                    for (int j=3;j>0;j--) {
                        if (!index[i][j]&&grid[i][j]==grid[i][j-1]){
                            index[i][j]=1;
                            grid[i][j]*=2;
                            ans+=grid[i][j];
                            grid[i][j-1]=0;
                        }
                        else if (!grid[i][j]&&grid[i][j-1]) {
                            swap(grid[i][j],grid[i][j-1]);
                            swap(index[i][j],index[i][j-1]);
                        }
                        else if (grid[i][j]&&grid[i][j-1]&&!index[i][j]&&grid[i][j]!=grid[i][j-1])
                            index[i][j]=1;
                    }
                }
                m.change(grid);
                m.printmap();
            }
        break;
        case 'T':
            vector<char> temp;
            for (auto x:operable)
                if (x<91) temp.push_back(x);
            Pos(37,19);
            cout<<"当前可以使用的操作是:";
            int i,n=temp.size();
            for (i=0;i<n-1;i++)
                cout<<"\'"<<temp[i]<<"\', ";
            cout<<"\'"<<temp.back()<<"\'"<<endl;
            system("pause");
            m.printmap();
        break;
    }
    return ans;
}

void game::play(int &x,char &opr){
    check();
    if (operable.empty()) {
        m.printmap();
        over();
        x=4;
        return;
    }
    opr='1';
    while (opr!='T'&&opr!='t'&&operable.find(opr)==operable.end()) {
        opr=getch();
    }
    int alter=use2(opr);
    m.change(grid,score+alter);
    score+=alter;
    Sleep(5);
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
