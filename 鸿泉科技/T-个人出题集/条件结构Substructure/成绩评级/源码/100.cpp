#include <iostream>
using namespace std;
int main() {
	double s;
	cin >> s;
	if (s >= 95) {
		cout << "A+" << endl;
	} else if (s >= 90) {
		cout << "A" << endl;
	} else if (s >= 85) {
		cout << "B+" << endl;
	} else if (s >= 80) {
		cout << "B" << endl;
	} else if (s >= 75) {
		cout << "C+" << endl;
	} else if (s >= 70) {
		cout << "C" << endl;
	} else if (s >= 65) {
		cout << "D+" << endl;
	} else if (s >= 60) {
		cout << "D" << endl;
	} else if (s >= 40) {
		cout << "E" << endl;
	} else if (s >= 0) {
		cout << "F" << endl;
	}
	return 0;
}
