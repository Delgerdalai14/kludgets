#ifndef KLUDGETSERVER_H
#define KLUDGETSERVER_H

#include <QProcess>
#include <QStringList>
#include <QtGui>
#include <QSignalMapper>

#include "kludgetinfo.h"
#include "kipc.h"

class KSettings;
class PreferenceWindow;
class AboutKludgetWindow;
class HotKey;
class QNetworkReply;

void BringWindowToTop(int winId);

class KServer : public QObject
{
    Q_OBJECT

public:

    class Widget
    {
    public:
        QString id;
        QString name;
        QString path;
        int pid;
        bool active;
        bool enabled;
    };

    KServer();

    static KServer* instance();

    bool initialize();
    void shutdown();

    QList<Widget>* widgets() { updateWidgetList(); return &widgetList; }

    void showHUD();
    void hideHUD();

private:

    void sendMessageToAll(KIPC::Message msg);

    void loadDefaultWidgets();
    void setupMenu();
    void updateSystemSettings();

    void updateWidgetList();
    void updateWidgetListPID();

    private
Q_SLOTS:
    void checkUpdate();
    void hotKeyPressed(Qt::Key, Qt::KeyboardModifier);
    void processWidgetQueue();
    void runWidget(const QString &path);
    void openPackage();
    void showMenu();
    void showWidget(int);
    void showAllWidgets();
    void uninstallWidgets();
    void configure();
    void aboutKludget();
    void goToWidgetsSite();
    void onSettingsChanged();
    void onAboutClosed();
    void onPreferencesClosed();
    void onCheckDone(QNetworkReply *reply);
    void exit();

Q_SIGNALS:
    void selectWidget(int);
    void selectInstalledWidget(QString);

private:
    QList<QWidget*> hudScreens;
    QList<Widget> widgetList;
    QStringList widgetQueue;

    QMenu trayMenu;
    QMenu widgetsMenu;
    QSystemTrayIcon trayIcon;
    QSignalMapper runningWidgetsMapper;
    QSignalMapper installedWidgetsMapper;

    PreferenceWindow *prefWindow;
    AboutKludgetWindow *aboutWindow;
    KSettings *settings;

    QSharedMemory *processLock;

    HotKey *hotKeyListener;

    QTimer updateTimer;

    KIPC ipc;
};

#endif // KLUDGETSERVER_H