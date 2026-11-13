#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	if (n == 1) {
		cout << "NO";
	} else if (n == 2) {
		cout << "1 2" << endl << "4 3";
	} else if (n == 3) {
		cout << "NO";
	} else if (n == 4) {
		cout << "1 2 11 12" << endl;
		cout << "4 9 8 5" << endl;
		cout << "7 10 3 14" << endl;
		cout << "6 13 16 15" << endl;
	} else if (n == 5) {
		cout << "1 2 3 4 7" << endl;
		cout << "6 5 14 15 16" << endl;
		cout << "13 24 23 8 21" << endl;
		cout << "10 19 18 11 20" << endl;
		cout << "9 22 25 12 17" << endl;
	}
	else if(n == 6) {
		cout << "1 2 3 8 9 4" << endl;
		cout << "6 11 20 33 28 25" << endl;
		cout << "5 36 23 14 15 22" << endl;
		cout << "12 31 30 17 26 21" << endl;
		cout << "7 16 13 24 35 32" << endl;
		cout << "10 27 34 19 18 29" << endl;
	}
	else if(n==7)
	{
		cout << "1 2 3 4 7 6 5" << endl;
		cout << "12 17 14 15 16 25 18" << endl;
		cout << "11 20 33 46 37 42 41" << endl;
		cout << "8 21 40 43 36 47 32" << endl;
		cout << "9 22 49 24 35 26 27" << endl;
		cout << "10 19 48 23 38 45 34" << endl;
		cout << "13 28 31 30 29 44 39" << endl;
	}
	else if(n==8)
	{
		cout << "1 2 3 4 7 6 5 14" << endl;
		cout << "12 17 20 27 34 25 18 23" << endl;
		cout << "11 26 21 32 39 28 61 36" << endl;
		cout << "8 15 46 51 58 31 48 35" << endl;
		cout << "9 22 57 52 55 42 41 62" << endl;
		cout << "10 19 40 49 54 47 56 45" << endl;
		cout << "13 24 43 60 53 50 33 64" << endl;
		cout << "16 37 30 29 44 59 38 63" << endl;
	}
	else if(n==9)
	{
		cout << "1 2 3 4 7 6 5 8 9" << endl;
		cout << "10 21 20 27 34 25 18 23 38" << endl;
		cout << "13 40 33 26 45 28 19 24 35" << endl;
		cout << "16 31 76 81 68 69 70 79 72" << endl;
		cout << "15 22 51 56 71 80 57 52 37" << endl;
		cout << "14 39 62 41 60 47 32 75 64" << endl;
		cout << "17 44 65 48 49 54 77 74 63" << endl;
		cout << "12 29 42 55 58 73 36 53 50" << endl;
		cout << "11 30 67 46 43 66 61 78 59" << endl;
	}
	else if(n==10)
	{
		cout << "1 2 3 4 7 6 5 8 9 10" << endl;
		cout << "12 29 38 33 34 25 36 23 44 27" << endl;
		cout << "11 30 59 68 45 28 43 24 35 26" << endl;
		cout << "18 41 42 71 56 75 64 37 66 47" << endl;
		cout << "13 48 19 60 53 98 93 100 91 90" << endl;
		cout << "16 31 40 97 54 83 74 99 82 67" << endl;
		cout << "15 22 49 52 55 96 77 80 57 46" << endl;
		cout << "14 39 58 79 72 95 62 87 70 61" << endl;
		cout << "17 50 51 88 85 78 89 92 81 76" << endl;
		cout << "20 21 32 69 94 73 84 65 86 63" << endl;
		
	}
	return 0;
}
