// main.cpp
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QPalette>
#include <QFont>
#include <QMessageBox>

class CountdownWidget : public QWidget {
    Q_OBJECT
public:
    explicit CountdownWidget(QWidget *parent = nullptr)
        : QWidget(parent), totalSeconds(0), remainingSeconds(0), isRunning(false) {
        setWindowTitle("课堂倒计时");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 时间显示标签
        timeLabel = new QLabel("HH:MM:SS", this);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setFont(QFont("Microsoft YaHei", 144));
        QFont font = timeLabel->font();
        font.setBold(true);
        timeLabel->setFont(font);
        timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(timeLabel);

        // 控制区：输入框 + 按钮
        QHBoxLayout *ctrlLayout = new QHBoxLayout();

        // 小时输入
        hourInput = new QLineEdit("0", this);
        hourInput->setFont(QFont("Microsoft YaHei", 20));
        hourInput->setPlaceholderText("时");
        hourInput->setFixedWidth(80);
        hourInput->setAlignment(Qt::AlignCenter);
        ctrlLayout->addWidget(hourInput);

        // 分钟输入
        minuteInput = new QLineEdit("0", this);
        minuteInput->setFont(QFont("Microsoft YaHei", 20));
        minuteInput->setPlaceholderText("分");
        minuteInput->setFixedWidth(80);
        minuteInput->setAlignment(Qt::AlignCenter);
        ctrlLayout->addWidget(minuteInput);

        // 秒输入
        secondInput = new QLineEdit("0", this);
        secondInput->setFont(QFont("Microsoft YaHei", 20));
        secondInput->setPlaceholderText("秒");
        secondInput->setFixedWidth(80);
        secondInput->setAlignment(Qt::AlignCenter);
        ctrlLayout->addWidget(secondInput);

        // 按钮
        startPauseBtn = new QPushButton("开始", this);
        startPauseBtn->setFont(QFont("Microsoft YaHei", 20));
        ctrlLayout->addWidget(startPauseBtn);

        resetBtn = new QPushButton("重置", this);
        resetBtn->setFont(QFont("Microsoft YaHei", 20));
        ctrlLayout->addWidget(resetBtn);

        fullScreenBtn = new QPushButton("全屏", this);
        fullScreenBtn->setFont(QFont("Microsoft YaHei", 20));
        ctrlLayout->addWidget(fullScreenBtn);

        mainLayout->addLayout(ctrlLayout);
        resize(1024, 768);

        // 定时器
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &CountdownWidget::onTimer);

        connect(startPauseBtn, &QPushButton::clicked, this, &CountdownWidget::onStartPause);
        connect(resetBtn, &QPushButton::clicked, this, &CountdownWidget::onReset);
        connect(fullScreenBtn, &QPushButton::clicked, this, [this]() {
            if (isFullScreen()) {
                showNormal();
                fullScreenBtn->setText("全屏");
            } else {
                showFullScreen();
                fullScreenBtn->setText("窗口");
            }
        });

        updateDisplay();
        updateBackground(1.0);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        // 根据窗口高度调整时间字体大小
        int h = timeLabel->height();
        int fontSize = h / 2.5;
        if (fontSize < 20) fontSize = 20;
        QFont font = timeLabel->font();
        font.setPixelSize(fontSize);
        timeLabel->setFont(font);
    }

private slots:
    void onStartPause() {
        if (!isRunning) {
            if (remainingSeconds == 0) {
                // 时间已到，需要重新读取输入时间
                int secs = parseInputToSeconds();
                if (secs <= 0) {
                    QMessageBox::warning(this, "错误", "请输入有效的时间（总秒数 > 0）");
                    return;
                }
                totalSeconds = secs;
                remainingSeconds = totalSeconds;
            }
            timer->start();
            isRunning = true;
            startPauseBtn->setText("暂停");
            setInputEnabled(false);
        } else {
            timer->stop();
            isRunning = false;
            startPauseBtn->setText("继续");
        }
    }

    void onReset() {
        timer->stop();
        isRunning = false;
        startPauseBtn->setText("开始");
        setInputEnabled(true);

        // 解析当前输入作为新时间
        int secs = parseInputToSeconds();
        if (secs > 0) {
            totalSeconds = secs;
        } else if (totalSeconds <= 0) {
            // 输入无效且没有合法总时间，回退到默认5分钟
            totalSeconds = 0;
            hourInput->setText("0"), minuteInput->setText("0"), secondInput->setText("0");
        }
        remainingSeconds = totalSeconds;
        updateDisplay();
        updateBackground(1.0);
    }

    void onTimer() {
        if (remainingSeconds > 0) {
            --remainingSeconds;
            updateDisplay();
            double ratio = static_cast<double>(remainingSeconds) / totalSeconds;
            updateBackground(ratio);
            if (remainingSeconds == 0) {
                timer->stop();
                isRunning = false;
                startPauseBtn->setText("开始");
                setInputEnabled(true);
                updateBackground(0.0);
                QApplication::beep();
                QTimer::singleShot(200, this, [this]() {
                    updateBackground(0.0);
                });
            }
        }
    }

private:
    int parseInputToSeconds() {
        bool okH, okM, okS;
        int h = hourInput->text().toInt(&okH);
        int m = minuteInput->text().toInt(&okM);
        int s = secondInput->text().toInt(&okS);
        if (!okH || !okM || !okS || h < 0 || m < 0 || s < 0) return -1;
        int total = h * 3600 + m * 60 + s;
        return total;
    }

    void setInputEnabled(bool enabled) {
        hourInput->setEnabled(enabled);
        minuteInput->setEnabled(enabled);
        secondInput->setEnabled(enabled);
    }

    void updateDisplay() {
        int hrs = remainingSeconds / 3600;
        int mins = (remainingSeconds % 3600) / 60;
        int secs = remainingSeconds % 60;
        timeLabel->setText(QString("%1:%2:%3")
                               .arg(hrs, 2, 10, QChar('0'))
                               .arg(mins, 2, 10, QChar('0'))
                               .arg(secs, 2, 10, QChar('0')));
    }

    void updateBackground(double ratio) {
        int red   = static_cast<int>(245 * (1.0 - ratio));
        int green = static_cast<int>(230 * ratio);
        int blue  = static_cast<int>(45 - 15 * ratio);
        QColor bgColor(red, green, blue);

        int lum = 0.299 * red + 0.587 * green + 0.114 * blue;
        QColor textColor(240, 240, 240);
        QString style = QString("background-color: %1; color: %2;").arg(bgColor.name(), textColor.name());
        setStyleSheet(style);
    }

private:
    QLabel *timeLabel;
    QLineEdit *hourInput, *minuteInput, *secondInput;
    QPushButton *startPauseBtn, *resetBtn, *fullScreenBtn;
    QTimer *timer;
    int totalSeconds, remainingSeconds;
    bool isRunning;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CountdownWidget window;
    window.show();
    return app.exec();
}

#include "main.moc"