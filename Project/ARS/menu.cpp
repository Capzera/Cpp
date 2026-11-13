#include "menu.h"

menu::menu(){
	quantity=0;
}

void menu::namein(string addr){
	fstream f;
	f.open(addr,ios::in);
	string tmp;
	if (!f.is_open())
		cout<<"ERROR"<<endl;
	while(!f.eof()){
		f>>tmp;
		quantity++;
		name.push_back(tmp);
	}
	f.close();
}

void menu::showmenu(){
	Sleep(10);
	cout<<"       @        "<<endl;
	cout<<"      / \\	   "<<endl;
	cout<<"      ***	   "<<endl;
	cout<<"      ***	   "<<endl;
	cout<<"      ***       "<<endl;
	cout<<"  ***********   "<<endl;
	cout<<" ************** "<<endl;
	cout<<"****************"<<endl;
	cout<<"      ***"<<endl;
	cout<<"      ***"<<endl;
	cout<<"      ***"<<endl;
	cout<<"     *****"<<endl;
	cout<<"    *******"<<endl;
	cout<<"----------------------"<<endl;
	int n=name.size();
	for (int i=0;i<n;i++){
		cout<<name[i]<<endl;
	}
	cout<<"----------------------"<<endl;
}

int menu::getsize(){
	return name.size()+48;
}
