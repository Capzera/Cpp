#include "game.h"

using namespace std;

Game::Game(){
	raw=0;
	col=5;
	cnt=0;
	success=FALSE;
	failure=FALSE;
}

Game::~Game(){
	
}

void Game::Start(){
	for (;is_run();delay_fps(120)){
		char k=' ';
		if (kbhit()) k=getch();
		Play(k);
		G.showmap();
	}
}

void Game::Judge(int raw){
	int correct=0;
	set<char> s;
	string CurAnswer="";
	for (auto c:SUPERANSWER) s.insert(c);
	for (int i=raw*5;i<raw*5+5;i++) CurAnswer+=G.whattext(i);
	for (int i=0;i<col;i++) {
		if (CurAnswer[i]==SUPERANSWER[i]) {
			G.changestate(2,raw*this->col+i);
			correct++;
		}
		else if (s.find(CurAnswer[i])!=s.end()) G.changestate(3,raw*this->col+i);
		else G.changestate(1,raw*this->col+i);
	}
	if (correct==5) succ();
	if (raw==5) fail();
}

void Game::Play(char key){
	if (isalpha(key)&&(!cnt||cnt!=raw*5+5)){
		if (key>96) key-=32;
		G.changechar(key,cnt++);
	}
	if (key==8) {
		if (cnt>5*raw) cnt--;
		G.changechar(' ',cnt);
		return;
	}
	if (cnt&&cnt%5==0&&key==13) {
		raw++;
		Judge(cnt/5-1);
	}
}

void Game::succ(){
	
}

void Game::fail(){
	
}
