#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include "common.h"
#include "menu.h"
#include "pw.h"
#include "passenger.h"

vector<menu> m(15);
static int adminlogin;
static int passlogin;
static int curpass;

void init(){//菜单、数据初始化
	m[1].namein("menu1.txt");
	m[2].namein("menu2.txt");
	m[3].namein("menu3.txt");
	m[4].namein("menu4.txt");
	m[5].namein("menu5.txt");
	m[6].namein("menu6.txt");
	m[7].namein("menu7.txt");
	m[8].namein("menu8.txt");
	m[9].namein("menu9.txt");
	painfoin();
	flinfoin();
}

void over(){//退出
	system("cls");
	cout<<"感谢使用本管理系统，再见！"<<endl;
}

void fllistprint(int index){//航班表打印 0标题，1标题+列表，>1标题+筛选
	cout.width(7);
	cout<<std::left<<"航班号";
	cout.width(17);
	cout<<std::left<<"航空公司";
	cout.width(17);
	cout<<std::left<<"始发时间";
	cout.width(5);
	cout<<std::left<<"票价";
	cout.width(10);
	cout<<std::left<<"始发站";
	cout.width(10);
	cout<<std::left<<"终点站";
	cout.width(3);
	cout<<std::left<<"余票"<<endl;
	if (index==1){
		for (int i=0;i<F.size();i++){
			F[i].show();
		}
		cout<<endl<<endl;
	}
	if (index>1){
		for (int i=0;i<F.size();i++){
			if (F[i].getdate()==index)
				F[i].show();
		}
	}
}

void palistprint(){
	system("cls");
	cout.width(19);
	cout<<std::left<<"身份证号";
	cout.width(9);
	cout<<std::left<<"姓名";
	cout.width(5);
	cout<<std::left<<"性别";
	cout.width(12);
	cout<<std::left<<"电话号码"<<endl;
	for (int i=0;i<pass.size();i++){
		cout.width(19);
		cout<<std::left<<pass[i].getid();
		cout.width(9);
		cout<<std::left<<pass[i].getname();
		cout.width(5);
		char sex=pass[i].getsex();
		if (sex=='M') cout<<std::left<<" 男";
		else cout<<std::left<<" 女";
		cout.width(12);
		cout<<std::left<<pass[i].gettel()<<endl;
	}
	getch();
}

void savepa(){//保存用户信息
	system("cls");
	cout<<"正在保存用户信息...";
	fstream f("padb.txt",ios::out);
	for (int i=0;i<pass.size();i++){
		string id=pass[i].getid();
		string name=pass[i].getname();
		char sex=pass[i].getsex();
		string tel=pass[i].gettel();
		string pwd=pass[i].getpwd();
		string fl=pass[i].getfl();
		f<<id<<" "<<name<<" "<<sex<<" "<<tel<<" "<<pwd<<" "<<fl;
		if (i!=pass.size()-1) f<<endl;
	}
	f.close();
	Sleep(500);
}

void savefl(){//保存航班信息
	system("cls");
	cout<<"正在保存航班信息...";
	fstream f("fldb.txt",ios::out);
	for (int i=0;i<F.size();i++){
		string id=F[i].getid();
		string company=F[i].getco();
		vector<int> time=F[i].gett();
		int price=F[i].getprice();
		string departure=F[i].getdep();
		string destination=F[i].getdes();
		int rest=F[i].getrest();
		f<<id<<" "<<company<<" ";
		for (int i=0;i<5;i++) f<<time[i]<<" "<<price<<" "<<departure<<" "<<destination<<" ";
		
		if (i!=F.size()-1) f<<rest<<" "<<endl;
		else f<<rest;
	}
	f.close();
	Sleep(500);
}

void pwdchange(){//修改用户密码
	string pwdin,pwdc;
	while (1){
		pwdin=pwdc="";
		int in=0,flag=1;
		char g=' ';
		while (1){
			system("cls");
			cout<<"请输入修改后的密码：";
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
	pass[curpass].pwdchange(pwdin);
	savepa();
}

void purc(){//订票操作
	int date=1;
	while (1){
		system("cls");
		fllistprint(1);
		cout<<"请输入始发日期：";
		cin>>date;
		if (date<20||date>30){
			system("cls");
			cout<<"日期格式错误,请重新输入";
			getch();
			continue;
		}
		else break;
	}
	string dep,des;
	while (1){
		system("cls");
		fllistprint(date);
		cout<<"请输入始发地，目的地，用空格隔开：";
		cin>>dep>>des;
		int index=-1;
		for (int i=0;i<F.size();i++){
			if (F[i].getdate()==date&&F[i].getdep()==dep&&F[i].getdes()==des){
				index=i;
				break;
			}
		}
		if (index<0){
			system("cls");
			cout<<"该航班次不存在，请重新输入！";
			getch();
			continue;
		}
		else {
			int flag=F[index].purchase();
			if (flag){
				pass[curpass].purchase(index);
				savefl();
				savepa();
				break;
			}
		}
	}
}

int veriid(string idin,vector<int> pur){//航班号验证,返回查验到的航班序号
	int flag=-1;
	for (int i=0;i<pur.size();i++){
		if (F[pur[i]].getid()==idin){
			flag=pur[i];
			break;
		}
	}
	return flag;
}

void flinfosearch(){//航班信息查询
	vector<int> pur=pass[curpass].showpurchase();
	system("cls");
	if (pur.size()){
		fllistprint(0);
		for (int i=0;i<pur.size();i++) F[pur[i]].show();
	}
	else {
		system("cls");
		cout<<"未查到订票记录!";
	}
}

void refund(){//退票
	vector<int> pur=pass[curpass].showpurchase();
	if (pur.empty()){
		system("cls");
		cout<<"未查到订票记录，无法退票！";
		getch();
		return;
	}
	string idin="";
	int flag=-1;
	while (1){
		system("cls");
		fllistprint(0);
		for (int i=0;i<pur.size();i++) F[pur[i]].show();
		cout<<endl<<"请输入航班号进行退票操作：";
		cin>>idin;
		flag=veriid(idin,pur);
		if (flag==-1){
			system("cls");
			cout<<"输入的航班号有误，请重新输入。";
			getch();
			continue;
		}
		break;
	}
	pass[curpass].refund(flag);
	F[flag].refund();
	system("cls");
	cout<<"退票成功。";
	getch();
	savefl();
	savepa();
}

void flinfochange(){//修改航班信息
	string idin="";
	int flag=-1;
	while (1){
		system("cls");
		fllistprint(1);
		cout<<"请输入待修改的航班号:";
		cin>>idin;
		vector<int> tmp(30);
		for (int i=0;i<30;i++) tmp[i]=i;
		flag=veriid(idin,tmp);
		if (flag==-1){
			system("cls");
			cout<<"该航班号不存在,请重新输入。";
			getch();
			continue;
		}
		break;
	}
	int date,hour,mini;
	while (1){
		system("cls");
		cout<<"请输入修改后的始发日，时，分：";
		cin>>date>>hour,mini;
		if (date<20||date>30||hour<0||hour>24||mini<0||mini>60){
			system("cls");
			cout<<"时间格式错误";
			getch();
			continue;
		}
		break;
	}
	F[flag].timechange(date,hour,mini);
	savefl();
	getch();
}

void start(int index){//程序主框架
	while (1){
		system("cls");
		char ch='z';
		if (index==7) fllistprint(1);
		m[index].showmenu();
		while (ch<'0'||ch>m[index].getsize()) ch=getch();
		if (index!=1&&ch==m[index].getsize()) return;
		if (index==1){
			switch(ch){
				case '1':
					start(2);
				break;
				case '2':
					start(3);
				break;
				case '3':
					over();
					return;
				break;
			}
		}
		else if (index==2){
			switch(ch){
				case '1':
					paregist();
				break;
				case '2':
					start(9);
				break;
			}
		}
		else if (index==3){
			adminlogin=0;
			adlogin(adminlogin);
			system("cls");
			if (adminlogin==1){
				cout<<"登陆成功!"<<endl;
				getch();
				start(8);
				return;
			}
			if (!adminlogin) {
				cout<<"用户名或密码错误，登陆失败"<<endl;
				getch();
			}
			
		}
		else if (index==4){
			switch(ch){
				case'1':
					start(5);
				break;
				case'2':
					start(6);
				break;
			}
		}
		else if (index==5){
			switch(ch){
				case '1':
					pass[curpass].show();
				break;
				case '2':
					pwdchange();
					system("cls");
					cout<<"密码修改成功。";
					getch();
					return;
				break;
			}
		}
		else if (index==6){
			switch(ch){
				case '1':
					start(7);
				break;
				case '2':
					flinfosearch();
					getch();
				break;
				case '3':
					refund();
				break;
			}
		}
		else if (index==7){
			purc();
		}
		else if (index==8){
			switch(ch){
				case '1':
					flinfochange();
				break;
				case '2':
					palistprint();
				break;
			}
		}
		else if (index==9){
			switch(ch){
				case '1':
					passlogin=0;
					curpass=idlogin(passlogin);
					system("cls");
					if (passlogin==1){
						cout<<"登陆成功！";
						getch();
						start(4);
						return;
					}
					else if (passlogin==0){
						cout<<"密码错误。";
						getch();
					}
					else {
						cout<<"该身份证号不存在，请先注册！";
						getch();
					}
				break;
				case '2':
					passlogin=0;
					curpass=tellogin(passlogin);
					system("cls");
					if (passlogin==1){
						cout<<"登陆成功！";
						getch();
						start(4);
						return;
					}
					else if (passlogin==0){
						cout<<"密码错误。";
						getch();
					}
					else {
						cout<<"该手机号不存在，请先注册！";
						getch();
					}
				break;
			}
		}
	}
}


#endif //construct.h
