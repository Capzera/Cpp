#include "game.h"

using namespace std;

void init(){
	srand(time(NULL));
	fstream f("Words\\dictionary.txt");
	vector<string> data;
	string tmp;
	while (getline(f,tmp)){
		data.push_back(tmp);
	}
	int n=data.size();
	int ran=rand()%n;
	a=data[ran];
	fstream file;
	file.open("Words\\Cache.txt",ios::out);
	file<<a<<endl;
	file<<GetDpiForWindow(hd)<<endl;
	file<<ZOOM<<endl;
	file.close();
}

void Game::Init(){
	init();
	raw=0;
	col=5;
	cnt=0;
	success=FALSE;
	failure=FALSE;
	for (int i=0;i<5;i++) Answer+=(a[i]-32);
}

void Game::Start(){
	for (;is_run();delay_fps(120)){
		G.showmap();
		if (success) {
			succ();
			break;
		}
		if (failure) {
			fail();
			break;
		}
		char k=' ';
		if (kbhit()) k=getch();
		if (k==27) return;
		Play(k);
	}
}

void Game::Judge(int raw){//answer=sabin  current=chess
	int correct=0;
	unordered_map<char,int> um;
	set<int> corr;
	string CurAnswer="";
	for (int i=raw*5;i<raw*5+5;i++) CurAnswer+=G.whattext(i);//获取当前输入的答案
	for (int i=0;i<col;i++) {
		if (CurAnswer[i]==Answer[i]) {//如果同字同位
			G.changestate(2,raw*this->col+i);//改成绿色
			correct++;//标准答案数目+1
			corr.insert(i);//保存正确答案位置
		}
		else {
			um[Answer[i]]=i;//将错误的字母放入哈希表
		}
	}
	for (int i=0;i<col;i++) {
		if (corr.find(i)!=corr.end()) continue;//当前位置被标记为正确则跳过
		if (um.find(CurAnswer[i])!=um.end()) G.changestate(3,raw*this->col+i);//
		else G.changestate(1,raw*this->col+i);
	}
	if (correct==5) success=TRUE;
	if (raw==5) failure=TRUE;
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
	if (cnt&&cnt==raw*5+5&&key==13) {
		raw++;
		Judge(cnt/5-1);
	}
}

void Game::succ(){
	setfont(60, 0, "楷体");
	setcolor(RED);
	outtextxy((SCREENWIDTH-textwidth("恭喜你，答对了！"))/2,(SCREENHEIGHT-textheight("恭喜你，答对了！"))/2+250,"恭喜你，答对了！");
	getch();
}

void Game::fail(){
	char ans[6];
	ans[5]='\0';
	for (int i=0;i<5;i++) ans[i]=Answer[i];
	setfont(60, 0, "楷体");
	setfillcolor(RGB(0xEC,0xD8,0x20));
	bar(480/ZOOM,378/ZOOM,1440/ZOOM,702/ZOOM);
	setcolor(RED);
	outtextxy((SCREENWIDTH-textwidth("答错了，这个单词是：     "))/2,(SCREENHEIGHT-textheight("答错了，这个单词是：     "))/2,"答错了，这个单词是：     ");
	setcolor(YELLOW);
	outtextxy(1170/ZOOM,512/ZOOM,ans);
	getch();
}
