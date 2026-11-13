#ifndef _FLIGHT_H_
#define _FLIGHT_H_

#include "common.h"

class flight{
public:
	flight();
	void infoin(string,string,vector<int>,int,string,string,int);
	void show();
	int purchase();
	void refund();
	int getdate();
	void timechange(int,int,int);
	string getid();
	string getco();
	vector<int>gett();
	int getprice();
	string getdep();
	string getdes();
	int getrest();
	~flight(){};
private:
	string id;
	string company;
	vector<int> time;
	int price;
	string departure;
	string destination;
	int rest;
};

#endif //flight.h
