#ifndef ANNOTATIONEDITOR_H
#define ANNOTATIONEDITOR_H
#include <QDialog>
#include <QString>

class QPlainTextEdit;
class QLabel;
class QTextBrowser;
class QPushButton;

class AnnotationEditor : public QDialog {
    Q_OBJECT
public:
    enum Mode { TextMode, MarkdownMode, LatexMode };
    explicit AnnotationEditor(Mode mode, const QString& initial, QWidget* parent = nullptr);

    QString content() const;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void buildUi();
    void applyStyle();

    Mode m_mode;
    QPlainTextEdit* m_editor = nullptr;
    QLabel* m_hint = nullptr;
    QTextBrowser* m_preview = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif
