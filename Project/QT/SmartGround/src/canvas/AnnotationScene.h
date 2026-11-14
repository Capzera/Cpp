#ifndef ANNOTATIONSCENE_H
#define ANNOTATIONSCENE_H
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QHash>

class AnnotationItem;
class AnnotationScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit AnnotationScene(QObject* parent = nullptr);
    ~AnnotationScene() override;

    void addItem(AnnotationItem* item);
    void removeItem(AnnotationItem* item, bool destroy = true);
    void destroyItem(AnnotationItem* item);

    void setHostView(QGraphicsView* view);
    QGraphicsView* hostView() const { return m_hostView; }

    QVector<AnnotationItem*> annotationItems() const;
    AnnotationItem* itemByUid(const QString& uid) const;

    bool annotationsVisible() const { return m_visible; }
    void setAnnotationsVisible(bool on);
    bool annotationsLocked() const { return m_locked; }
    void setAnnotationsLocked(bool on);

    void clearAnnotations();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    void stashTo(QString bucket);
    bool restoreFrom(QString bucket);

signals:
    void itemsChanged();
    void visibilityChanged(bool visible);
    void lockChanged(bool locked);

private:
    bool m_visible = true;
    bool m_locked = false;
    QVector<AnnotationItem*> m_owned;
    QGraphicsView* m_hostView = nullptr;
    QHash<QString, QJsonObject> m_stash;
};

#endif
