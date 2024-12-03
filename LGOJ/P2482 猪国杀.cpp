#include <bits/stdc++.h>
using namespace std;
class pigs{	
public:
	int id; // 编号
	int iden; // 身份0 主公，1忠臣，2反贼
	int HP; // 血量
	int dead; // 是否死亡
	int weapon; // 是否装备武器
	int prev; // 上一个活着玩家的编号
	int next; // 下一个活着玩家的编号
	int enermy; // 跳身份，0未跳身份，1跳身份
	string cards;
} pig[11];
const bool atk_Table[3][3] = { // 攻击表，阵营之间是否应该攻击
	{0, 0, 1},
	{0, 0, 1},
	{1, 1, 0}
};
deque<char> card_pile; // 牌堆
int n, m, renegade, dead_renegade, rounds;
char ch;
void init() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		pig[i].id = i; // 设置编号
 		pig[i].HP = 4; // 设置血量
		pig[i].weapon = pig[i].dead = pig[i].enermy = 0; // 未装备武器，没死，未表明身份
		pig[i].next = i % n + 1; // 下一个人的编号设置
		cin >> ch;
		if (ch == 'F') renegade++, pig[i].iden = 2; // 统计反贼数目以便于判定主公阵营胜利条件
		if (ch == 'M') pig[i].iden = 0;
		if (ch == 'Z') pig[i].iden = 1;
		cin >> ch;
		for (int j = 1; j <= 4; j++) {
			cin >> ch;
			pig[i].cards += ch;
		}
	}
	pig[1].enermy = 1;
	for (int i = 1; i <= m; i++) {
		cin >> ch;
		card_pile.push_back(ch);
	}
}

void get_card(int node) { // 摸牌
	pig[node].cards += card_pile.front(); // 给编号为node的猪摸牌
	if (card_pile.size() > 1) card_pile.pop_front(); // 牌堆减少一张堆顶
}

void healing(int node) { // 加血
	pig[node].HP++;
}

void player_equip_zgln(int node) { // 装备武器
	pig[node].weapon = 1;
}

void slained(int atk, int def) { // atk 将 node 彻底击杀
	pig[atk].next = pig[def].next; // 位置接续
	pig[def].dead = 1; // 设置死亡
	pig[def].cards = ""; // 手牌清空
	if (def == 1) return;
	if (pig[def].iden == 2) { // 如果是反贼则摸3张牌
		if (++dead_renegade == renegade) return;
		get_card(atk), get_card(atk), get_card(atk);
	}
	if (pig[atk].iden == 0 && pig[def].iden == 1) { // 如果主公杀了忠臣
		pig[atk].cards = ""; // 主公手牌清空
		pig[atk].weapon = 0; // 主公诸葛连弩下掉
	}
}

void attacked(int atk, int def) { // 受攻击方为node，伤害来源为origin
	pig[def].HP--;
	if (!pig[def].HP) { // 进入濒死状态
		int peach_loc = pig[def].cards.find("P");
		if (peach_loc == -1) { // 手中无桃
			slained(atk, def);
		} else {
			healing(def);
			pig[def].cards.erase(peach_loc, 1);
		}
	}
}

void dis_kill(int atk, int def) { // 由cur 攻击 nxt一次
	if (!pig[atk].enermy) pig[atk].enermy = 1;
	int defende_loc = pig[def].cards.find("D");
	if (defende_loc != -1) {
		pig[def].cards.erase(defende_loc, 1);
		return;
	}
	attacked(atk, def); // 无闪则执行伤害判定
}


bool can_atk(int node, int nxt) { // 返回node的目标是否可以攻击
	if (!pig[nxt].enermy) return 0; // 如果目标未表明身份，不能攻击
	return atk_Table[pig[node].iden][pig[nxt].iden];
}

bool dis_card(int node) { // 游戏主流程，出牌
	// return 0 表示游戏未结束， return 1表示游戏终止
	int nxt = pig[node].next; // 下一个活着的玩家
	int can_kill = can_atk(node, nxt); // 是否可以攻击
	int is_used_kill = 0;
	int i, round;
	for (round = 1; ; round++) {
		bool used = 0;
		for (i = 0; i < pig[node].cards.size(); i++) {
			switch (pig[node].cards[i]) {
			case 'P': // 桃
				if (pig[node].HP < 4) {
					healing(node);
					pig[node].cards.erase(i, 1);
					i = pig[node].cards.size();
					used = 1;
				}
				break;
			case 'K': // 杀
				if (can_kill && (pig[node].weapon || !is_used_kill)) {
					dis_kill(node, nxt);
					is_used_kill = 1;
					pig[node].cards.erase(i, 1);
					i = pig[node].cards.size();
					used = 1;
				}
				break;
			case 'Z': // 诸葛连弩
				player_equip_zgln(node);
				pig[node].cards.erase(i, 1);
				i = pig[node].cards.size();
				used = 1;
				break;
			default:
				break;
			}
		}
		if (dead_renegade == renegade || pig[1].dead) return 1; // 如果反贼全死，或主公死亡，游戏结束
		if (pig[node].dead) return 0; // 如果当前猪造成自己死亡，则游戏结束
		if (!used) break; // 未使用牌，则直接退出轮次
	}
	return 0;
}

bool game_start(int node) {
	get_card(node), get_card(node);
	return dis_card(node);
}

void game() {
	for (int i = 1, game_event = 0; !game_event && dead_renegade != renegade; i = pig[i].next) {
		//cout << endl;for (auto& x : pig[1].cards) cout << x << " ";cout << endl;
		if (!pig[i].dead) {
			game_event = game_start(i);
		}		
	}
}

void print() {
	cout << (pig[1].dead ? "FP" : "MP") << endl;
	for (int i = 1; i <= n; i++) {
		if (pig[i].dead) {
			cout << "DEAD";
		} else {
			for (auto& x : pig[i].cards) cout << x << " ";
		}
		cout << endl;
	}
}
int main() {
	init(); // 初始化
	game(); // 游戏开始
	print(); // 输出
	return 0;
}
/*

3 30
MP K P K K
ZP K K K K
FP D D D D
K K K K D K K K K K D K K K K K D K K K K K D K K K K K D K

3 30
MP D D D D
ZP K K K K
FP K K K K
D D K K K K D D K K K K D D K K K K D D K K K K D D K K K K

3 10
0 MP K K K K K K K K K K K K K
4 ZP K K K K K K K K K K K K
4 FP D D D D D D D

*/
