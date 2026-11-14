#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QAbstractButton>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QStringList>
#include <QVector>
#include <QHash>

class QVBoxLayout;

class ComboOption : public QWidget {
    Q_OBJECT
public:
    explicit ComboOption(const QString& text, QWidget* parent = nullptr);
    void setSelected(bool on);
    void setTileMode(bool on);
signals:
    void picked();
protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
private:
    void paintTile(QPainter& painter, const QRectF& box);
    void paintRow(QPainter& painter, const QRectF& box);
    QString m_text;
    bool m_selected = false;
    bool m_hover = false;
    bool m_tile = false;
};

class ComboSectionHeader : public QWidget {
    Q_OBJECT
public:
    explicit ComboSectionHeader(const QString& text, const QColor& accent, QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QString m_text;
    QColor m_accent;
};

class ComboCard : public QFrame {
    Q_OBJECT
public:
    explicit ComboCard(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
};

class ComboPopupFrame : public QWidget {
    Q_OBJECT
public:
    explicit ComboPopupFrame(QWidget* parent = nullptr);
    void setCardWidget(QWidget* card) { m_card = card; }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QWidget* m_card = nullptr;
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
    void addGroup(const QString& groupName, const QStringList& texts);
    void clear();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool isPopupOpen() const { return m_popup && m_popup->isVisible(); }
    QString groupOf(int index) const;
    int groupCount() const { return m_groups.size(); }
    QStringList groupNames() const;
    QStringList groupItems(int groupIndex) const;

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
    struct Group {
        QString name;
        QVector<int> indices;
    };

    void openPopup();
    void closePopup();
    void rebuildRows();
    bool isPopupChild(QObject* obj) const;

    QStringList m_items;
    QVector<Group> m_groups;
    QHash<QString, int> m_indexByText;
    int m_current = -1;
    bool m_hover = false;
    QPointer<QWidget> m_popup;
    QWidget* m_popupContent = nullptr;
    QVBoxLayout* m_popupLayout = nullptr;
    QVector<ComboOption*> m_options;
    QVector<QWidget*> m_popupWidgets;
};

#endif
