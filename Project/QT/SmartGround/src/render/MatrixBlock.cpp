#include "MatrixBlock.h"
#include <QPainter>
#include <QFontMetrics>
#include <QFont>

MatrixBlock::MatrixBlock(int r, int c, QGraphicsItem* parent) : QGraphicsObject(parent) {
    rows = qMax(1, r);
    cols = qMax(1, c);
    data.resize(rows);
    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            data[i][j] = QString::number(i * cols + j + 1);
        }
    }
    setZValue(1);
}

void MatrixBlock::setTitle(const QString& text) {
    title = text;
    update();
}

void MatrixBlock::setCell(int r, int c, const QString& text) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    data[r][c] = text;
    update();
}

QString MatrixBlock::cellAt(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return QString();
    return data[r][c];
}

void MatrixBlock::fillRandom(int seed) {
    int state = qAbs(seed) + 1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            state = (state * 1103515245 + 12345) & 0x7fffffff;
            data[i][j] = QString::number(state % 90 + 10);
        }
    }
    update();
}

double MatrixBlock::headerHeight() const {
    return title.isEmpty() ? 0.0 : 24.0;
}

QRectF MatrixBlock::boundingRect() const {
    double h = headerHeight();
    return QRectF(-2, -2 - h, cols * cell + 4, rows * cell + h + 4);
}

void MatrixBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    double h = headerHeight();
    if (!title.isEmpty()) {
        QFont f("Microsoft YaHei UI", 9, QFont::Bold);
        painter->setFont(f);
        painter->setPen(QColor(40, 70, 110));
        painter->drawText(QRectF(0, -h, cols * cell, h), Qt::AlignLeft | Qt::AlignVCenter, title);
    }
    painter->setFont(QFont("Consolas", 12, QFont::Bold));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QRectF r(j * cell, i * cell, cell, cell);
            painter->setBrush(cellColor);
            painter->setPen(QPen(gridColor, 1.4));
            painter->drawRect(r);
            painter->setPen(QColor(35, 45, 60));
            painter->drawText(r, Qt::AlignCenter, data[i][j]);
        }
    }
}
