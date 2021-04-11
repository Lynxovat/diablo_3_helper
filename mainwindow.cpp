#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    registerHotkey();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTick()));

    checkBoxKeysMapping[ui->key1cb] = Qt::Key_1;
    checkBoxKeysMapping[ui->key2cb] = Qt::Key_2;
    checkBoxKeysMapping[ui->key3cb] = Qt::Key_3;
    checkBoxKeysMapping[ui->key4cb] = Qt::Key_4;
}

MainWindow::~MainWindow()
{
    unregisterHotkey();
    delete ui;
    delete timer;
}

void MainWindow::registerHotkey()
{
    RegisterHotKey((HWND)MainWindow::winId(), 100, 0, VK_CAPITAL);
}

void MainWindow::unregisterHotkey()
{
    UnregisterHotKey((HWND)MainWindow::winId(), 100);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    // Transform the message pointer to the MSG WinAPI
    MSG* msg = reinterpret_cast<MSG*>(message);

    // If the message is a HotKey, then ...
    if(msg->message == WM_HOTKEY){
        // ... check HotKey
        if(msg->wParam == 100){
            onCapsLock();
            return true;
        }
    }
    return false;
}

void MainWindow::emulateKey(Qt::Key key)
{
#ifdef Q_OS_WIN32
    INPUT ip; //устройство ввода
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = key;

    //посылаем событие нажатия клавиши
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));
    //а затем отжатия клавиши
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
#endif
}

void MainWindow::onCapsLock()
{
    if (timer->isActive()) {
        timer->stop();
    } else {
        timer->start(75);
        updateKeysToEmulate();
    }
    ui->statusbar->showMessage(
                timer->isActive() ? "Active" : "Stopped");
}

void MainWindow::updateKeysToEmulate()
{
    keysToEmulate.clear();
    QMap<QCheckBox*, Qt::Key>::iterator it = checkBoxKeysMapping.begin();
    for (; it != checkBoxKeysMapping.end(); ++it) {
        if (it.key()->isChecked()) {
            keysToEmulate << it.value();
        }
    }
}

void MainWindow::onTick()
{
    QList<Qt::Key>::iterator it = keysToEmulate.begin();
    for (; it != keysToEmulate.end(); ++it) {
        emulateKey(*it);
    }
}

