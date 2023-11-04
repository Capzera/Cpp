#include "mainwindow.h"
#include "ui_mainwindow.h"

void reverse(QString &s) {
    int n = s.size();
    for (int i = 0; i < n / 2; i++) {
        QChar t = s[i];
        s[i] = s[n - i - 1];
        s[n - i - 1] = t;
    }
}

void print(QVector<int> nums) {
    QString ans;
    for (auto x : nums) {
        ans += QString::number(x);
        ans += " ";
    }
    qDebug() << ans;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    btn1 = ui->btn1;
    lb1 = ui->lb1;
    lb2 = ui->lb2;
    le1 = ui->le1;
    pte1 = ui->pte1;
    connect(btn1, SIGNAL(clicked(bool)), this, SLOT(mousePressEvent()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent() {
    shoupai = le1->text();
    shoupai = shoupai.trimmed();
    //if (shoupai.size() != 26) return;

    Init();

    Solve();

    QString ans;

    for (auto &tp : tenpai.toStdMap()) {
        QString key = tp.first;
        ans += key;
        ans.push_back(" ");
    }

    if (ans == "") ans = "未听牌";
    pte1->setPlainText(ans);
}

void MainWindow::Init() {
    tenpai.clear();
    mp.clear();
    pai.clear();
    nom = nop = nos = noz = 1;
    for (int i = 0; i < shoupai.size(); i += 2) {
        QString key = shoupai.mid(i, 2);//每次截取手牌的2位字符
        reverse(key);//字符反转为先字母后数字，方便进行排序。
        int encode = 0;
        if (key[0] == 'm') {
            encode = 0;
            nom = 0;
        }
        if (key[0] == 'p') {
            encode = 10;
            nop = 0;
        }
        if (key[0] == 's') {
            encode = 20;
            nos = 0;
        }
        if (key[0] == 'z') {
            encode = 30;
            noz = 0;
        }
        encode += key[1].toLatin1() - '0';
        if (encode % 10 == 0) encode += 5;
        mp[encode]++;
        pai.push_back(encode);
    }
}

QString MainWindow::Decode(int code) {
    QString ans = "";
    ans += QString::number(code % 10);
    code /= 10;
    if (code == 0) ans += 'm';
    if (code == 1) ans += 'p';
    if (code == 2) ans += 's';
    if (code == 3) ans += 'z';
    return ans;
}

void MainWindow::Solve() {
    //暴力把每个牌插入牌组中，看是否能够达成和牌
    for (int p = 1; p < 40; p++) {
        if (p % 10 == 0) continue;
        //如果不含有任何一种花色,无需进行听牌模拟
        if (nom && p / 10 == 0) continue;
        if (nop && p / 10 == 1) continue;
        if (nos && p / 10 == 2) continue;
        if (noz && p / 10 == 3) continue;
        if (mp[p] == 4) continue;

        //将牌插入数组
        pai.push_back(p);
        //如果和了，则说明是个可行答案
        if (CheckHu(pai)) {
            tenpai[Decode(p)] = 1;
        }
        //把牌抽出，进行下一次搜索
        pai.pop_back();

    }
}

bool MainWindow::dfs(QVector<int> pai, int cnt) {
    if (cnt == 5) return 1;
    if (pai[1] - pai[0] > 1) return 0;
    QVector<int> Pair = havePair(pai);
    //先考虑扣掉雀头
    if (!cnt) {
        if (Pair.size() == 7) return 1;//7对子
        if (Pair.empty()) return 0;
        for (auto& pair : Pair) {
            QVector<int> nxt(pai);
            removePair(nxt, pair);
            if (dfs(nxt, cnt + 1)) return 1;
        }
    }
    //考虑扣掉刻子
    QVector<int> Triple = haveTriple(pai);
    if (!Triple.empty()) {
        for (auto& tri : Triple) {
            QVector<int> nxt(pai);
            removeTriple(nxt, tri);
            if (dfs(nxt, cnt + 1)) return 1;
        }
    }
    //考虑扣掉顺子
    QVector<int> Straight = haveStraight(pai);
    if (!Straight.empty()) {
        for (auto& str : Straight) {
            QVector<int> nxt(pai);
            removeStraight(nxt, str);
            if (dfs(nxt, cnt + 1)) return 1;
        }
    }
    return 0;
}

bool MainWindow::CheckHu(QVector<int> pai) {
    sort(pai.begin(), pai.end());
    return dfs(pai, 0);
}

QVector<int> MainWindow::haveStraight(QVector<int> pai) {
    QVector<int> ans;
    for (int s1 = 0; s1 < pai.size() - 2; s1++) {
        if (s1 && pai[s1] == pai[s1 - 1]) continue;
        if (pai[s1] > 30) break;
        int s2 = pai.indexOf(pai[s1] + 1);
        if (s2 == -1) continue;
        int s3 = pai.indexOf(pai[s1] + 2);
        if (s3 == -1) continue;
        ans.push_back(pai[s1]);
    }

    return ans;
}

QVector<int> MainWindow::haveTriple(QVector<int> pai) {
    QVector<int> ans;
    for (int t1 = 0; t1 < pai.size() - 2; t1++) {
        if (t1 && pai[t1] == pai[t1 - 1]) continue;
        if (pai[t1] == pai[t1 + 1] && pai[t1] == pai[t1 + 2]) {
            ans.push_back(pai[t1]);
        }
    }
    return ans;
}

QVector<int> MainWindow::havePair(QVector<int> pai) {
    QVector<int> ans;
    for (int p1 = 0; p1 < pai.size() - 1; p1++) {
        if (p1 && pai[p1] == pai[p1 - 1]) continue;
        if (pai[p1] == pai[p1 + 1]) ans.push_back(pai[p1]);
    }
    return ans;
}

void MainWindow::removeStraight(QVector<int>& pai, int st) {
    int l1 = pai.indexOf(st + 2);
    pai.remove(l1);
    l1 = pai.indexOf(st + 1);
    pai.remove(l1);
    l1 = pai.indexOf(st);
    pai.remove(l1);
}

void MainWindow::removeTriple(QVector<int>& pai, int tr) {
    int st = pai.indexOf(tr);
    if (st == -1) return;
    pai.remove(st, 3);
}

void MainWindow::removePair(QVector<int>& pai, int pa) {
    int st = pai.indexOf(pa);
    if (st == -1) return;
    pai.remove(st, 2);
}
/*
4p0p6p7p8p9p2s3s4s4s5s8s8s 3s6s
1m1m3m3m0m5m7m7m2p2p8s8s6z 6z
1m2m2m3m3m4m5m5m6m6m7m7m8m 2m5m8m
1m1m1m2m3m4m5m6m7m8m9m9m9m 1m2m3m4m5m6m7m8m9m
1p1p2p3p3p4p4p0p5p6p7p7p8p 6p9p
1m2m2m3m3m4m5m5m5m6m6m7m7m 2m5m6m7m8m
2s2s2s3s4s5s0s5s6s6s6s7s7s 1s2s3s4s5s6s7s
*/
