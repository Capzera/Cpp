#ifndef CAPZERADIALOG_H
#define CAPZERADIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

// 🚀 全局高定弹窗引擎：彻底替换老旧的 QMessageBox
class CapzeraDialog : public QDialog {
    Q_OBJECT
public:
    enum ButtonType { Ok, YesNo };
    
    // 提供四个静态方法，完美对标原生的 information, warning, critical, question
    static void information(QWidget *parent, const QString &title, const QString &text);
    static void warning(QWidget *parent, const QString &title, const QString &text);
    static void critical(QWidget *parent, const QString &title, const QString &text);
    static bool question(QWidget *parent, const QString &title, const QString &text);

private:
    explicit CapzeraDialog(const QString &title, const QString &text, const QString &typeColor, const QString &iconText, ButtonType btnType, QWidget *parent = nullptr);
    
    // 支持无边框窗口的鼠标拖拽
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    bool m_result;
    QPoint m_dragPosition;
};

#endif // CAPZERADIALOG_H