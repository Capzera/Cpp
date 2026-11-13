#include "pw.h"

vector<passenger> pass;
vector<administrator> admin;
vector<flight> F;

using namespace std;

void flinfoin(){
	fstream f("fldb.txt",ios::in);
	while (!f.eof()){
		string id,company;
		vector<int> time(5);
		int price;
		string departure,destination;
		int rest;
		f>>id>>company>>time[0]>>time[1]>>time[2]>>time[3]>>time[4]>>price>>departure>>destination>>rest;
		flight tmp;
		tmp.infoin(id,company,time,price,departure,destination,rest);
		F.push_back(tmp);
	}
	f.close();
}

void painfoin(){
	fstream f("padb.txt",ios::in);
	while (!f.eof()){
		char sexin;
		string idin,namein,telin,pwdin,flin;
		passenger tmp;
		f>>idin>>namein>>sexin>>telin>>pwdin>>flin;
		tmp.infoin(idin,namein,sexin,telin,pwdin,flin);
		pass.push_back(tmp);
	}
}

void adlogin(int& x){
	system("cls");
	cout<<"请输入管理员编号"<<endl;
	string idin;
	cin>>idin;
	fstream f("addb.txt",ios::in);
	while (!f.eof()){
		administrator tmp;
		f>>tmp.id>>tmp.name>>tmp.tel>>tmp.pwd;//读入管理员信息
		admin.push_back(tmp);
	}
	int flag=0,i;
	int n=admin.size();
	for (i=0;i<n;i++){
		if (idin==admin[i].id) {
			flag=1;//检验管理员id是否正确
			break;
		}
	}
	if (flag){
		system("cls");
		string namein,pwdin;
		cout<<"用户名:";
		cin>>namein;
		cout<<"密码:";
		char ch=0;//记录键盘
		int in=0;//记录密码输入了几位
		while (1) {
			system("cls");
			Sleep(10);
			cout<<"用户名:"<<namein<<endl<<"密码:";
			for (int i=0;i<in;i++) cout<<"*";
			ch=getch();
			if (ch==13) break;
			if (ch==8){
				if (in){
					in--;
					pwdin.pop_back();
				}
			}
			else {
				in++;
				pwdin.push_back(ch);
			}
		}
		x=(namein==admin[i].name&&pwdin==admin[i].pwd);
		return;
	}
	else {
		system("cls");
		cout<<"未能找到此管理员";
		x=-1;
		getch();
	}
}

bool checkid(string s){
	if (s=="") return false;
	if (s.size()<18) return false;
	int flag=1;
	for (int i=0;i<17;i++){
		if (isdigit(s[i])) continue;
		flag=0;
		break;
	}
	return flag;
}

bool pwdcheck(string s){
	if (s=="") return false;
	if (s.size()<6){
		system("cls");
		cout<<"密码长度过短，请重新输入！";
		getch();
		return false;
	}
	return true;
}

bool pwdcheck(string s1,string s2){
	if (s1=="") return false;
	if (s1!=""&&s2==""){
		system("cls");
		cout<<"确认密码为空，请重新输入！";
		getch();
		return false;
	}
	if (s1!=s2){
		system("cls");
		cout<<"确认密码不符！";
		getch();
		return false;
	}
	return true;
}

void paregist(){
	fstream f("padb.txt",ios_base::app);
	string idin="";
	while (!checkid(idin)){
		system("cls");
		cout<<"请输入身份证号：";
		cin>>idin;
		if (!checkid(idin)){
			system("cls");
			cout<<"身份证格式错误，请重新输入!";
			getch();
		}
	}
	int n=pass.size();
	for (int i=0;i<n;i++){
		if (pass[i].getid()==idin){
			system("cls");
			cout<<"该用户已存在！"<<endl;
			getch();
			return;
		}
	}
	string namein="";
	system("cls");
	cout<<"请输入姓名：";
	cin>>namein;
	char sexin='Z';
	while (sexin!='F'&&sexin!='M'){
		system("cls");
		cout<<"请输入性别(M/F)：";
		cin>>sexin;
		if (sexin!='F'&&sexin!='M') {
			system("cls");
			cout<<"性别格式错误，请重新输入!";
			getch();
		}
	}
	string telin="";
	system("cls");
	cout<<"请输入电话号码：";
	cin>>telin;
	string pwdin,pwdc;
	while (1){
		pwdin=pwdc="";
		int in=0,flag=1;
		char g=' ';
		while (1){
			system("cls");
			cout<<"请输入密码：";
			for (int i=0;i<in;i++) cout<<"*";
			g=getch();
			if (g==13) break;
			if (g==8){
				if (in){
					in--;
					pwdin.pop_back();
				}
			}
			else {
				in++;
				pwdin.push_back(g);
			}
		}
		flag=pwdcheck(pwdin);
		if (!flag) continue;//密码不符合规范，重新输入
		in=0,flag=1;
		g=' ';
		while (1){
			system("cls");
			cout<<"请输入确认密码：";
			for (int i=0;i<in;i++) cout<<"*";
			g=getch();
			if (g==13) break;
			if (g==8){
				if (in){
					in--;
					pwdc.pop_back();
				}
			}
			else {
				in++;
				pwdc.push_back(g);
			}
		}
		flag=pwdcheck(pwdin,pwdc);
		if (flag) break;
	}
	passenger tmp;
	tmp.infoin(idin,namein,sexin,telin,pwdin,"*");
	f<<idin<<" "<<namein<<" "<<sexin<<" "<<telin<<" "<<pwdin<<" *"<<endl;
	system("cls");
	cout<<"注册成功！"<<endl;
	pass.push_back(tmp);
	getch();
	return;
	f.close();
}

int idlogin(int& x){
	string id="";
	while (!checkid(id)){
		system("cls");
		cout<<"请输入身份证号：";
		cin>>id;
		if (!checkid(id)){
			system("cls");
			cout<<"身份证格式错误，请重新输入!";
			getch();
		}
	}
	int n=pass.size();
	int index=-1;
	for (int i=0;i<n;i++){
		if (pass[i].getid()==id){
			index=i;
			break;
		}
	}
	if (index==-1){
		x=-1;
		return -1;
	}
	x=verification(index);
	return index;
}

int tellogin(int& x){
	string tel="";
	system("cls");
	cout<<"请输入手机号：";
	cin>>tel;
	int n=pass.size();
	int index=-1;
	for (int i=0;i<n;i++){
		if (pass[i].gettel()==tel){
			index=i;
			break;
		}
	}
	if (index==-1){
		x=-1;
		return -1;
	}
	x=verification(index);
	return index;
}

int verification(int index){
	string pwdin=passwordinput();
	return pass[index].getpwd()==pwdin;	
}

string passwordinput(){
	int in=0;
	char ch=' ';
	string pwdin="";
	while (1){
		system("cls");
		cout<<"请输入密码：";
		for (int i=0;i<in;i++) cout<<"*";
		ch=getch();
		if (ch==13) break;
		if (ch==8){
			if (in){
				in--;
				pwdin.pop_back();
			}
		}
		else {
			in++;
			pwdin.push_back(ch);
		}
	}
	return pwdin;
}
