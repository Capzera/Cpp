#ifndef ANNOTATIONEDITOR_H
#define ANNOTATIONEDITOR_H
#include <QDialog>
#include <QString>

class QPlainTextEdit;
class QLabel;
class QTextBrowser;
class QPushButton;
class QSvgRenderer;

class AnnotationEditor : public QDialog {
    Q_OBJECT
public:
    enum Mode { TextMode, MarkdownMode, LatexMode };
    explicit AnnotationEditor(Mode mode, const QString& initial, QWidget* parent = nullptr);

    QString content() const;
    void setLiveMode(bool on);
    bool isLiveMode() const { return m_live; }

signals:
    void contentChanged(const QString& text);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void buildUi();
    void applyStyle();
    void applyMarkdownPreviewStyle();
    void refreshLatexPreview();
    void renderLatexPreview(const QString& tex, const QString& svg, double w, double h);

    Mode m_mode;
    bool m_live = false;
    QPlainTextEdit* m_editor = nullptr;
    QLabel* m_hint = nullptr;
    QTextBrowser* m_preview = nullptr;
    QPushButton* m_okButton = nullptr;
    QLabel* m_latexPreview = nullptr;
    QSvgRenderer* m_latexRenderer = nullptr;
    QString m_latexPending;
};

#endif
