#include "common.h"
#include "init.h"
#include "game.h"

int main() {
    init();
	game g;
	g.start();
	getch();
	closegraph();
    return 0;
}
