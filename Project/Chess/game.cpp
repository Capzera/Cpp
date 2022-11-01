#include "game.h"

void showmap() {
	int n = 100;
	setbkcolor(WHITE);
	//BLACK 181, 135, 99
	//WHITE 240, 218, 181
	setfillcolor(EGERGB(237, 238, 209));//白格预设颜色
	for (int x = 80; x < 880; x += 2 * n) {//填充白格
		for (int y = 80; y < 880; y += 2 * n) {
			bar(x, y, x + n, y + n);
		}
	}
	for (int x = 180; x < 880; x += 2 * n) {
		for (int y = 180; y < 880; y += 2 * n) {
			bar(x, y, x + n, y + n);
		}
	}
	setfillcolor(EGERGB(119, 153, 82));//黑格预设颜色
	for (int x = 80; x < 880; x += 2 * n) {//填充黑格
		for (int y = 180; y < 880; y += 2 * n) {
			bar(x, y, x + n, y + n);
		}
	}
	for (int x = 180; x < 880; x += 2 * n) {
		for (int y = 80; y < 880; y += 2 * n) {
			bar(x, y, x + n, y + n);
		}
	}
	setfont(20 , 0, "等线");
	setcolor(BLACK);
	int x = 120, y = 900;
	char alpha = 'A';
	for (; x < 880; x += n + 1  ) {
		outtextxy(x, y, alpha);
		alpha++;
	}
	x = 50, y = 120;
	char num = '8';
	for (; y < 880; y += n + 1) {
		outtextxy(x, y, num);
		num--;
	}
}

void game::init() {
	br1.setpiece("br1", 0, 1, 'a', 8);
	br2.setpiece("br2", 0, 1, 'h', 8);
	bn1.setpiece("bn1", 0, 2, 'b', 8);
	bn2.setpiece("bn2", 0, 2, 'g', 8);
	bb1.setpiece("bb1", 0, 3, 'c', 8);
	bb2.setpiece("bb2", 0, 3, 'f', 8);
	bq.setpiece("bq", 0, 4, 'd', 8);
	bk.setpiece("bk", 0, 5, 'e', 8);
	wr1.setpiece("wr1",1, 1, 'a', 1);
	wr2.setpiece("wr2", 1, 1, 'h', 1);
	wn1.setpiece("wn1", 1, 2, 'b', 1);
	wn2.setpiece("wn2", 1, 2, 'g', 1);
	wb1.setpiece("wb1", 1, 3, 'c', 1);
	wb2.setpiece("wb2", 1, 3, 'f', 1);
	wq.setpiece("wq", 1, 4, 'd', 1);
	wk.setpiece("wk", 1, 5, 'e', 1);
	for (int i = 0; i < 8; i++) {
		wp[i].setpiece("wp" + to_string(i), 1, 0, 'a' + i, 2);
		bp[i].setpiece("bp" + to_string(i), 0, 0, 'a' + i, 7);
	}
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			grid[{i, j}] = "";
		}
	}
	grid[{1, 8}] = "br1";
	grid[{8, 8}] = "br2";
	grid[{2, 8}] = "bn1";
	grid[{7, 8}] = "bn2";
	grid[{3, 8}] = "bb1";
	grid[{6, 8}] = "bb2";
	grid[{4, 8}] = "bq";
	grid[{5, 8}] = "bk";
	grid[{1, 1}] = "wr1";
	grid[{8, 1}] = "wr2";
	grid[{2, 1}] = "wn1";
	grid[{7, 1}] = "wn2";
	grid[{3, 1}] = "wb1";
	grid[{6, 1}] = "wb2";
	grid[{4, 1}] = "wq";
	grid[{5, 1}] = "wk";
	for (int i = 0; i < 8; i++) {
		grid[{i + 1, 2}] = "wp" + to_string(i);
		grid[{i + 1, 7}] = "bp" + to_string(i);
	}
}

void game::show() {
	showmap();
	br1.showpiece();
	br2.showpiece();
	bn1.showpiece();
	bn2.showpiece();
	bb1.showpiece();
	bb2.showpiece();
	bq.showpiece();
	bk.showpiece();
	wr1.showpiece();
	wr2.showpiece();
	wn1.showpiece();
	wn2.showpiece();
	wb1.showpiece();
	wb2.showpiece();
	wq.showpiece();
	wk.showpiece();
	for (int i = 0; i < 8; i++) {
		wp[i].showpiece();
		bp[i].showpiece();
	}
}

void game::getselect(string piece, Piece* &select) {
	if (piece == "br1") select = &br1;
	if (piece == "br2") select = &br2;
	if (piece == "bn1") select = &bn1;
	if (piece == "bn2") select = &bn2;
	if (piece == "bb1") select = &bb1;
	if (piece == "bb2") select = &bb2;
	if (piece == "bq") select = &bq;
	if (piece == "bk") select = &bk;
	if (piece == "wr1") select = &wr1;
	if (piece == "wr2") select = &wr2;
	if (piece == "wn1") select = &wn1;
	if (piece == "wn2") select = &wn2;
	if (piece == "wb1") select = &wb1;
	if (piece == "wb2") select = &wb2;
	if (piece == "wq") select = &wq;
	if (piece == "wk") select = &wk;
	bool color = (piece[0] == 'w');
	int num = piece[2] - '0';
	if (piece[1] == 'p') {
		if (color) select = &wp[num];
		else select = &bp[num];
	}
}

string game::checkmsg(mouse_msg msg, int choose, bool White) {
	int mx = (msg.x + 20)/ 100, my = 8 - ((msg.y - 80) / 100);//目标点击格
	if (choose == 1) {
		if (grid[{mx, my}] == "") return "";
		bool color = (grid[{mx, my}][0] == 'w');
		if (color == White)  {
			getselect(grid[{mx, my}], select);
			select->Select();
			return select->getname();
		}
		return "";
	}
	string res = move(select, mx, my, White);
	if (res == "" || res == "Cancel" || res == "Perpetualcheck") return res;
	if (checkdraw(White ^ 1)) return "Stalemate";
	if (!checkmate(White ^ 1)) return "1";
	return "Checkmate";
}

void game::start() {
	init();
	static bool White = 1;
	while (1) {
		show();
		mouse_msg msg = {0};
		while (!msg.is_down()) msg = getmouse();
		if (msg.x < 80 || msg.y < 80 || msg.x >= 880 || msg.y >= 880) continue;
		cleardevice();
		if (!flag) {
			select = nullptr;
			string piecechoose = checkmsg(msg, 1, White);
			if (piecechoose == "") continue;
			else {
				flag = 1;
				continue;
			}
		}
		string gridchoose = checkmsg(msg, 2, White);
		if (gridchoose == "") continue;//重新开始第二阶段
		if (gridchoose == "Cancel") {//退回第一阶段
			select->clear();
			flag = 0;
			continue;
		}
		if (gridchoose == "Checkmate") {
			int w = White;
			over(w ^ 1);
			break;
		}
		if (gridchoose == "Stalemate") {
			over(2);
			break;
		}
		if (gridchoose == "Perpetualcheck") {
			over(3);
			break;
		}
		flag = 0;
		White ^= 1;
	};
}

bool game::checkcheck(bool white) {
	Piece *p, *king;
	string target;
	if (white) getselect("wk", king);
	else getselect("bk", king);
	int x = king->getloc().first, y = king->getloc().second, kx, ky;
	int ndx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int ndy[] = {1, -1, 2, -2, 2, -2, 1, -1};
	//检查是否被兵将军
	ky = (white ? 1 : -1);//如果是白方则检测上方兵，如果是黑王则检测下方兵
	if (x < 8) {
		if (y + ky > 0 && y + ky <= 8) {
			target = grid[{x + 1, y + ky}];
			if (target != "") {
				getselect(target, p);
				if (p->getcolor() != white && p->gettype() == 0) return 1;
			}
		}
	}
	if (x > 1) {
		if (y + ky > 0 && y + ky <= 8) {
			target = grid[{x - 1, y + ky}];
			if (target != "") {
				getselect(target, p);
				if (p->getcolor() != white && p->gettype() == 0) return 1;
			}
		}
	}
	//检查是否被马将军
	for (int d = 0; d < 8; d++) {
		int mx = x + ndx[d], my = y + ndy[d];
		if (mx < 1 || mx > 8 || my < 1 || my > 8) continue;
		if (grid[{mx, my}] == "") continue;
		target = grid[{mx, my}];
		getselect(target, p);
		if (p->getcolor() != white && p->gettype() == 2) return 1;
	}
	//检查斜格是否有象或后
	for (int i = 0; i < 4; i++) {//枚举四个象限
		for (int j = 1; j < 8; j++) {//枚举距离
			if (i == 0 || i == 3) kx = 1;
			else kx = -1;
			if (i == 0 || i == 1) ky = 1;
			else ky = -1;
			int mx = x + kx * j, my = y + ky * j;
			if (mx < 1 || mx > 8 || my < 1 || my > 8) break;
			target = grid[{mx, my}];
			if (target == "") continue;
			getselect(target, p);
			if (p->getcolor() != white && (p->gettype() == 3 || p->gettype() == 4)) return 1;
			break;
		}
	}
	//检查直格是否有车或后
	for (int i = 0; i < 4; i++) {//枚举四个方向{0, 1},{-1, 0}, {0, -1}, {1, 0}
		for (int j = 1; j < 8; j++) {//枚举距离
			kx = ky = 0;
			if (i == 0) ky = 1;
			if (i == 1) kx = -1;
			if (i == 2) ky = -1;
			if (i == 3) kx = 1;
			int mx = x + kx * j, my = y + ky * j;
			if (mx < 1 || mx > 8 || my < 1 || my > 8) continue;
			target = grid[{mx, my}];
			if (target == "") continue;
			getselect(target, p);
			if (p->getcolor() != white && (p->gettype() == 1 || p->gettype() == 4)) return 1;
			break;
		}
	}
	return 0;
}

set<pii> game::calloc(Piece* &select) {
	set<pii> nextgrid;
	if (select->getdead()) return nextgrid;
	Piece* dead = nullptr;
	string target, name = select->getname();
	int cx = select->getloc().first, cy = select->getloc().second;
	int color = select->getcolor(), type = select->gettype();
	if (type == 0) {//计算兵的路线
		int ky = (color ? 1 : -1);
		if (cy + ky > 0 && cy + ky <= 8) {//判断兵直行1格
			target = grid[{cx, cy + ky}];
			if (target == "") {
				grid[{cx, cy + ky}] = name;
				grid[{cx, cy}] = "";
				select->setloc(cx + 'a' - 1, cy + ky);
				if (!checkcheck(color)) nextgrid.insert({cx, cy + ky});//走完不会被将军则可走
				grid[{cx, cy + ky}] = target;
				grid[{cx, cy}] = name;
				select->setloc(cx + 'a' - 1, cy);
			}
		}
		if (!select->getpawnstart() && grid[{cx, cy + ky}] == "" && grid[{cx, cy + 2 * ky}] == "") {
			grid[{cx, cy + ky * 2}] = name;
			grid[{cx, cy}] = "";
			select->setloc(cx + 'a' - 1, cy + ky * 2);
			if (!checkcheck(color)) nextgrid.insert({cx, cy + ky * 2});//走完不会被将军则可走
			grid[{cx, cy + ky * 2}] = target;
			grid[{cx, cy}] = name;
			select->setloc(cx + 'a' - 1, cy);
		}
		//如果要吃子,可能移动到{cx - 1, cy + ky} 和{cx + 1, cy + ky}
		if (cx > 1) {
			int mx = cx - 1, my = cy + ky;
			target = grid[{mx, my}];
			grid[{mx, my}] = name;
			grid[{cx, cy}] = "";
			select->setloc(mx + 'a' - 1, my);
			if (target != "") {
				getselect(target, dead);
				if (dead->getcolor() != color) {//目标格为敌方
					dead->isdead();
					if (!checkcheck(color)) nextgrid.insert({mx, my});//走完不会被将军则可走
					dead->isalive(mx + 'a' - 1, my);
				}
			}
			grid[{mx, my}] = target;
			grid[{cx, cy}] = name;
			select->setloc(cx + 'a' - 1, cy);
		}
		if (cx < 8) {
			int mx = cx + 1, my = cy + ky;
			target = grid[{mx, my}];
			grid[{mx, my}] = name;
			grid[{cx, cy}] = "";
			select->setloc(mx + 'a' - 1, my);
			if (target != "") {
				getselect(target, dead);
				if (dead->getcolor() != color) {//目标格为敌方
					dead->isdead();
					if (!checkcheck(color)) nextgrid.insert({mx, my});//走完不会被将军则可走
					dead->isalive(mx + 'a' - 1, my);
				}
			}
			grid[{mx, my}] = target;
			grid[{cx, cy}] = name;
			select->setloc(cx + 'a' - 1, cy);
		}
	}
	if (type == 1 || type == 4) {//计算城堡和皇后可以走的路线
		for (int i = 0; i < 4; i++) {
			int kx = 0, ky = 0;
			if (i == 0) ky = 1;
			if (i == 1) kx = -1;
			if (i == 2) ky = -1;
			if (i == 3) kx = 1;
			for (int j = 1; j <= 8; j++) {
				int mx = cx + kx * j, my = cy + ky * j;
				if (mx < 1 || mx > 8 || my < 1 || my > 8) continue;
				target = grid[{mx, my}];
				dead = nullptr;
				grid[{mx, my}] = name;
				grid[{cx, cy}] = "";
				select->setloc(mx + 'a' - 1, my);
				if (target != "") {//目标格有子
					getselect(target, dead);
					if (dead->getcolor() == color) {
						grid[{mx, my}] = target;
						grid[{cx, cy}] = name;
						select->setloc(cx + 'a' - 1, cy);
						break;//是友方直接退出
					}
					dead->isdead();//令目标格死亡
					if (!checkcheck(color)) nextgrid.insert({mx, my});//未被将军则可走
					dead->isalive(mx + 'a' - 1, my);
				}
				else {//目标格是空的
					//只需判断是否移动后被将军
					if (!checkcheck(color)) nextgrid.insert({mx, my});
				}
				grid[{mx, my}] = target;
				grid[{cx, cy}] = name;
				select->setloc(cx + 'a' - 1, cy);
				if (target != "") break;
			}
		}
	}
	if (type == 2) {//计算骑士的路线
		int ndx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
		int ndy[] = {1, -1, 2, -2, 2, -2, 1, -1};
		for (int d = 0; d < 8; d++) {
			int mx = cx + ndx[d], my = cy + ndy[d];
			if (mx < 1 || mx > 8 || my < 1 || my > 8) continue;
			target = grid[{mx, my}];
			dead = nullptr;
			grid[{mx, my}] = name;
			grid[{cx, cy}] = "";
			select->setloc(mx + 'a' - 1, my);
			if (target != "") {
				getselect(target, dead);
				if (dead->getcolor() == color) {
					grid[{mx, my}] = target;
					grid[{cx, cy}] = name;
					select->setloc(cx + 'a' - 1, cy);
					continue;//是友方则进行下一个位置判断
				}
				dead->isdead();
				if (!checkcheck(color)) nextgrid.insert({mx, my});
				dead->isalive(mx + 'a' - 1, my);
			}
			else {
				if (!checkcheck(color)) nextgrid.insert({mx, my});
			}
			grid[{mx, my}] = target;
			grid[{cx, cy}] = name;
			select->setloc(cx + 'a' - 1, cy);
			continue;
		}
	}
	if (type == 3 || type == 4) {//计算主教和皇后可以走的路线
		for (int i = 0; i < 4; i++) {//枚举象限
			int kx, ky;
			if (i == 0 || i == 3) kx = 1;//一三象限，x坐标为正系数
			else kx = -1;
			if (i == 0 || i == 1) ky = 1;//二四象限，y坐标为正系数
			else ky = -1;
			for (int j = 1; j <= 8; j++) {//枚举距离
				int mx = cx + kx * j, my = cy + ky * j;
				if (mx < 1 || mx > 8 || my < 1 || my > 8) break;
				target = grid[{mx, my}];
				dead = nullptr;
				grid[{mx, my}] = name;
				grid[{cx, cy}] = "";
				select->setloc(mx + 'a' - 1, my);
				if (target != "") {
					getselect(target, dead);
					if (dead->getcolor() == color) {
						grid[{mx, my}] = target;
						grid[{cx, cy}] = name;
						select->setloc(cx + 'a' - 1, cy);
						break;//是友方直接退出
					}
					dead->isdead();
					if (!checkcheck(color)) nextgrid.insert({mx, my});
					dead->isalive(mx + 'a' - 1, my);
				}
				else {
					if (!checkcheck(color)) nextgrid.insert({mx, my});
				}
				grid[{mx, my}] = target;
				grid[{cx, cy}] = name;
				select->setloc(cx + 'a' - 1, cy);
				if (target != "") break;
			}
		}
	}
	if (type == 5) {//计算国王可以走的路线
		int kdx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
		int kdy[] = {-1, 0, 1, 1, -1, -1, 0, 1};
		for (int d = 0; d < 8; d++) {
			int mx = cx + kdx[d], my = cy + kdy[d];//目标状态
			if (mx < 1 || mx > 8 || my < 1 || my > 8) continue;
			target = grid[{mx, my}];
			dead = nullptr;
			grid[{mx, my}] = name;
			grid[{cx, cy}] = "";
			select->setloc(mx + 'a' - 1, my);
			if (target != "") {//国王如果要吃子，不可以送吃
				getselect(target, dead);
				if (dead->getcolor() == color) {
					grid[{mx, my}] = target;
					grid[{cx, cy}] = name;
					select->setloc(cx + 'a' - 1, cy);
					continue;//是友方则进行下一个位置判断
				}
				dead->isdead();
				if (!checkcheck(color)) nextgrid.insert({mx, my});
				dead->isalive(mx + 'a' - 1, my);
			}
			else {
				if (!checkcheck(color)) nextgrid.insert({mx, my});
			}
			grid[{mx, my}] = target;
			grid[{cx, cy}] = name;
			select->setloc(cx + 'a' - 1, cy);
		}
		//易位格判断，无阻挡子，无移动，路径不被将军，当前不被将军
		if (checkcheck(color)) return nextgrid;//当前被将军
		if (select->getkingstart()) return nextgrid;//王移动过
		string target;
		if (color) target = "w";
		else target = "b";
		Piece *p1 ,*p2;
		getselect(target + "r1", p1);//长易位车
		getselect(target + "r2", p2);//短易位车
		if (p1->getrookstart() && p2->getrookstart()) return nextgrid;//车全移动过
		//长易位，王移动到cx - 2, cy   短易位,王移动到cx + 2, cy;
		bool check1, check2;
		if (!p2->getrookstart()) {//如果短易位车没动过
			if (grid[{6, cy}] == "" && grid[{7, cy}] == "") {//没有阻挡子
				select->setloc('f', cy);//模拟路径
				grid[{6, cy}] = select->getname();
				grid[{5, cy}] = "";
				check1 = checkcheck(color);//判断是否被将军
				select->setloc('g', cy);
				grid[{7, cy}] = select->getname();
				grid[{6, cy}] = "";
				check2 = checkcheck(color);
				if (!check1 && !check2) nextgrid.insert({7, cy});//路径不被将军
				grid[{5, cy}] = select->getname();
				grid[{7, cy}] = "";
				select->setloc('e', cy);
			}
		}
		if (!p1->getrookstart()) {//如果长易位车没动过
			if (grid[{3, cy}] == "" && grid[{4, cy}] == "" && grid[{2, cy}] == "") {//没有阻挡子
				select->setloc('d', cy);//模拟路径
				grid[{4, cy}] = select->getname();
				grid[{5, cy}] = "";
				check1 = checkcheck(color);//判断是否被将军
				select->setloc('c', cy);
				grid[{3, cy}] = select->getname();
				grid[{4, cy}] = "";
				check2 = checkcheck(color);
				if (!check1 && !check2) nextgrid.insert({3, cy});//路径不被将军
				grid[{5, cy}] = select->getname();
				grid[{3, cy}] = "";
				select->setloc('e', cy);
			}
		}	
	}
	return nextgrid;
}

string game::move(Piece* &select, int mx, int my, bool White) {
	int cx = select->getloc().first, cy = select->getloc().second;//记录初始格状态
	if (cx == mx && cy == my) return "Cancel";
	Piece* dead = nullptr;
	string target = grid[{mx, my}];
	if (target != "") {
		getselect(target, dead);
		if (dead->getcolor() == White) {
			select->clear();
			getselect(grid[{mx, my}], select);
			select->Select();
			return "";
		}
	}
	set<pii> tmp = calloc(select);//计算可行格范围
	if (tmp.find({mx, my}) == tmp.end()) return "Cancel";//点击格不在可行格范围内
	select->setloc(mx + 'a' - 1, my);
	grid[{mx, my}] = select->getname();
	grid[{cx, cy}] = "";
	if (select->gettype() == 0 && my == 7 * White + 1) select->promotion();
	if (target != "") {
		getselect(target, dead);
		dead->isdead();
	}
	if (select->getname()[1] == 'k' && select->getkingstart() == 0 && abs(mx - cx) == 2) {//王车易位判断
		bool Short = (mx > cx);//短易位判断
		string castle;
		if (select->getcolor()) castle = 'w';
		else castle = 'b';
		if (Short) castle += "r2";
		else castle += "r1";
		Piece* rook;
		getselect(castle, rook);
		if (Short) {
			rook->setloc('f', my);//更改短易位车位置
			grid[{6, my}] = castle;
			grid[{8, my}] = "";//消除原有位置
		}
		else {
			rook->setloc('d', my);//更改长易位车位置
			grid[{4, my}] = castle;
			grid[{1, my}] = "";//消除原有位置
		}
		rook->rookmove();
	}
	select->clear();
	if (select->gettype() == 0) select->pawnmove();//兵移动判断
	if (select->gettype() == 1) select->rookmove();//车移动判断
	if (select->gettype() == 5) select->kingmove();//王移动判断
	if (checkcheck(White ^ 1)) {//如果移动完后导致对方被将军
		string origin;
		origin = (White ? "bk" : "wk");
		Piece* king;
		getselect(origin, king);
		king->Check();
	}
	if (!checkcheck(White)) {//如果移动完后解除己方将军
		string origin;
		origin = (White ? "wk" : "bk");
		Piece* king;
		getselect(origin, king);
		king->checkcancel();
	}
	perpetualcheck[grid]++;//长将和计数器加一
	if (perpetualcheck[grid] >= 3 && checkcheck(White ^ 1)) return "Perpetualcheck";//如果判断长将和
	return "1";
}

int game::checkmate(bool white) {//检查当前!white方是否被将死，刚走完white棋
	if (!checkcheck(white)) return 0;//如果对方目前未被将军，不考虑将死
	Piece *king, *p;
	if (white) getselect("wk", king);//走完白棋，判断黑棋是否被将死
	else getselect("bk", king);
	set<pii> tmp = calloc(king);
	if (tmp.size()) return 0;
	//此时王无路可走，判断移动所有棋子是否可以满足解除将军
	string origin, target;
	if (white) origin = "w";//如果当前检查黑方，搜索黑方所有棋子是否可行
	else origin = "b";
	bool ans = 0;//如果有任何一个可行解，ans为1
	//尝试每一个棋子
	ans |= checkmove(origin + "r1");
	ans |= checkmove(origin + "r2");
	ans |= checkmove(origin + "n1");
	ans |= checkmove(origin + "n2");
	ans |= checkmove(origin + "b1");
	ans |= checkmove(origin + "b2");
	ans |= checkmove(origin + "q");
	for (int i = 0; i < 8; i++) {
		char c = i + '0';
		ans |= checkmove(origin + "p" + c);
	}
	if (ans) return 0;
	return 1;
}

bool game::checkmove(string origin) {
	//传入分析哪一个棋子，是哪一方，返回1或0,1则说明可以成功应将，0则说明该棋子无法应将
	Piece* p;
	getselect(origin, p);
	bool color = p->getcolor();
	if (p->getdead()) return 0;//如果该棋子死亡，一定不能够成功应将
	set<pii> tmp = calloc(p);
	Piece* finish;
	int cx = p->getloc().first, cy = p->getloc().second;//获取该棋子原始位置
	for (set<pii>::iterator it = tmp.begin(); it != tmp.end(); it++) {
		finish = nullptr;
		int mx = (*it).first, my = (*it).second;
		string target = grid[{mx, my}];//获取目标格信息
		if (target != "") getselect(target, finish);//将目标格指针导入finish
		if (finish != nullptr && color == finish->getcolor()) continue;//如果目标格为友方棋子直接跳过
		//尝试一次移动
		grid[{mx, my}] = p->getname();
		grid[{cx, cy}] = "";
		p->setloc(mx + 'a' - 1, my);
		if (finish == nullptr) {//当目标格为空值时
			if (!checkcheck(color)) {
				grid[{mx, my}] = "";//回溯棋子状态
				grid[{cx, cy}] = p->getname();
				p->setloc(cx + 'a' - 1, cy);
				return 1;
			}
			
		}
		else {//目标格有棋子并且不是本方棋子，吃子
			finish->isdead();//将目标格设为死棋
			if (!checkcheck(color)) {
				grid[{mx, my}] = target;
				finish->isalive(mx + 'a' - 1, my);
				grid[{cx, cy}] = p->getname();
				p->setloc(cx + 'a' - 1, cy);
				return 1;
			}
			finish->isalive(mx + 'a' - 1, my);//重新将目标格棋复活
		}
		grid[{mx, my}] = target;//恢复初始状态
		grid[{cx, cy}] = p->getname();
		p->setloc(cx + 'a' - 1, cy);
	}
	return 0;
}

bool game::checkdraw(bool white) {
	//传入的white表示判断该方是否被逼和逼和的条件为，当前没有被将军且无任何棋子可走
	if (checkcheck(white)) return 0;//当前处于被将军
	//枚举所有可行棋子
	Piece* p;
	set<pii> move;
	string origin;
	origin = (white ? "w" : "b");
	getselect(origin + "r1",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "r2",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "n1",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "n2",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "b1",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "b2",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "q",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	getselect(origin + "k",p);
	move.clear();
	move = calloc(p);
	if (move.size()) return 0;
	return 1;
	
}

void game::over(int result) {
	show();
	setcolor(BLACK);
	setfont(60, 0 , "等线");
	if (result == 0)
		outtextxy(250, 450, "游戏结束，白方胜");
	if (result == 1)
		outtextxy(250, 450, "游戏结束，黑方胜");
	if (result == 2)
		outtextxy(275, 450, "游戏结束，逼和");
	if (result == 3)
		outtextxy(250, 450, "游戏结束，长将和");
}
