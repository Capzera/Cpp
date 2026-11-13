#ifndef _PASSENGER_H_
#define _PASSENGER_H_

#include "common.h"

class passenger{
private:
	string id;
	string name;
	char sex;
	string tel;
	string pwd;
	string fl;
public:
	passenger();
	void infoin(string,string,char,string,string,string);//ÐÅÏ¢Â¼Èë;
	void show();
	vector<int> showpurchase();
	void purchase(int);
	void refund(int);
	void pwdchange(string);
	string getid();
	string getname();
	char getsex();
	string gettel();
	string getpwd();
	string getfl();
	~passenger(){};
};

#endif
