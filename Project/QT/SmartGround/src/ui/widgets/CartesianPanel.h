#ifndef CARTESIANPANEL_H
#define CARTESIANPANEL_H
#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QDialog>

class CartesianEngine;
class CartesianPointItem;
class CartesianSegmentItem;
class CartesianCurveItem;
class MyTextInput;
class MyButton;
class QLabel;
class MyToggleSwitch;
class MySlider;
class MySegmentedControl;
class MyColorPicker;
class QScrollArea;

class CartesianPanel : public QWidget {
    Q_OBJECT
public:
    explicit CartesianPanel(QWidget* parent = nullptr);
    ~CartesianPanel() override;

    void setEngine(CartesianEngine* engine);
    CartesianEngine* engine() const { return m_engine; }

    void refresh();
    QWidget* buildGuideCard(QWidget* parent);

signals:
    void pointSelected(int index);
    void segmentSelected(int index);
    void functionRemoveRequested(const QString& name);
    void pointEdited(int index, const QString& name, double x, double y, bool nameHidden);
    void pointStyleEdited(int index, const QColor& color, qreal size, int shape, bool nameHidden);
    void functionStyleEdited(const QString& name, const QColor& color);
    void statusMessage(const QString& text);

private:
    QWidget* buildPointRow(CartesianPointItem* point, int index);
    QWidget* buildSegmentRow(CartesianSegmentItem* segment, int index);
    QWidget* buildFunctionRow(CartesianCurveItem* curve, int index);
    void rebuildList();
    void showPointMenu(CartesianPointItem* point, int index, const QPoint& globalPos);
    void showPointStyleEditor(CartesianPointItem* point, int index, const QPoint& globalPos);
    void showSegmentMenu(CartesianSegmentItem* segment, int index, const QPoint& globalPos);
    void showFunctionMenu(CartesianCurveItem* curve, int index, const QPoint& globalPos);
    void showFunctionStyleEditor(CartesianCurveItem* curve, int index, const QPoint& globalPos);
    void editPointCoordinates(CartesianPointItem* point, int index);
    bool eventFilter(QObject* obj, QEvent* event) override;

    CartesianEngine* m_engine = nullptr;
    QWidget* m_listHost = nullptr;
    QScrollArea* m_listScroll = nullptr;
    QVector<QWidget*> m_rows;
    bool m_refreshing = false;
};

class CartesianPointEditor : public QDialog {
    Q_OBJECT
public:
    explicit CartesianPointEditor(QWidget* parent = nullptr);

    void setPointData(const QString& name, double x, double y, bool nameHidden);
    QString pointName() const;
    double pointX() const;
    double pointY() const;
    bool nameHidden() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    MyTextInput* m_nameInput = nullptr;
    MyTextInput* m_xInput = nullptr;
    MyTextInput* m_yInput = nullptr;
    MyToggleSwitch* m_hideSwitch = nullptr;
};
#endif
