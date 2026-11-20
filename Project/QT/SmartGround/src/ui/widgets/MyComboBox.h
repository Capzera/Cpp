#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QAbstractButton>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QPropertyAnimation>
#include <QStringList>
#include <QVector>
#include <QHash>

class QVBoxLayout;

enum class ComboVisualStyle {
    VueLight
};

class ComboOption : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)
    Q_PROPERTY(qreal pressProgress READ pressProgress WRITE setPressProgress)
public:
    explicit ComboOption(const QString& text, QWidget* parent = nullptr);
    void setSelected(bool on);
    void setTileMode(bool on);
    void setAccent(const QColor& color);
    void setVisualStyle(ComboVisualStyle style);
    ComboVisualStyle visualStyle() const { return m_style; }
    QString text() const { return m_text; }
    int preferredWidth() const;
    qreal hoverProgress() const { return m_hoverProgress; }
    void setHoverProgress(qreal v);
    qreal pressProgress() const { return m_pressProgress; }
    void setPressProgress(qreal v);
signals:
    void picked();
protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void resizeEvent(QResizeEvent*) override;
private:
    void paintTile(QPainter& painter, const QRectF& box);
    void paintRow(QPainter& painter, const QRectF& box);
    void paintVueTile(QPainter& painter, const QRectF& box);
    void syncAnimations();    QString m_text;
    bool m_selected = false;
    bool m_hover = false;
    bool m_tile = false;
    bool m_pressed = false;
    QColor m_accent;
    ComboVisualStyle m_style = ComboVisualStyle::VueLight;
    qreal m_hoverProgress = 0.0;
    qreal m_pressProgress = 0.0;
    QPropertyAnimation* m_hoverAnim = nullptr;
    QPropertyAnimation* m_pressAnim = nullptr;
};
class ComboSectionHeader : public QWidget {
    Q_OBJECT
public:
    explicit ComboSectionHeader(const QString& text, const QColor& accent, QWidget* parent = nullptr);
    void setVisualStyle(ComboVisualStyle style);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QString m_text;
    QColor m_accent;
    ComboVisualStyle m_style = ComboVisualStyle::VueLight;
};

class ComboCard : public QFrame {
    Q_OBJECT
public:
    explicit ComboCard(QWidget* parent = nullptr);
    void setVisualStyle(ComboVisualStyle style);
    ComboVisualStyle visualStyle() const { return m_style; }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    ComboVisualStyle m_style = ComboVisualStyle::VueLight;
};

class ComboPopupFrame : public QWidget {
    Q_OBJECT
public:
    explicit ComboPopupFrame(QWidget* parent = nullptr);
    void setCardWidget(QWidget* card) { m_card = card; }
    void setVisualStyle(ComboVisualStyle style);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QWidget* m_card = nullptr;
    ComboVisualStyle m_style = ComboVisualStyle::VueLight;
};

class MyComboBox : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(qreal chevronProgress READ chevronProgress WRITE setChevronProgress)
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

    void setVisualStyle(ComboVisualStyle style);
    ComboVisualStyle visualStyle() const { return m_style; }
    static void setGlobalVisualStyle(ComboVisualStyle style);
    static ComboVisualStyle globalVisualStyle();
    static QStringList styleNames();
    static QString styleName(ComboVisualStyle style);
    static int styleIndex(ComboVisualStyle style);

    qreal chevronProgress() const { return m_chevronProgress; }
    void setChevronProgress(qreal v);

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
    void computeColumnWidth();
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
    int m_columnWidth = 180;
    ComboVisualStyle m_style = ComboVisualStyle::VueLight;
    qreal m_chevronProgress = 0.0;
    QPropertyAnimation* m_chevronAnim = nullptr;
};

#endif
