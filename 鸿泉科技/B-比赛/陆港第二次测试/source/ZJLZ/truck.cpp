#include <iostream>
#include <cmath>

using namespace std;

long long calcDist(long long m, long long n, long long s, long long l) {
	long long dist = 0;
	long long mt = m;
	long long at = n;

	while (mt > 0) {
		if (mt >= l) {
			dist += l * s;
			mt -= l;

			if (at > 0) {
				mt += 1;
				at -= 1;
			}
		} else {
			dist += mt * s;
			mt = 0;
		}
	}

	return dist;
}

int main() {
	long long m, n, s, l;

	cin >> m >> n >> s >> l;

	long long maxDist = calcDist(m, n, s, l);

	cout << maxDist << endl;

	return 0;
}
