#ifndef SERVICE_H
#define SERVICE_H

#include "ui_service.h"
#include <qbluetoothuuid.h>
#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QBluetoothAddress)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)

QT_USE_NAMESPACE

class ServiceDiscoveryDialog : public QDialog
{
    Q_OBJECT

public:
    ServiceDiscoveryDialog(const QString &name, const QBluetoothAddress &address, QWidget *parent = nullptr);
    ~ServiceDiscoveryDialog();

public slots:
    void addService(const QBluetoothServiceInfo&);
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void discoveryFinished(void);
    void connected(void);
    void disconnected(void);

private:
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    Ui_ServiceDiscovery *ui;
};

#endif // SERVICE_H
