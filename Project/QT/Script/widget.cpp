#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QRegularExpression>
#include <QEventLoop>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QKeySequenceEdit>
#include <QKeySequence>
#include <QSet>

#include <timeapi.h>

std::vector<InputEvent> Widget::g_events;
HHOOK Widget::g_hMouse = nullptr;
HHOOK Widget::g_hKeyboard = nullptr;
std::chrono::steady_clock::time_point Widget::g_startTime;
std::atomic<bool> Widget::g_recording{false};
std::atomic<bool> Widget::g_recordStarting{false};
std::atomic<bool> Widget::g_playing{false};
std::atomic<bool> Widget::g_playbackStarting{false};
std::atomic<bool> Widget::g_stopPlayback{false};
std::atomic<bool> Widget::g_settingsDialogOpen{false};
std::atomic<bool> Widget::g_hotkeyStop{false};
std::atomic<bool> Widget::g_hotkeyAbort{false};
std::atomic<int>  Widget::g_vkRecord{VK_F9};
std::atomic<int>  Widget::g_vkStop{VK_F10};
std::atomic<int>  Widget::g_vkPlayback{VK_F11};
std::atomic<int>  Widget::g_vkAbort{VK_F12};
POINT Widget::g_lastRecordedPoint = {0, 0};
bool Widget::g_hasLastPoint = false;

static int qtKeyToVk(int qtKey)
{
    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F24) {
        return VK_F1 + (qtKey - Qt::Key_F1);
    }
    if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z) {
        return 'A' + (qtKey - Qt::Key_A);
    }
    if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9) {
        return '0' + (qtKey - Qt::Key_0);
    }
    switch (qtKey) {
    case Qt::Key_Escape:    return VK_ESCAPE;
    case Qt::Key_Tab:       return VK_TAB;
    case Qt::Key_Backspace: return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:     return VK_RETURN;
    case Qt::Key_Insert:    return VK_INSERT;
    case Qt::Key_Delete:    return VK_DELETE;
    case Qt::Key_Home:      return VK_HOME;
    case Qt::Key_End:       return VK_END;
    case Qt::Key_PageUp:    return VK_PRIOR;
    case Qt::Key_PageDown:  return VK_NEXT;
    case Qt::Key_Left:      return VK_LEFT;
    case Qt::Key_Right:     return VK_RIGHT;
    case Qt::Key_Up:        return VK_UP;
    case Qt::Key_Down:      return VK_DOWN;
    case Qt::Key_Space:     return VK_SPACE;
    default:                return 0;
    }
}

static QKeySequence vkToKeySequence(int vk)
{
    if (vk >= VK_F1 && vk <= VK_F24) {
        return QKeySequence(Qt::Key_F1 + (vk - VK_F1));
    }
    if (vk >= 'A' && vk <= 'Z') {
        return QKeySequence(vk);
    }
    if (vk >= '0' && vk <= '9') {
        return QKeySequence(vk);
    }
    switch (vk) {
    case VK_ESCAPE:    return QKeySequence(Qt::Key_Escape);
    case VK_TAB:       return QKeySequence(Qt::Key_Tab);
    case VK_BACK:      return QKeySequence(Qt::Key_Backspace);
    case VK_RETURN:    return QKeySequence(Qt::Key_Return);
    case VK_INSERT:    return QKeySequence(Qt::Key_Insert);
    case VK_DELETE:    return QKeySequence(Qt::Key_Delete);
    case VK_HOME:      return QKeySequence(Qt::Key_Home);
    case VK_END:       return QKeySequence(Qt::Key_End);
    case VK_PRIOR:     return QKeySequence(Qt::Key_PageUp);
    case VK_NEXT:      return QKeySequence(Qt::Key_PageDown);
    case VK_LEFT:      return QKeySequence(Qt::Key_Left);
    case VK_RIGHT:     return QKeySequence(Qt::Key_Right);
    case VK_UP:        return QKeySequence(Qt::Key_Up);
    case VK_DOWN:      return QKeySequence(Qt::Key_Down);
    case VK_SPACE:     return QKeySequence(Qt::Key_Space);
    default:           return QKeySequence();
    }
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Capzera 终极无敌操作复刻机 V1.5.2");

    timeBeginPeriod(1);

    loadSettings();
    updateHotkeyLabels();

    lastRecordDown   = (GetAsyncKeyState(g_vkRecord.load())  & 0x8000) != 0;
    lastPlaybackDown = (GetAsyncKeyState(g_vkPlayback.load()) & 0x8000) != 0;

    m_startupTimer.start();

    QTimer* poll = new QTimer(this);
    connect(poll, &QTimer::timeout, this, &Widget::pollHotkeys);
    poll->start(10);

    refreshRecordList();

    if (ui->comboRecordFiles->count() > 0) {
        ui->labelStatus->setText(
            QString("已加载录像列表，共 %1 个文件。").arg(ui->comboRecordFiles->count()));
    } else {
        ui->labelStatus->setText("就绪。按底部按钮或快捷键开始使用。");
    }
}

Widget::~Widget()
{
    uninstallHooks();
    timeEndPeriod(1);
    delete ui;
}

DWORD Widget::GetElapsedMs()
{
    auto now = std::chrono::steady_clock::now();
    return (DWORD)std::chrono::duration_cast<std::chrono::milliseconds>(
               now - g_startTime).count();
}

void Widget::loadSettings()
{
    QSettings s("Capzera", "InfinitodeRecorder");
    g_vkRecord.store(s.value("hotkey/record",   VK_F9).toInt());
    g_vkStop.store(  s.value("hotkey/stop",     VK_F10).toInt());
    g_vkPlayback.store(s.value("hotkey/playback", VK_F11).toInt());
    g_vkAbort.store( s.value("hotkey/abort",    VK_F12).toInt());
}

void Widget::saveSettings()
{
    QSettings s("Capzera", "InfinitodeRecorder");
    s.setValue("hotkey/record",   g_vkRecord.load());
    s.setValue("hotkey/stop",     g_vkStop.load());
    s.setValue("hotkey/playback", g_vkPlayback.load());
    s.setValue("hotkey/abort",    g_vkAbort.load());
}

void Widget::updateHotkeyLabels()
{
    ui->btnRecord->setText(
        QString("开始录制（%1）").arg(vkToKeySequence(g_vkRecord.load()).toString(QKeySequence::NativeText)));
    ui->btnStop->setText(
        QString("停止录制（%1）").arg(vkToKeySequence(g_vkStop.load()).toString(QKeySequence::NativeText)));
    ui->btnPlayback->setText(
        QString("开始回放（%1）").arg(vkToKeySequence(g_vkPlayback.load()).toString(QKeySequence::NativeText)));
}

void Widget::on_btnSettings_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("快捷键设置");
    dlg.setMinimumWidth(420);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dlg);

    QLabel* tip = new QLabel(
        "点击输入框后按下想要绑定的按键（推荐 F1~F12）。\n"
        "不支持组合键，四个快捷键不能重复。", &dlg);
    tip->setWordWrap(true);
    mainLayout->addWidget(tip);

    QFormLayout* form = new QFormLayout();

    auto makeEdit = [&](int vk) -> QKeySequenceEdit* {
        QKeySequenceEdit* e = new QKeySequenceEdit(&dlg);
        e->setMaximumSequenceLength(1);
        e->setKeySequence(vkToKeySequence(vk));
        return e;
    };

    QKeySequenceEdit* eRec   = makeEdit(g_vkRecord.load());
    QKeySequenceEdit* eStop  = makeEdit(g_vkStop.load());
    QKeySequenceEdit* ePlay  = makeEdit(g_vkPlayback.load());
    QKeySequenceEdit* eAbort = makeEdit(g_vkAbort.load());

    form->addRow("开始录制：", eRec);
    form->addRow("停止录制：", eStop);
    form->addRow("开始回放：", ePlay);
    form->addRow("中止回放：", eAbort);
    mainLayout->addLayout(form);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnReset  = new QPushButton("恢复默认", &dlg);
    QPushButton* btnOk     = new QPushButton("确定", &dlg);
    QPushButton* btnCancel = new QPushButton("取消", &dlg);
    btnLayout->addWidget(btnReset);
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    QObject::connect(btnReset, &QPushButton::clicked, [&]() {
        eRec->setKeySequence(QKeySequence(Qt::Key_F9));
        eStop->setKeySequence(QKeySequence(Qt::Key_F10));
        ePlay->setKeySequence(QKeySequence(Qt::Key_F11));
        eAbort->setKeySequence(QKeySequence(Qt::Key_F12));
    });
    QObject::connect(btnOk,     &QPushButton::clicked, &dlg, &QDialog::accept);
    QObject::connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    g_settingsDialogOpen.store(true);
    int dlgResult = dlg.exec();
    g_settingsDialogOpen.store(false);

    lastRecordDown   = (GetAsyncKeyState(g_vkRecord.load())  & 0x8000) != 0;
    lastPlaybackDown = (GetAsyncKeyState(g_vkPlayback.load()) & 0x8000) != 0;

    if (dlgResult != QDialog::Accepted) return;

    auto getVk = [](QKeySequenceEdit* e, int* outVk) -> bool {
        QKeySequence seq = e->keySequence();
        if (seq.isEmpty() || seq.count() != 1) return false;
        QKeyCombination comb = seq[0];
        if (comb.keyboardModifiers() != Qt::NoModifier) return false;
        int vk = qtKeyToVk(comb.key());
        if (vk == 0) return false;
        *outVk = vk;
        return true;
    };

    int vkRec = 0, vkStop = 0, vkPlay = 0, vkAbort = 0;
    if (!getVk(eRec, &vkRec) || !getVk(eStop, &vkStop) ||
        !getVk(ePlay, &vkPlay) || !getVk(eAbort, &vkAbort)) {
        QMessageBox::warning(this, "设置失败",
                             "存在不支持或组合的按键。\n"
                             "请使用单键（推荐 F1~F12、字母、数字、方向键等）。");
        return;
    }

    QSet<int> unique{vkRec, vkStop, vkPlay, vkAbort};
    if (unique.size() != 4) {
        QMessageBox::warning(this, "设置失败", "四个快捷键不能重复。");
        return;
    }

    g_vkRecord.store(vkRec);
    g_vkStop.store(vkStop);
    g_vkPlayback.store(vkPlay);
    g_vkAbort.store(vkAbort);
    saveSettings();
    updateHotkeyLabels();

    lastRecordDown   = (GetAsyncKeyState(g_vkRecord.load())  & 0x8000) != 0;
    lastPlaybackDown = (GetAsyncKeyState(g_vkPlayback.load()) & 0x8000) != 0;

    ui->labelStatus->setText("快捷键已更新并保存。");
}

QString Widget::recordsDir() const
{
    QString dir = QCoreApplication::applicationDirPath() + "/records";
    QDir().mkpath(dir);
    return dir;
}

QString Widget::currentRecordPath() const
{
    if (ui->comboRecordFiles->count() == 0) return QString();
    return ui->comboRecordFiles->currentData().toString();
}

void Widget::refreshRecordList(const QString& selectPath)
{
    ui->comboRecordFiles->blockSignals(true);
    ui->comboRecordFiles->clear();

    QDir dir(recordsDir());
    QStringList files = dir.entryList(QStringList() << "*.bin", QDir::Files, QDir::Name);
    for (const QString& f : files) {
        QString full = dir.absoluteFilePath(f);
        QString displayName = QFileInfo(f).completeBaseName();
        ui->comboRecordFiles->addItem(displayName, full);
    }

    if (ui->comboRecordFiles->count() > 0) {
        int targetIdx = 0;
        if (!selectPath.isEmpty()) {
            QString absSel = QFileInfo(selectPath).absoluteFilePath();
            for (int i = 0; i < ui->comboRecordFiles->count(); ++i) {
                QString cur = QFileInfo(ui->comboRecordFiles->itemData(i).toString())
                .absoluteFilePath();
                if (cur == absSel) {
                    targetIdx = i;
                    break;
                }
            }
        }
        ui->comboRecordFiles->setCurrentIndex(targetIdx);
    }

    ui->comboRecordFiles->blockSignals(false);
}

void Widget::installHooks()
{
    HINSTANCE hInst = GetModuleHandle(nullptr);
    g_hMouse    = SetWindowsHookEx(WH_MOUSE_LL,    MouseProc,    hInst, 0);
    g_hKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);

    if (!g_hMouse || !g_hKeyboard) {
        qDebug() << "钩子安装失败，请以管理员身份运行！";
        QMessageBox::warning(this, "错误",
                             "全局钩子安装失败。\n请右键程序 -> 以管理员身份运行。");
    }
}

void Widget::uninstallHooks()
{
    if (g_hMouse)    { UnhookWindowsHookEx(g_hMouse);    g_hMouse = nullptr; }
    if (g_hKeyboard) { UnhookWindowsHookEx(g_hKeyboard); g_hKeyboard = nullptr; }
}

LRESULT CALLBACK Widget::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && g_recording.load()) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;
        InputEvent evt{};
        evt.pt       = pMouse->pt;
        evt.delay_ms = GetElapsedMs();

        if (wParam == WM_MOUSEMOVE) {
            if (!g_hasLastPoint ||
                abs(pMouse->pt.x - g_lastRecordedPoint.x) > 2 ||
                abs(pMouse->pt.y - g_lastRecordedPoint.y) > 2) {
                evt.type = InputEvent::MOUSE_MOVE;
                g_events.push_back(evt);
                g_lastRecordedPoint = pMouse->pt;
                g_hasLastPoint = true;
            }
        } else if (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP ||
                   wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP) {
            evt.type      = InputEvent::MOUSE_BUTTON;
            evt.isKeyDown = (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN);
            evt.data      = (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP)
                           ? MOUSEEVENTF_LEFTDOWN
                           : MOUSEEVENTF_RIGHTDOWN;
            g_events.push_back(evt);
            g_lastRecordedPoint = pMouse->pt;
            g_hasLastPoint = true;
        }
    }
    return CallNextHookEx(g_hMouse, nCode, wParam, lParam);
}

LRESULT CALLBACK Widget::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (g_recording.load() || g_recordStarting.load())) {
        KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
        int vkRec   = g_vkRecord.load();
        int vkStop  = g_vkStop.load();
        int vkPlay  = g_vkPlayback.load();
        int vkAbort = g_vkAbort.load();

        if (pKey->vkCode == vkRec || pKey->vkCode == vkStop ||
            pKey->vkCode == vkPlay || pKey->vkCode == vkAbort) {
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
                if (pKey->vkCode == vkStop)  g_hotkeyStop.store(true);
                if (pKey->vkCode == vkAbort) g_hotkeyAbort.store(true);
            }
            return 1;
        }

        if (g_recording.load()) {
            InputEvent evt{};
            evt.type       = InputEvent::KEYBOARD;
            evt.data       = pKey->vkCode;
            evt.isKeyDown  = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            evt.isExtended = (pKey->flags & LLKHF_EXTENDED) != 0;
            evt.delay_ms   = GetElapsedMs();
            g_events.push_back(evt);
        }
    }
    return CallNextHookEx(g_hKeyboard, nCode, wParam, lParam);
}

void Widget::pollHotkeys()
{
    int vkRec   = g_vkRecord.load();
    int vkPlay  = g_vkPlayback.load();
    int vkAbort = g_vkAbort.load();

    if (g_settingsDialogOpen.load()) {
        lastRecordDown   = (GetAsyncKeyState(vkRec)  & 0x8000) != 0;
        lastPlaybackDown = (GetAsyncKeyState(vkPlay) & 0x8000) != 0;
        g_hotkeyStop.exchange(false);
        g_hotkeyAbort.exchange(false);
        return;
    }

    bool startupGrace = m_startupTimer.elapsed() < 500;

    bool idle = !g_recording.load() && !g_recordStarting.load()
                && !g_playing.load()   && !g_playbackStarting.load();

    if (idle) {
        bool recPhys = (GetAsyncKeyState(vkRec)  & 0x8000) != 0;
        bool plyPhys = (GetAsyncKeyState(vkPlay) & 0x8000) != 0;
        bool recEdge = recPhys && !lastRecordDown && !startupGrace;
        bool plyEdge = plyPhys && !lastPlaybackDown && !startupGrace;
        lastRecordDown   = recPhys;
        lastPlaybackDown = plyPhys;

        if (recEdge) { on_btnRecord_clicked();   return; }
        if (plyEdge) { on_btnPlayback_clicked(); return; }
    } else {
        lastRecordDown   = (GetAsyncKeyState(vkRec)  & 0x8000) != 0;
        lastPlaybackDown = (GetAsyncKeyState(vkPlay) & 0x8000) != 0;
    }

    bool stop  = g_hotkeyStop.exchange(false);
    bool abort = g_hotkeyAbort.exchange(false);

    if (!abort && (g_playing.load() || g_playbackStarting.load())) {
        if (GetAsyncKeyState(vkAbort) & 0x0001) abort = true;
    }

    if (stop && (g_recording.load() || g_recordStarting.load())) {
        on_btnStop_clicked();
    }
    if (abort && (g_playing.load() || g_playbackStarting.load())) {
        g_stopPlayback.store(true);
    }
}

void Widget::setButtonsEnabled(bool enabled)
{
    ui->btnRecord->setEnabled(enabled);
    ui->btnStop->setEnabled(enabled);
    ui->btnPlayback->setEnabled(enabled);
    ui->btnRefresh->setEnabled(enabled);
    ui->btnSettings->setEnabled(enabled);
    ui->comboRecordFiles->setEnabled(enabled);
    ui->spinLoopCount->setEnabled(enabled);
    ui->spinIntervalSec->setEnabled(enabled);
}

void Widget::on_btnRecord_clicked()
{
    if (g_recording.load() || g_recordStarting.load() ||
        g_playing.load()   || g_playbackStarting.load()) return;

    g_events.clear();
    g_events.reserve(200000);
    g_hasLastPoint = false;
    g_hotkeyStop.store(false);
    g_hotkeyAbort.store(false);
    g_recordStarting.store(true);

    installHooks();
    setButtonsEnabled(false);
    ui->btnStop->setEnabled(true);

    ui->labelStatus->setText("3 秒后开始录制，请立即切换游戏窗口……\n（按停止键取消）");

    QTimer::singleShot(3000, this, [this]() {
        if (!g_recordStarting.load()) return;
        g_recordStarting.store(false);
        g_startTime = std::chrono::steady_clock::now();
        g_recording.store(true);
        ui->labelStatus->setText("录制中…… 按停止键结束并保存。");
    });
}

void Widget::on_btnStop_clicked()
{
    if (g_recordStarting.load()) {
        g_recordStarting.store(false);
        uninstallHooks();
        g_events.clear();
        setButtonsEnabled(true);
        ui->labelStatus->setText("录制已取消（倒计时期间按停止键）。");
        return;
    }

    if (!g_recording.load() && g_events.empty()) return;

    g_recording.store(false);
    uninstallHooks();
    setButtonsEnabled(true);

    if (g_events.empty()) {
        ui->labelStatus->setText("录制结束，但没有捕获到任何事件。");
        return;
    }

    QString defaultName = "record_" +
                          QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

    QInputDialog dlg(this);
    dlg.setWindowTitle("保存录像");
    dlg.setLabelText("请输入录像名称：");
    dlg.setTextValue(defaultName);
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);

    if (dlg.exec() != QDialog::Accepted) {
        g_events.clear();
        ui->labelStatus->setText("已取消保存，本次录像已丢弃。");
        return;
    }

    QString name = dlg.textValue().trimmed();
    if (name.isEmpty()) {
        g_events.clear();
        ui->labelStatus->setText("名称为空，本次录像已丢弃。");
        return;
    }

    name.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_");
    QString path = recordsDir() + "/" + name + ".bin";

    if (QFile::exists(path)) {
        auto ans = QMessageBox::question(this, "覆盖确认",
                                         "文件已存在：\n" + path + "\n\n是否覆盖？",
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ans != QMessageBox::Yes) {
            g_events.clear();
            ui->labelStatus->setText("已取消保存（同名文件存在），本次录像已丢弃。");
            return;
        }
    }

    saveRecordingAs(path);
    refreshRecordList(path);

    ui->labelStatus->setText(
        QString("录制结束，共 %1 个事件，已保存：\n%2")
            .arg(g_events.size())
            .arg(path));
}

void Widget::on_btnRefresh_clicked()
{
    QString cur = currentRecordPath();
    refreshRecordList(cur);
    ui->labelStatus->setText(
        QString("已刷新录像列表，共 %1 个文件。")
            .arg(ui->comboRecordFiles->count()));
}

void Widget::saveRecordingAs(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "错误",
                             "无法写入录像文件：\n" + path + "\n" + f.errorString());
        return;
    }

    quint32 version = 2;
    quint32 count   = (quint32)g_events.size();
    f.write((const char*)&version, sizeof(version));
    f.write((const char*)&count,   sizeof(count));
    if (count > 0) {
        f.write((const char*)g_events.data(), (qint64)count * sizeof(InputEvent));
    }
    f.close();
    qDebug() << "录像已保存：" << path << "事件数：" << count;
}

bool Widget::loadRecordingFrom(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "提示",
                                 "无法打开录像文件：\n" + path);
        return false;
    }

    quint32 version = 0, count = 0;
    if (f.read((char*)&version, sizeof(version)) != sizeof(version) ||
        f.read((char*)&count,   sizeof(count))   != sizeof(count)   ||
        version != 2 || count == 0) {
        QMessageBox::warning(this, "错误",
                             "录像文件无效或版本过旧：\n" + path);
        return false;
    }

    g_events.resize(count);
    qint64 need = (qint64)count * sizeof(InputEvent);
    qint64 got  = f.read((char*)g_events.data(), need);
    f.close();

    if (got != need) {
        g_events.clear();
        QMessageBox::warning(this, "错误", "录像文件读取不完整。");
        return false;
    }
    qDebug() << "录像已加载：" << path << "事件数：" << count;
    return true;
}

bool Widget::WaitUntilQpc(LONGLONG targetTicks)
{
    LARGE_INTEGER freq, now;
    QueryPerformanceFrequency(&freq);
    int vkAbort = g_vkAbort.load();

    while (true) {
        QueryPerformanceCounter(&now);
        LONGLONG remainTicks = targetTicks - now.QuadPart;
        if (remainTicks <= 0) break;

        DWORD remainMs = (DWORD)(remainTicks * 1000 / freq.QuadPart);

        if (remainMs > 50) {
            QThread::msleep(20);
        } else if (remainMs > 2) {
            QThread::msleep(remainMs - 2);
        } else {
            QThread::usleep(200);
        }

        if (GetAsyncKeyState(vkAbort) & 0x8001) {
            g_stopPlayback.store(true);
            return false;
        }
    }
    return true;
}

void Widget::startPlayback()
{
    if (g_events.empty()) return;

    const int screenW = GetSystemMetrics(SM_CXSCREEN);
    const int screenH = GetSystemMetrics(SM_CYSCREEN);

    LARGE_INTEGER freq, startQpc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&startQpc);

    for (const auto& evt : g_events) {
        LONGLONG targetTicks = startQpc.QuadPart
                               + (LONGLONG)evt.delay_ms * freq.QuadPart / 1000;
        if (!WaitUntilQpc(targetTicks)) {
            return;
        }

        INPUT input{};
        if (evt.type == InputEvent::MOUSE_MOVE) {
            input.type       = INPUT_MOUSE;
            input.mi.dx      = (LONG)((LONGLONG)evt.pt.x * 65535 / screenW);
            input.mi.dy      = (LONG)((LONGLONG)evt.pt.y * 65535 / screenH);
            input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
            SendInput(1, &input, sizeof(INPUT));
        }
        else if (evt.type == InputEvent::MOUSE_BUTTON) {
            input.type = INPUT_MOUSE;
            if (evt.data == MOUSEEVENTF_LEFTDOWN) {
                input.mi.dwFlags = evt.isKeyDown ? MOUSEEVENTF_LEFTDOWN
                                                 : MOUSEEVENTF_LEFTUP;
            } else {
                input.mi.dwFlags = evt.isKeyDown ? MOUSEEVENTF_RIGHTDOWN
                                                 : MOUSEEVENTF_RIGHTUP;
            }
            SendInput(1, &input, sizeof(INPUT));
        }
        else if (evt.type == InputEvent::KEYBOARD) {
            input.type       = INPUT_KEYBOARD;
            input.ki.wVk     = (WORD)evt.data;
            input.ki.wScan   = (WORD)MapVirtualKey(evt.data, MAPVK_VK_TO_VSC);
            input.ki.dwFlags = (evt.isKeyDown ? 0 : KEYEVENTF_KEYUP)
                               | (evt.isExtended ? KEYEVENTF_EXTENDEDKEY : 0);
            SendInput(1, &input, sizeof(INPUT));
        }
    }
}

bool Widget::waitIntervalWithAbort(int seconds)
{
    int vkAbort = g_vkAbort.load();

    if (seconds <= 0) {
        if (GetAsyncKeyState(vkAbort) & 0x8001) {
            g_stopPlayback.store(true);
            return false;
        }
        return true;
    }

    LARGE_INTEGER freq, start, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    LONGLONG targetTicks = start.QuadPart + (LONGLONG)seconds * freq.QuadPart;

    while (true) {
        QueryPerformanceCounter(&now);
        if (now.QuadPart >= targetTicks) return true;

        QThread::msleep(20);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        if (GetAsyncKeyState(vkAbort) & 0x8001) {
            g_stopPlayback.store(true);
            return false;
        }
    }
}

void Widget::on_btnPlayback_clicked()
{
    if (g_recording.load() || g_recordStarting.load() ||
        g_playing.load()   || g_playbackStarting.load()) return;

    QString path = currentRecordPath();
    if (path.isEmpty()) {
        QMessageBox::information(this, "提示",
                                 "当前没有可用的录像文件，请先录制一次。");
        return;
    }
    if (!loadRecordingFrom(path)) return;

    g_stopPlayback.store(false);
    g_hotkeyAbort.store(false);
    g_playbackStarting.store(true);

    const int loopCount   = ui->spinLoopCount->value();
    const int intervalSec = ui->spinIntervalSec->value();

    setButtonsEnabled(false);
    ui->labelStatus->setText(
        QString("3 秒后开始回放，请立即切到游戏窗口……\n"
                "文件：%1\n"
                "循环次数：%2（0 = 无限），间隔：%3 秒。")
            .arg(QFileInfo(path).fileName())
            .arg(loopCount)
            .arg(intervalSec));

    QTimer::singleShot(3000, this, [this, loopCount, intervalSec]() {
        if (!g_playbackStarting.load()) {
            setButtonsEnabled(true);
            return;
        }
        g_playbackStarting.store(false);

        if (g_stopPlayback.load()) {
            setButtonsEnabled(true);
            ui->labelStatus->setText("回放已在倒计时期间被取消。");
            return;
        }

        g_playing.store(true);

        int played = 0;
        while (true) {
            if (g_stopPlayback.load()) break;

            startPlayback();
            played++;

            if (g_stopPlayback.load()) break;
            if (loopCount > 0 && played >= loopCount) break;
            if (!waitIntervalWithAbort(intervalSec)) break;
        }

        g_playing.store(false);
        setButtonsEnabled(true);

        if (g_stopPlayback.load()) {
            ui->labelStatus->setText(
                QString("回放已被中止（已完成 %1 次）。").arg(played));
        } else {
            ui->labelStatus->setText(
                QString("回放完成，共 %1 次。").arg(played));
        }
    });
}