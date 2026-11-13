#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("原神抽卡模拟系统  By:机长大大&cnl123 V1.2");//设置标题
    initok = 0;
    role5 = {
        "请选择五星角色",
        "提那里",
        "刻晴",
        "莫娜",
        "七七",
        "迪卢克",
        "琴",
        "艾尔海森",
        "流浪者",
        "纳西妲",
        "妮露",
        "赛诺",
        "夜兰",
        "神里绫人",
        "八重神子",
        "申鹤",
        "荒泷一斗",
        "珊瑚宫心海",
        "雷电将军",
        "宵宫",
        "神里绫华",
        "枫原万叶",
        "优菈",
        "胡桃",
        "魈",
        "甘雨",
        "阿贝多",
        "钟离",
        "达达利亚",
        "可莉",
        "温蒂"
    };
    role4 = {
        "请选择四星角色",
        "瑶瑶",
        "珐露珊",
        "莱依拉",
        "坎蒂丝",
        "多莉",
        "柯莱",
        "鹿野苑平藏",
        "久岐忍",
        "云堇",
        "五郎",
        "托马",
        "九条裟罗",
        "早柚",
        "烟绯",
        "罗莎莉亚",
        "辛焱",
        "迪奥娜",
        "砂糖",
        "重云",
        "诺艾尔",
        "班尼特",
        "菲谢尔",
        "凝光",
        "行秋",
        "北斗",
        "香菱",
        "雷泽",
        "芭芭拉"
    };
    weapon4 = {
        "弓藏",
        "绝弦",
        "昭心",
        "流浪乐章",
        "西风长枪",
        "雨裁",
        "钟剑",
        "匣里龙吟",
        "笛剑",
        "祭礼弓",
        "西风猎弓",
        "祭礼残章",
        "西风秘典",
        "匣里灭辰",
        "祭礼大剑",
        "西风大剑",
        "祭礼剑",
        "西风剑",
    };
    weapon3 = {
        "弹弓",
        "鸦羽弓",
        "讨龙英杰谭",
        "黑缨枪",
        "沐浴龙血的剑",
        "飞天御剑",
        "冷刃",
        "神射手之誓",
        "翡玉法球",
        "魔导绪论",
        "以理服人",
        "铁影阔剑",
        "黎明神剑",
    };
    preset_name = {
        "自定义预设",
        "V1.0(上)杯装之诗",
        "V1.0(下)闪焰的驻足",
        "V1.1(上)暂别冬都",
        "V1.1(下)陵薮市朝",
        "V1.2(上)深秘之息",
        "V1.2(下)浮生孰来",
        "V1.3(上)烟火之邀",
        "V1.3(中)鱼龙灯昼",
        "V1.3(下)赤团开时",
        "V1.4(上)杯装之诗",
        "V1.4(下)暂别冬都",
        "V1.5(上)陵薮市朝",
        "V1.5(下)浪涌之瞬",
        "V1.6(上)闪焰的驻足",
        "V1.6(下)叶落风随",
        "V2.0(上)白鹭之庭",
        "V2.0(下)焰色天河",
        "V2.1(上)影寂天下人",
        "V2.1(下)浮岳虹珠",
        "V2.2(上)暂别冬都",
        "V2.2(下)赤团开时",
        "V2.3(上)深秘之息",
        "V2.3(上-2)浪涌之瞬",
        "V2.3(下)鬼门斗宴",
        "V2.4(上)出尘入世",
        "V2.4(上-2)烟火之邀",
        "V2.4(下)陵薮市朝",
        "V2.4(下-2)浮生孰来",
        "V2.5(上)华紫樱绯",
        "V2.5(下)影寂天下人",
        "V2.5(下-2)浮岳虹珠",
        "V2.6(上)苍流踏花",
        "V2.6(上-2)杯装之诗",
        "V2.6(下)白鹭之庭",
        "V2.7(上)素霓伣天",
        "V2.7(上-2)烟火之邀",
        "V2.7(下)鬼门斗宴",
        "V2.8(上)叶落风随",
        "V2.8(上-2)闪焰的驻足",
        "V2.8(下)焰色天河",
        "V3.0(上)巡御蘙荟",
        "V3.0(上-2)陵薮市朝",
        "V3.0(下)浮生孰来",
        "V3.0(下-2)浮岳虹珠",
        "V3.1(上)雳裁冥昭",
        "V3.1(上-2)杯装之诗",
        "V3.1(下)翩舞歈莲",
        "V3.1(下-2)深秘之息",
        "V3.2(上)月草的赐慧",
        "V3.2(上-2)焰色天河",
        "V3.2(下)华紫樱绯",
        "V3.2(下-2)暂别冬都",
        "V3.3(上)余火变相",
        "V3.3(上-2)鬼门斗宴",
        "V3.3(下)影寂天下人",
        "V3.3(下-2)苍流踏花",
        "V3.4(上)敕诫枢谋",
        "V3.4(上-2)烟火之邀",
        "V3.4(下)赤团开时",
        "V3.4(下-2)素霓伣天",
    };
    preset = {
        {12,23,24,25},
        {30,22,26,28},
        {29,18,20,24},
        {28,17,23,25},
        {27,16,19,27},
        {26,18,21,22},
        {25,20,24,26},
        {24,16,17,25},
        {0,0,0,0},
        {23,18,20,27},
        {30,19,24,26},
        {28,15,22,28},
        {27,14,17,20},
        {22,16,24,25},
        {29,18,22,28},
        {21,15,21,27},
        {20,14,19,23},
        {19,13,16,17},
        {18,12,18,26},
        {17,15,24,25},
        {28,14,19,23},
        {23,11,13,17},
        {26,15,20,21},
        {22,15,20,21},
        {16,10,26,28},
        {15,9,19,23},
        {24,9,19,23},
        {27,14,24,25},
        {25,14,24,25},
        {14,11,17,22},
        {18,12,16,21},
        {17,12,16,21},
        {13,9,18,26},
        {30,9,18,26},
        {20,13,15,27},
        {12,14,20,28},
        {24,14,20,28},
        {16,8,10,19},
        {21,7,11,23},
        {29,7,11,23},
        {19,9,16,21},
        {0,0,0,0},
        {27,0,0,0},
        {26,5,18,24},
        {17,5,18,24},
        {11,4,8,13},
        {30,4,8,13},
        {10,25,26,28},
        {26,25,26,28},
        {9,20,21,27},
        {19,20,21,27},
        {14,3,7,11},
        {28,3,7,11},
        {8,2,10,14},
        {16,2,10,14},
        {18,12,13,15},
        {20,12,13,15},
        {7,1,9,16},
        {24,1,9,16},
        {23,23,24,25},
        {12,23,24,25},
    };

    btn1 = new QPushButton("初始化", this);
    btn2 = new QPushButton("抽卡模拟", this);
    lb1 = new QLabel("当期UP角色:", this);
    lb2 = new QLabel("已垫抽:", this);
    lb3 = new QLabel("所有抽:", this);
    lb4 = new QLabel("当前5星小保底是否存在:", this);
    lb5 = new QLabel("演算结果:", this);
    lb6 = new QLabel("当前4星小保底是否存在:", this);
    lb7 = new QLabel("模拟总次数:", this);
    lb8 = new QLabel("预设", this);
    check1 = new QCheckBox(this);
    check2 = new QCheckBox(this);
    le1 = new QLineEdit(this);
    le1->setPlaceholderText(tr("请输入目前已垫抽数(5星，4星)"));
    le2 = new QLineEdit(this);
    le2->setPlaceholderText(tr("请输入目前已有抽数"));
    le3 = new QLineEdit(this);
    result = new QPlainTextEdit(this);
    result->setReadOnly(1);
    result->setPlaceholderText("请先进行初始化后进行模拟       制作者：机长大大&cnl123");
    cb1 = new QComboBox(this);
    for (int i = 0; i < role5.size(); i++) {
        if (i && i < 7) continue;
        cb1->addItem(role5[i]);
    }
    cb2 = new QComboBox(this);
    cb3 = new QComboBox(this);
    cb4 = new QComboBox(this);
    cb5 = new QComboBox(this);
    for (auto& s : role4) {
        cb2->addItem(s);
        cb3->addItem(s);
        cb4->addItem(s);
    }
    for (auto& s : preset_name) {
        cb5->addItem(s);
    }
    cb5->setCurrentIndex(0);
    init_data();
    gl1 = new QGridLayout;
    gl1->addWidget(lb1, 0, 0);
    gl1->addWidget(cb1, 0, 1);
    gl1->addWidget(cb2, 0, 2);
    gl1->addWidget(cb3, 0, 3);
    gl1->addWidget(cb4, 0, 4);
    gl1->addWidget(lb2, 1, 0);
    gl1->addWidget(le1, 1, 1, 1, 2);
    gl1->addWidget(lb4, 1, 3);
    gl1->addWidget(check1, 1, 4);
    gl1->addWidget(lb3, 2, 0);
    gl1->addWidget(le2, 2, 1, 1, 2);
    gl1->addWidget(lb6, 2, 3);
    gl1->addWidget(check2, 2, 4);
    gl1->addWidget(lb7, 3, 0);
    gl1->addWidget(le3, 3, 1, 1, 2);
    gl1->addWidget(btn1, 3, 3);
    gl1->addWidget(btn2, 3, 4);
    gl1->addWidget(lb8, 4, 0);
    gl1->addWidget(cb5, 4, 1, 1, 6);
    gl1->addWidget(lb5, 5, 0);
    gl1->addWidget(result, 6, 0, 3, 7);
    setLayout(gl1);
    connect(btn1, SIGNAL(clicked(bool)), this, SLOT(Init()));
    connect(btn2, SIGNAL(clicked(bool)), this, SLOT(Solve()));
    connect(cb5, SIGNAL(currentIndexChanged(int)), this, SLOT(PresetChange(int)));
}

void Widget::Init() {
    result->setPlainText("正在进行初始化...");
    pad4 = pad5 = total = times = 0;
    guarantee4 = guarantee5 = 0;
    QString str = le2->text();
    for (auto& c : str) {
        char x = c.toLatin1();
        if (x < '0' || x > '9') break;
        total = total * 10 + x - '0';
    }
    if (!total) {
        result->setPlainText("总抽数格式错误，初始化失败...");
        return;
    }
    str = le1->text();
    bool flag = 1;
    for (auto& c : str) {
        char x = c.toLatin1();
        if (x < '0' || x > '9') flag = 0;
        else {
            if (flag) pad5 = pad5 * 10 + x - '0';
            else pad4 = pad4 * 10 + x - '0';
        }
    }
    str = le3->text();
    times = str.toInt();
    if (!times) {
        result->setPlainText("总模拟次数格式错误，初始化失败...");
        return;
    }
    int in2 = cb2->currentIndex(), in3 = cb3->currentIndex(), in4 = cb4->currentIndex();
    if (in2 == in3 || in2 == in4 || in3 == in4 || !in2 || !in3 || !in4) {
        result->setPlainText("四星Up数据冲突，初始化失败...");
        return;
    }
    if (pad4 > 9) {
        result->setPlainText("四星保底数据有误，初始化失败...");
        return;
    }
    if (pad5 > 89) {
        result->setPlainText("五星保底数据有误，初始化失败...");
        return;
    }
    guarantee4 = check2->checkState();
    guarantee5 = check1->checkState();
    initok = 1;
    result->setPlainText("初始化成功...");
}

void Widget::Solve() {
    QDir *folder = new QDir;
    bool folder_exist = folder->exists("./logs");
    if (!folder_exist) {
        folder->mkpath("./logs");
    }
    total3 = total4 = total5 = ga = 0;
    fate_count = QVector<int> (8, 0);
    rng = new QRandomGenerator(QTime::currentTime().msecsSinceStartOfDay());
    if (!initok) {
        init_data();
        result->setPlainText("初始化未通过，请预设数据");
        return;
    }
    int backup_pad4 = pad4, backup_pad5 = pad5;
    bool backup_guarantee4 = guarantee4, backup_guarantee5 = guarantee5;
    um.clear();
    for (int t = 0; t < times; t++) {
        pad4 = backup_pad4, pad5 = backup_pad5;
        guarantee4 = backup_guarantee4, guarantee5 = backup_guarantee5;
        int isUp = 0;
        QVector<QString> ans = Gacha();
        for (auto& str : ans) {
            um[str]++;
            if (str == role5[curIn1]) isUp++;
        }
        if (isUp > 7) isUp = 7;
        fate_count[isUp]++;
    }
    QString ret = "五星角色：";
    ret += role5[curIn1] + "x" + QString::number(um[role5[curIn1]]) + ", ";
    for (auto& str : role5) {
        if (!um.count(str)) continue;
        if (role5[curIn1] == str) continue;
        ret += str + "x" + QString::number(um[str]) + ", ";
    }
    int cnt = 0;
    ret.chop(2), ret += ";\n四星角色：";
    for (auto& str : role4) {
        if (!um.count(str)) continue;
        ret += str + "x" + QString::number(um[str]) + ", ";
        cnt++;
    }
    if (cnt) ret.chop(2);
    cnt = 0;
    ret += ";\n四星武器：";
    for (auto& str : weapon4) {
        if (!um.count(str)) continue;
        ret += str + "x" + QString::number(um[str]) + ", ";
        cnt++;
    }
    if (cnt) {
        ret.chop(2);
        ret += ";\n";
    }
    Record(backup_pad5, backup_pad4, backup_guarantee5, backup_guarantee4);
    result->setPlainText(ret);
}

void Widget::init_data() {
    cb1->setCurrentIndex(curIn1 - 6);
    cb2->setCurrentIndex(curIn2);
    cb3->setCurrentIndex(curIn3);
    cb4->setCurrentIndex(curIn4);
    le1->clear();
    le2->clear();
    le3->setText("1");
    check1->setChecked(1);
    check2->setChecked(1);
    initok = 0;
}

void Widget::PresetChange(int para) {
    curIn1 = preset[para][0];
    curIn2 = preset[para][1];
    curIn3 = preset[para][2];
    curIn4 = preset[para][3];
    init_data();
}

QVector<QString> Widget::Gacha() {
    QVector<QString> r_role;
    for (int t = 0; t < total; t++) {
        if (pad5 == 89) {
            SSR(r_role);
        }
        int ran = rng->bounded(10000) + 1;
        int pr5 = 60, pr3 = 9490;
        if (pad5 > 72) pr5 += (pad5 - 72) * 600;
        if (pad4 == 8) pr3 = qMax(pr5, 4390);
        if (pad4 > 8) pr3 = pr5;
        if (ran <= pr5) {
            SSR(r_role);
        }
        else if (ran > pr3) {
            SR(r_role);
        }
        else {
            R(r_role);
        }
    }
    return r_role;
}

void Widget::SSR(QVector<QString>& r_role) {
    total5++;
    ga += pad5;
    pad5 = 0;
    if (!guarantee5) {
        r_role.push_back(role5[curIn1]);
        guarantee5 = 1;
        return;
    }
    int ran = rng->bounded(1, 13);
    if (ran > 6) {
        r_role.push_back(role5[curIn1]);
    }
    else {
        r_role.push_back(role5[ran]);
        guarantee5 = 0;
    }
}

void Widget::SR(QVector<QString>& r_role) {
    total4++, pad4 = 0, pad5++;
    int ran;
    if (!guarantee4) {
        ran = rng->bounded(3) + 1;
        if (ran == 1) r_role.push_back(role4[curIn2]);
        if (ran == 2) r_role.push_back(role4[curIn3]);
        if (ran == 3) r_role.push_back(role4[curIn4]);
        guarantee4 = 1;
        return;
    }
    ran = rng->bounded(2);
    if (ran) {
        guarantee4 = 1;
        ran = rng->bounded(3) + 1;
        if (ran == 1) r_role.push_back(role4[curIn2]);
        if (ran == 2) r_role.push_back(role4[curIn3]);
        if (ran == 3) r_role.push_back(role4[curIn4]);
        return;
    }
    ran = rng->bounded(2);
    guarantee4 = 0;
    if (ran) {
        ran = rng->bounded(weapon4.size());
        r_role.push_back(weapon4[ran]);
        return;
    }
    ran = rng->bounded(role4.size()) + 1;
    while (ran == curIn2 || ran == curIn3 || ran == curIn4) {
        ran = rng->bounded(role4.size()) + 1;
    }
    r_role.push_back(role4[ran]);

}

void Widget::R(QVector<QString>& r_role) {
    total3++, pad4++, pad5++;
    int ran = rng->bounded(weapon3.size());
    r_role.push_back(weapon3[ran]);
}

void Widget::make_file(){
    QDateTime d_time;
    timeStr = d_time.currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    fileName = "./logs/" + timeStr + ".txt";
}

void Widget::Record(int p5, int p4, bool g5, bool g4) {
    make_file();
    QFile file(fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
    QString Write, timeCopy = timeStr;
    timeCopy.replace(10, 1, " ");
    timeCopy.replace(13, 1, ":");
    timeCopy.replace(16, 1, ":");
    Write = "日志创建时间：" + timeCopy + '\n';
    file.write(Write.toLocal8Bit());
    QString gg5 = g5 ? "是" : "否", gg4 = g4 ? "是" : "否";
    Write = "当前五星已垫抽：" + to_string(p5) + "  当前四星已垫抽：" + to_string(p4) + "\n";
    file.write(Write.toLocal8Bit());
    Write = "五星小保底是否存在：" + gg5 + "  四星小保底是否存在：" + gg4 + "\n";
    file.write(Write.toLocal8Bit());
    Write = "每次模拟统计共：" + to_string(total) + "抽\n";
    file.write(Write.toLocal8Bit());
    Write = "共计模拟次数：" + to_string(times) + "次\n";
    file.write(Write.toLocal8Bit());
    Write = "数据结果如下\n";
    file.write(Write.toLocal8Bit());
    Write = "三星共计" + to_string(total3) + "个 四星共计" + to_string(total4) + "个 五星共计" + to_string(total5) + "个\n";
    file.write(Write.toLocal8Bit());
    Write = "当期Up角色数量：" + to_string(um[role5[curIn1]]) + "个\n";
    file.write(Write.toLocal8Bit());
    for (int i = 1; i < 8; i++) {
        double percent = fate_count[i] * 100.0 / times;
        Write = to_string(i - 1) + "命发生概率：" + QString::number(percent, 'f', 2) + "%\n";
        file.write(Write.toLocal8Bit());
    }
    Write = "祈愿结果如下：\n五星角色：";
    file.write(Write.toLocal8Bit());
    Write = role5[curIn1] + "x" + to_string(um[role5[curIn1]]) + " ";
    file.write(Write.toLocal8Bit());
    for (auto& str : role5) {
        if (str == role5[curIn1]) continue;
        if (!um[str]) continue;
        Write = str + "x" + to_string(um[str]) + " ";
        file.write(Write.toLocal8Bit());
    }
    Write = "\n四星角色：";
    file.write(Write.toLocal8Bit());
    Write = role4[curIn2] + "x" + to_string(um[role4[curIn2]]) + " ";
    Write += role4[curIn3] + "x" + to_string(um[role4[curIn3]]) + " ";
    Write += role4[curIn4] + "x" + to_string(um[role4[curIn4]]) + " ";
    file.write(Write.toLocal8Bit());
    for (auto& str : role4) {
        if (!um[str]) continue;
        if (str == role4[curIn2] || str == role4[curIn3] || str == role4[curIn4]) continue;
        Write = str + "x" + to_string(um[str]) + " ";
        file.write(Write.toLocal8Bit());
    }
    Write = "\n四星武器：";
    file.write(Write.toLocal8Bit());
    for (auto& str : weapon4) {
        if (!um[str]) continue;
        Write = str + "x" + to_string(um[str]) + " ";
        file.write(Write.toLocal8Bit());
    }
    Write = "\n三星武器：";
    file.write(Write.toLocal8Bit());
    for (auto& str : weapon3) {
        if (!um[str]) continue;
        Write = str + "x" + to_string(um[str]) + " ";
        file.write(Write.toLocal8Bit());
    }
    double percent = total5 * 100.0 / times / total;
    Write = "\n模拟中,五星出率为：" + QString::number(percent, 'f', 2) + "%";
    file.write(Write.toLocal8Bit());
    percent = total4 * 100.0 / times / total;
    Write = "  四星出率为：" + QString :: number(percent, 'f', 2) + "%";
    file.write(Write.toLocal8Bit());
    percent = total3 * 100.0 / times / total;
    Write = "  三星出率为：" + QString :: number(percent, 'f', 2) + "%";
    file.write(Write.toLocal8Bit());
    percent = um[role5[curIn1]] * 100.0 / times / total;
    Write = "\n当期Up五星出率为：" + QString :: number(percent, 'f', 2) + "%";
    file.write(Write.toLocal8Bit());
    percent = um[role5[curIn1]] * 100.0 / total5;
    Write = "  ,占五星中的：" + QString::number(percent, 'f', 2) + "%";
    file.write(Write.toLocal8Bit());
    percent = ga * 1.0 / total5;
    Write = "\n获得五星平均需要" + QString::number(percent, 'f', 2) + "抽";
    file.write(Write.toLocal8Bit());
    file.close();
}

QString Widget::to_string(int num) {
    QString ret = QString::number(num);
    return ret;
}

Widget::~Widget() {}
