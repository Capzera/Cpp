#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QAbstractButton>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QStringList>
#include <QVector>

class QVBoxLayout;

class ComboRow : public QWidget {
    Q_OBJECT
public:
    explicit ComboRow(const QString& text, QWidget* parent = nullptr);
    void setSelected(bool on);
    void setHovered(bool on);
signals:
    void picked();
protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
private:
    QString m_text;
    bool m_selected = false;
    bool m_hover = false;
};

class ComboCard : public QFrame {
    Q_OBJECT
public:
    explicit ComboCard(QWidget* parent = nullptr) : QFrame(parent) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(box, 12.0, 12.0);
        painter.setPen(QPen(QColor(223, 228, 236), 1));
        painter.setBrush(QColor(255, 255, 255));
        painter.drawPath(path);
    }
};

class MyComboBox : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    explicit MyComboBox(QWidget* parent = nullptr);
    ~MyComboBox();

    int count() const { return m_items.size(); }
    int currentIndex() const { return m_current; }
    void setCurrentIndex(int index);
    QString currentText() const;
    QString itemText(int index) const;
    void addItem(const QString& text);
    void addItems(const QStringList& texts);
    void clear();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool isPopupOpen() const { return m_popup && m_popup->isVisible(); }

signals:
    void currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void openPopup();
    void closePopup();
    void rebuildRows();
    int rowIndexFor(QObject* row) const;

    QStringList m_items;
    int m_current = -1;
    bool m_hover = false;
    QPointer<QWidget> m_popup;
    QWidget* m_popupContent = nullptr;
    QVBoxLayout* m_popupLayout = nullptr;
    QVector<ComboRow*> m_rows;
};

#endif
