void ForwardStarModule::layoutArrays(int nodeCount, int edgeCapacity) {
    auto addLabel = [&](const QString& text, double x, double y, const QColor& color) {
        QGraphicsTextItem* label = m_scene->addText(text);
        label->setFont(UiFonts::mono(13, true));
        label->setDefaultTextColor(color);
        label->setPos(x, y);
    };

    const double rowWidth = std::max(nodeCount, edgeCapacity) * kFsCellW;
    m_arraysX = 250.0;
    const double titleX = m_arraysX - 104;

    addLabel("head[u]", titleX, kFsTopY + kFsCellH / 2.0 - 10, QColor(45, 130, 210));
    addLabel("to[k]", titleX, kFsTopY + kFsRowGap + kFsCellH / 2.0 - 10, QColor(70, 165, 195));
    addLabel("nxt[k]", titleX, kFsTopY + 2 * kFsRowGap + kFsCellH / 2.0 - 10, QColor(160, 110, 220));
    addLabel("weight[k]", titleX, kFsTopY + 3 * kFsRowGap + kFsCellH / 2.0 - 10, QColor(200, 130, 70));

    auto addIndexRow = [&](double rowY, int count) {
        for (int i = 1; i <= count; ++i) {
            QGraphicsTextItem* sub = m_scene->addText(QString::number(i));
            sub->setFont(UiFonts::mono(11));
            sub->setDefaultTextColor(QColor(120, 134, 155));
            sub->setPos(m_arraysX + (i - 1) * kFsCellW + kFsCellW / 2.0 - 5, rowY - 22);
        }
    };

    for (int u = 1; u <= nodeCount; ++u) {
        ArrayBlock* block = new ArrayBlock(u, 0, QString::number(u));
        block->setCellSize(kFsCellW, kFsCellH);
        block->setShowLabel(false);
        block->setPos(m_arraysX + (u - 1) * kFsCellW, kFsTopY);
        block->color = kFsPale;
        m_scene->addItem(block);
        m_headBlocks[u] = block;
    }
    addIndexRow(kFsTopY, nodeCount);

    for (int k = 1; k <= edgeCapacity; ++k) {
        ArrayBlock* to = new ArrayBlock(k, 0, QString::number(k));
        to->setCellSize(kFsCellW, kFsCellH);
        to->setShowLabel(false);
        to->setPos(m_arraysX + (k - 1) * kFsCellW, kFsTopY + kFsRowGap);
        to->color = kFsPale;
        m_scene->addItem(to);
        m_toBlocks[k] = to;

        ArrayBlock* nxt = new ArrayBlock(k, 0, QString::number(k));
        nxt->setCellSize(kFsCellW, kFsCellH);
        nxt->setShowLabel(false);
        nxt->setPos(m_arraysX + (k - 1) * kFsCellW, kFsTopY + 2 * kFsRowGap);
        nxt->color = kFsPale;
        m_scene->addItem(nxt);
        m_nxtBlocks[k] = nxt;

        ArrayBlock* weight = new ArrayBlock(k, 0, QString::number(k));
        weight->setCellSize(kFsCellW, kFsCellH);
        weight->setShowLabel(false);
        weight->setPos(m_arraysX + (k - 1) * kFsCellW, kFsTopY + 3 * kFsRowGap);
        weight->color = kFsPale;
        m_scene->addItem(weight);
        m_weightBlocks[k] = weight;
    }
    addIndexRow(kFsTopY + kFsRowGap, edgeCapacity);
    addIndexRow(kFsTopY + 2 * kFsRowGap, edgeCapacity);
    addIndexRow(kFsTopY + 3 * kFsRowGap, edgeCapacity);

    m_arraysBottomY = kFsTopY + 3 * kFsRowGap + kFsCellH;
    Q_UNUSED(rowWidth);
}