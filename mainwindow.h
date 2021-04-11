#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QSystemTrayIcon>
#include "windows.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QMap<QCheckBox*, Qt::Key> checkBoxKeysMapping;
    QList<Qt::Key> keysToEmulate;

    void registerHotkey();
    void unregisterHotkey();
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void emulateKey(Qt::Key key);
    void onCapsLock();
    void updateKeysToEmulate();

private slots:
    void onTick();
};
#endif // MAINWINDOW_H
