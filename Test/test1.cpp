#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int main(){
	srand(time(NULL));
	cout<<1<<endl;
	fstream f("D:\\Program\\C++\\Project\\Wordle\\Words\\dictionary.txt");
	vector<string> data;
	string Answer;
	string tmp;
	while (getline(f,tmp)){
		data.push_back(tmp);
	}
	f.close();
	int n=data.size();
	int ran=rand()%n;
	Answer=data[ran];
	fstream file;
	file.open("C:\\Users\\96987\\Desktop\\Cache.txt",ios::out);
	file<<Answer<<endl;
	file.close();
	return 0;
}


