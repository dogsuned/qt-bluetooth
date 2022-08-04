#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qbluetoothlocaldevice.h>
#include <qlistwidget.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void log(const QString &text);

public slots:
    void startScan();
    void addDevice(const QBluetoothDeviceInfo&);
    void scanFinished();
    void displayPairingMenu(const QPoint &pos);
    void pairingDone(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing);
    // void setGeneralUnlimited(bool unlimited);
    void itemActivated(QListWidgetItem *item);
    // void hostModeStateChanged(QBluetoothLocalDevice::HostMode);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
