#include "testlib.h"
using namespace std;
int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);
	string my = ouf.readString();
	string cor = ans.readString();
	for (auto& x : my) {
		x = toupper(x);
	}
	if (my == cor) {
		quitf(_ok, "The answer is correct.");
	} else {
		quitf(_wa, "The answer is wrong: expected = %s, found = %s", cor, my);
	}
}
