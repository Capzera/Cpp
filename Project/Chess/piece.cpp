#include "piece.h"

void Piece::init() {
	int width = 100;
	origin = newimage();
	if (white && type == 0) getimage(origin, "icon\\wp.png");
	if (white && type == 1) getimage(origin, "icon\\wr.png");
	if (white && type == 2) getimage(origin, "icon\\wn.png");
	if (white && type == 3) getimage(origin, "icon\\wb.png");
	if (white && type == 4) getimage(origin, "icon\\wq.png");
	if (white && type == 5) getimage(origin, "icon\\wk.png");
	if (!white && type == 0) getimage(origin, "icon\\bp.png");
	if (!white && type == 1) getimage(origin, "icon\\br.png");
	if (!white && type == 2) getimage(origin, "icon\\bn.png");
	if (!white && type == 3) getimage(origin, "icon\\bb.png");
	if (!white && type == 4) getimage(origin, "icon\\bq.png");
	if (!white && type == 5) getimage(origin, "icon\\bk.png");
	image = newimage(width, width);
	putimage(image, 0, 0, width, width, origin, 0, 0, getwidth(origin), getheight(origin));
	
}

Piece::Piece(bool color, int type, char x, int y) {
	this->white = color;
	this->type = type;
	this->Pos.x = x;
	this->Pos.y = y;
	init();
}

void Piece::showpiece() {
	if (dead) return;
	int y = 880 - Pos.y * 100;
	int x = 100 * (Pos.x - 'a' + 1) - 20;
	if (isselect) {
		setcolor(EGERGB(255, 255, 128));
		setlinestyle(0,0,4);
		int row = Pos.x - 'a' + 1;
		int col = Pos.y;
		int r1 = row * 100 - 20, c1 = 880 - 100 * col;
		int r2 = r1 + 100, c2 = c1 + 100;
		line(r1, c1, r1 + 33, c1);
		line(r1, c1, r1, c1 + 33);
		line(r2 - 33, c1, r2, c1);
		line(r2, c1, r2, c1 + 33);
		line(r1, c2 - 33, r1, c2);
		line(r1, c2, r1 + 33, c2);
		line(r2 - 33, c2, r2, c2);
		line(r2, c2 - 33, r2, c2);
	}
	if (ischeck) {
		setfillcolor(EGERGB(245, 155, 146));
		bar(x, y, x + 100, y + 100);
	}
	putimage_withalpha(NULL, image, x, y);
}

void Piece::setpiece(string name, bool color, int type, char x, int y) {
	this->name = name;
	this->white = color;
	this->type = type;
	this->Pos.x = x;
	this->Pos.y = y;
	this->dead = 0;
	this->kingstart = this->pawnstart = this->rookstart = this->isselect = this->ischeck = 0;
	init();
}

void Piece::setloc(char x, int y) {
	this->Pos.x = x;
	this->Pos.y = y;
}

bool Piece::getdead() {
	return this->dead;
}

pii Piece::getloc() {
	return {this->Pos.x - 'a' + 1, this->Pos.y};
}

string Piece::getname() {
	return this->name;
}

void Piece::pawnmove() {
	this->pawnstart = 1;
}

void Piece::kingmove() {
	this->kingstart = 1;
}

void Piece::rookmove() {
	this->rookstart = 1;
}

int Piece::gettype() {
	return this->type;
}

void Piece::isdead() {
	this->dead = 1;
	this->Pos.x = 'z';
	this->Pos.y = 26;
}

void Piece::isalive(char x, int y) {
	this->dead = 0;
	this->Pos.x = x;
	this->Pos.y = y;
}

void Piece::Select() {
	isselect = 1;
}

void Piece::clear() {
	isselect = 0;
}

bool Piece::getcolor() {
	return white;
}

bool Piece::getpawnstart() {
	return pawnstart;
}

bool Piece::getkingstart() {
	return kingstart;
}

bool Piece::getrookstart() {
	return rookstart;
}

void Piece::Check() {
	this->ischeck = 1;
}

void Piece::checkcancel() {
	this->ischeck = 0;
}

void Piece::promotion() {
	this->type = 4;
	init();
}
