#include "flight.h"

flight::flight(){
	time=vector<int> (5);
}

void flight::infoin(string i,string c,vector<int> t,int p,string d,string de,int r){
	time.resize(5);
	id=i;
	company=c;
	for (int i=0;i<5;i++){
		time[i]=t[i];
	}
	price=p;
	departure=d;
	destination=de;
	rest=r;
}

void flight::show(){
	cout.width(7);
	cout<<std::left<<id;
	cout.width(17);
	cout<<std::left<<company;
	cout<<time[0]<<"-";
	if (time[1]<10) cout<<"0";
	cout<<time[1]<<"-";
	if (time[2]<10) cout<<"0";
	cout<<time[2]<<" ";
	if(time[3]<10) cout<<"0";
	cout<<time[3]<<":";
	if (time[4]<10) cout<<"0";
	cout<<time[4]<<" ";
	cout.width(5);
	cout<<std::left<<price;
	cout.width(10);
	cout<<std::left<<departure;
	cout.width(10);
	cout<<std::left<<destination;
	cout.width(3);
	cout<<std::left<<rest<<endl;
}

void flight::refund(){
	rest++;
}

int flight::purchase(){
	if (rest){
		rest--;
		return 1;
	}
	system("cls");
	cout<<"ÓàÆ±²»×ã£¡";
	getch();
	return 0;
}

int flight::getdate(){
	return time[2];
}

string flight::getdep(){
	return departure;
}

string flight::getdes(){
	return destination;
}

string flight::getid(){
	return id;
}

string flight::getco(){
	return company;
}

vector<int> flight::gett(){
	return time;
}

int flight::getprice(){
	return price;
}

int flight::getrest(){
	return rest;
}

void flight::timechange(int a,int b,int c){
	time[2]=a;
	time[3]=b;
	time[4]=c;
}
