#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QStringList>
#include <cmath>

class GridTradingApp : public QWidget {
public:
    GridTradingApp(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("量化网格交易策略生成器");
        resize(600, 500);

        // 1. 初始化 UI 组件
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        inputBasePrice = new QDoubleSpinBox();
        inputBasePrice->setRange(0.01, 10000.0);
        inputBasePrice->setDecimals(3);
        inputBasePrice->setValue(1.164); // 默认参考 159995.SZ

        inputTotalCapital = new QDoubleSpinBox();
        inputTotalCapital->setRange(100.0, 10000000.0);
        inputTotalCapital->setValue(8000.0);

        inputGridSteps = new QSpinBox();
        inputGridSteps->setRange(1, 20);
        inputGridSteps->setValue(2);

        inputDropPct = new QDoubleSpinBox();
        inputDropPct->setRange(0.1, 50.0);
        inputDropPct->setValue(4.0); // 默认跌 4% 买一档

        inputProfitPct = new QDoubleSpinBox();
        inputProfitPct->setRange(0.1, 100.0);
        inputProfitPct->setValue(5.0); // 默认涨 5% 卖出

        formLayout->addRow("基准价格 (元):", inputBasePrice);
        formLayout->addRow("总投资额 (元):", inputTotalCapital);
        formLayout->addRow("网格加仓档数:", inputGridSteps);
        formLayout->addRow("每档下跌买入 (%):", inputDropPct);
        formLayout->addRow("网格反弹止盈 (%):", inputProfitPct);

        btnGenerate = new QPushButton("生成策略网格", this);
        btnGenerate->setMinimumHeight(40);

        tableResult = new QTableWidget();
        tableResult->setColumnCount(5);
        tableResult->setHorizontalHeaderLabels({"操作类型", "触发价格 (元)", "交易数量 (股)", "动用资金 (元)", "策略说明"});
        tableResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableResult->setEditTriggers(QAbstractItemView::NoEditTriggers);

        mainLayout->addLayout(formLayout);
        mainLayout->addWidget(btnGenerate);
        mainLayout->addWidget(tableResult);

        // 2. 绑定计算逻辑 (使用 Lambda 表达式)
        connect(btnGenerate, &QPushButton::clicked, this, [this]() {
            calculateGrid();
        });

        // 初始化计算一次
        calculateGrid();
    }

private:
    QDoubleSpinBox *inputBasePrice;
    QDoubleSpinBox *inputTotalCapital;
    QSpinBox *inputGridSteps;
    QDoubleSpinBox *inputDropPct;
    QDoubleSpinBox *inputProfitPct;
    QPushButton *btnGenerate;
    QTableWidget *tableResult;

    void addTableRow(const QString &action, double price, int shares, double cost, const QString &note) {
        int row = tableResult->rowCount();
        tableResult->insertRow(row);

        tableResult->setItem(row, 0, new QTableWidgetItem(action));
        tableResult->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 3)));

        if (shares > 0) {
            tableResult->setItem(row, 2, new QTableWidgetItem(QString::number(shares)));
        } else {
            tableResult->setItem(row, 2, new QTableWidgetItem("-"));
        }

        if (cost > 0) {
            tableResult->setItem(row, 3, new QTableWidgetItem(QString::number(cost, 'f', 2)));
        } else {
            tableResult->setItem(row, 3, new QTableWidgetItem("-"));
        }

        tableResult->setItem(row, 4, new QTableWidgetItem(note));
    }

    void calculateGrid() {
        // 清空表格
        tableResult->setRowCount(0);

        double basePrice = inputBasePrice->value();
        double totalCapital = inputTotalCapital->value();
        int steps = inputGridSteps->value();
        double dropRate = inputDropPct->value() / 100.0;
        double profitRate = inputProfitPct->value() / 100.0;

        // 策略设定：40%资金用于底仓，剩余60%均分给各个网格
        double baseCapital = totalCapital * 0.4;
        double gridCapital = (totalCapital - baseCapital) / steps;

        // 1. 计算止盈目标 (基于底仓)
        double sellPrice = basePrice * (1.0 + profitRate);
        addTableRow("网格止盈", sellPrice, 0, 0, "触及此价格，卖出部分底仓锁定利润");

        // 2. 计算底仓
        // A股规则：必须是100的整数倍
        int baseShares = std::floor((baseCapital / basePrice) / 100.0) * 100;
        double actualBaseCost = baseShares * basePrice;
        addTableRow("建立底仓", basePrice, baseShares, actualBaseCost, "占用约40%规划资金");

        // 3. 计算网格买入档位
        for (int i = 1; i <= steps; ++i) {
            double buyPrice = basePrice * (1.0 - i * dropRate);
            int buyShares = std::floor((gridCapital / buyPrice) / 100.0) * 100;
            double actualCost = buyShares * buyPrice;

            QString actionName = QString("网格买入 %1").arg(i);
            QString note = QString("较基准价下跌 %1%").arg(i * inputDropPct->value());

            addTableRow(actionName, buyPrice, buyShares, actualCost, note);
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置高分屏支持和基础样式
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setStyle("Fusion");

    GridTradingApp window;
    window.show();

    return app.exec();
}
