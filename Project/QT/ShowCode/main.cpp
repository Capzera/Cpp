#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QStackedWidget>
#include <QScreen>
#include <QKeyEvent>

class PasswordRevealer : public QWidget
{
    Q_OBJECT

public:
    PasswordRevealer(QWidget *parent = nullptr)
        : QWidget(parent), m_stackedWidget(new QStackedWidget(this))
    {
        setupUi();
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &PasswordRevealer::checkTime);

        // 默认设置为明天同一时间
        m_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    }

protected:
    // 事件过滤器：处理全屏密码窗口的 ESC 按键
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == m_passwordWindow && event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape) {
                if (m_passwordWindow) {
                    m_passwordWindow->hide();
                }
                m_stackedWidget->setCurrentIndex(0);
                return true; // 事件已处理
            }
        }
        return QWidget::eventFilter(obj, event);
    }

private slots:
    void startCountdown()
    {
        m_password = m_passwordEdit->text().trimmed();
        if (m_password.isEmpty()) {
            QMessageBox::warning(this, "警告", "密码不能为空！");
            return;
        }

        m_targetTime = m_dateTimeEdit->dateTime();
        if (m_targetTime <= QDateTime::currentDateTime()) {
            showPassword();
            return;
        }

        m_stackedWidget->setCurrentIndex(1);
        m_timer->start(1000);
        checkTime();
    }

    void checkTime()
    {
        QDateTime now = QDateTime::currentDateTime();
        qint64 secsToTarget = now.secsTo(m_targetTime);

        if (secsToTarget <= 0) {
            m_timer->stop();
            showPassword();
            return;
        }

        int days = secsToTarget / 86400;
        int hours = (secsToTarget % 86400) / 3600;
        int minutes = (secsToTarget % 3600) / 60;
        int seconds = secsToTarget % 60;
        QString countdown = QString("距离显示密码还有：%1天 %2小时 %3分 %4秒")
                                .arg(days)
                                .arg(hours, 2, 10, QChar('0'))
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0'));
        m_countdownLabel->setText(countdown);
    }

    void showPassword()
    {
        if (!m_passwordWindow) {
            m_passwordWindow = new QWidget(nullptr);
            m_passwordWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            m_passwordWindow->setStyleSheet("background-color: black;");
            m_passwordWindow->installEventFilter(this); // 安装事件过滤器以捕获 ESC

            QVBoxLayout *layout = new QVBoxLayout(m_passwordWindow);
            layout->setContentsMargins(50, 50, 50, 50);

            QLabel *titleLabel = new QLabel("解压密码", m_passwordWindow);
            titleLabel->setAlignment(Qt::AlignCenter);
            titleLabel->setStyleSheet("color: white; font-size: 40px;");

            // 密码显示标签（自动换行，每行最多16字符）
            m_displayPasswordLabel = new QLabel(m_passwordWindow);
            m_displayPasswordLabel->setAlignment(Qt::AlignCenter);
            m_displayPasswordLabel->setStyleSheet("color: yellow; font-size: 80px; font-weight: bold;");
            m_displayPasswordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

            // 提示标签
            QLabel *hintLabel = new QLabel("按 ESC 键退出全屏", m_passwordWindow);
            hintLabel->setAlignment(Qt::AlignCenter);
            hintLabel->setStyleSheet("color: gray; font-size: 20px;");

            QPushButton *closeButton = new QPushButton("关闭", m_passwordWindow);
            closeButton->setStyleSheet("font-size: 30px; padding: 10px 30px;");
            connect(closeButton, &QPushButton::clicked, this, [this]() {
                if (m_passwordWindow) {
                    m_passwordWindow->hide();
                }
                m_stackedWidget->setCurrentIndex(0);
            });

            layout->addWidget(titleLabel);
            layout->addWidget(m_displayPasswordLabel);
            layout->addWidget(hintLabel);
            layout->addWidget(closeButton, 0, Qt::AlignCenter);
        }

        // 将密码按每16字符分割，插入换行符
        QString formattedPassword;
        for (int i = 0; i < m_password.length(); i += 16) {
            formattedPassword += m_password.mid(i, 16);
            if (i + 16 < m_password.length())
                formattedPassword += '\n';
        }
        m_displayPasswordLabel->setText(formattedPassword);

        m_passwordWindow->showFullScreen();
        m_passwordWindow->raise();
        m_passwordWindow->activateWindow();
    }

    void cancelCountdown()
    {
        m_timer->stop();
        m_stackedWidget->setCurrentIndex(0);
    }

private:
    void setupUi()
    {
        setWindowTitle("考试密码定时显示");
        resize(500, 300);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(m_stackedWidget);

        // 设置页面（索引0）
        QWidget *setupPage = new QWidget;
        QVBoxLayout *setupLayout = new QVBoxLayout(setupPage);

        QLabel *infoLabel = new QLabel("设置解压密码和显示时间");
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

        QHBoxLayout *passwordLayout = new QHBoxLayout;
        QLabel *passwordLabel = new QLabel("密码:");
        m_passwordEdit = new QLineEdit;
        m_passwordEdit->setPlaceholderText("请输入解压密码");
        passwordLayout->addWidget(passwordLabel);
        passwordLayout->addWidget(m_passwordEdit);

        QHBoxLayout *timeLayout = new QHBoxLayout;
        QLabel *timeLabel = new QLabel("显示时间:");
        m_dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(3600));
        m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        m_dateTimeEdit->setCalendarPopup(true);
        timeLayout->addWidget(timeLabel);
        timeLayout->addWidget(m_dateTimeEdit);

        QPushButton *startButton = new QPushButton("开始倒计时");
        startButton->setStyleSheet("font-size: 16px; padding: 8px;");
        connect(startButton, &QPushButton::clicked, this, &PasswordRevealer::startCountdown);

        setupLayout->addWidget(infoLabel);
        setupLayout->addLayout(passwordLayout);
        setupLayout->addLayout(timeLayout);
        setupLayout->addWidget(startButton);
        setupLayout->addStretch();

        // 等待页面（索引1）
        QWidget *waitPage = new QWidget;
        QVBoxLayout *waitLayout = new QVBoxLayout(waitPage);
        m_countdownLabel = new QLabel;
        m_countdownLabel->setAlignment(Qt::AlignCenter);
        m_countdownLabel->setStyleSheet("font-size: 24px;");
        QPushButton *cancelButton = new QPushButton("取消");
        cancelButton->setStyleSheet("font-size: 16px; padding: 8px 20px;");
        connect(cancelButton, &QPushButton::clicked, this, &PasswordRevealer::cancelCountdown);
        waitLayout->addStretch();
        waitLayout->addWidget(m_countdownLabel);
        waitLayout->addWidget(cancelButton, 0, Qt::AlignCenter);
        waitLayout->addStretch();

        m_stackedWidget->addWidget(setupPage);
        m_stackedWidget->addWidget(waitPage);
        m_stackedWidget->setCurrentIndex(0);
    }

private:
    QStackedWidget *m_stackedWidget;
    QLineEdit *m_passwordEdit;
    QDateTimeEdit *m_dateTimeEdit;
    QLabel *m_countdownLabel;
    QTimer *m_timer;
    QDateTime m_targetTime;
    QString m_password;

    QWidget *m_passwordWindow = nullptr;
    QLabel *m_displayPasswordLabel = nullptr;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PasswordRevealer window;
    window.show();
    return app.exec();
}

#include "main.moc"