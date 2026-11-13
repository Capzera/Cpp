#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QElapsedTimer>
#include <windows.h>
#include <vector>
#include <chrono>
#include <atomic>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct InputEvent {
    enum Type { MOUSE_MOVE = 0, MOUSE_BUTTON, KEYBOARD } type;
    DWORD data;
    POINT pt;
    DWORD delay_ms;
    bool  isKeyDown;
    bool  isExtended;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private slots:
    void on_btnRecord_clicked();
    void on_btnStop_clicked();
    void on_btnPlayback_clicked();
    void on_btnRefresh_clicked();
    void on_btnSettings_clicked();
    void pollHotkeys();

private:
    Ui::Widget *ui;

    static std::vector<InputEvent> g_events;
    static HHOOK g_hMouse;
    static HHOOK g_hKeyboard;
    static std::chrono::steady_clock::time_point g_startTime;
    static std::atomic<bool> g_recording;
    static std::atomic<bool> g_recordStarting;
    static std::atomic<bool> g_playing;
    static std::atomic<bool> g_playbackStarting;
    static std::atomic<bool> g_stopPlayback;
    static std::atomic<bool> g_settingsDialogOpen;

    static std::atomic<bool> g_hotkeyStop;
    static std::atomic<bool> g_hotkeyAbort;

    static std::atomic<int> g_vkRecord;
    static std::atomic<int> g_vkStop;
    static std::atomic<int> g_vkPlayback;
    static std::atomic<int> g_vkAbort;

    static POINT g_lastRecordedPoint;
    static bool  g_hasLastPoint;

    QElapsedTimer m_startupTimer;

    bool lastRecordDown   = false;
    bool lastPlaybackDown = false;

    static DWORD GetElapsedMs();

    void installHooks();
    void uninstallHooks();
    void startPlayback();
    bool waitIntervalWithAbort(int seconds);
    void saveRecordingAs(const QString& path);
    bool loadRecordingFrom(const QString& path);
    void refreshRecordList(const QString& selectPath = QString());
    QString recordsDir() const;
    QString currentRecordPath() const;
    void setButtonsEnabled(bool enabled);

    void loadSettings();
    void saveSettings();
    void updateHotkeyLabels();

    static bool WaitUntilQpc(LONGLONG targetTicks);
};

#endif