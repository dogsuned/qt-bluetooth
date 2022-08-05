#include "service.h"

#include <qbluetoothaddress.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <QLowEnergyController>

ServiceDiscoveryDialog::ServiceDiscoveryDialog(const QString &name,
                                               const QBluetoothAddress &address, QWidget *parent)
:   QDialog(parent), ui(new Ui_ServiceDiscovery)
{
    ui->setupUi(this);

    //Using default Bluetooth adapter
    QBluetoothLocalDevice localDevice;
    QBluetoothAddress adapterAddress = localDevice.address();

    /*
     * In case of multiple Bluetooth adapters it is possible to
     * set which adapter will be used by providing MAC Address.
     * Example code:
     *
     * QBluetoothAddress adapterAddress("XX:XX:XX:XX:XX:XX");
     * discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);
     */

//    discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

//    discoveryAgent->setRemoteAddress(address);

    setWindowTitle(name);

//    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
//            this, SLOT(addService(QBluetoothServiceInfo)));
//    connect(discoveryAgent, SIGNAL(finished()), ui->status, SLOT(hide()));

//    discoveryAgent->start();
    qDebug() << address.toString();
    QLowEnergyController *m_control = QLowEnergyController::createCentral(address, adapterAddress, this);
    connect(m_control, &QLowEnergyController::serviceDiscovered,
        this, &ServiceDiscoveryDialog::serviceDiscovered);
    connect(m_control, &QLowEnergyController::discoveryFinished,
        this, &ServiceDiscoveryDialog::discoveryFinished);
    connect(m_control, &QLowEnergyController::connected, this, &ServiceDiscoveryDialog::connected);
    connect(m_control, &QLowEnergyController::disconnected, this, &ServiceDiscoveryDialog::disconnected);
    m_control->connectToDevice();
//    m_control->discoverServices();
}

void ServiceDiscoveryDialog::serviceDiscovered(const QBluetoothUuid &gatt)
{
//    if (gatt == QBluetoothUuid(QBluetoothUuid::HeartRate)) {
//        setInfo("Heart Rate service discovered. Waiting for service scan to be done...");
//        m_foundHeartRateService = true;
//    }
    qDebug() << "found service";
    qDebug() << gatt;
}

void ServiceDiscoveryDialog::discoveryFinished(void)
{
    qDebug("discoveryFinished");
}

void ServiceDiscoveryDialog::connected(void)
{
    qDebug("connected");
}

void ServiceDiscoveryDialog::disconnected(void)
{
    qDebug("disconnected");
}

ServiceDiscoveryDialog::~ServiceDiscoveryDialog()
{
    delete discoveryAgent;
    delete ui;
}

void ServiceDiscoveryDialog::addService(const QBluetoothServiceInfo &info)
{
    if (info.serviceName().isEmpty())
        return;

    QString line = info.serviceName();
    if (!info.serviceDescription().isEmpty())
        line.append("\n\t" + info.serviceDescription());
    if (!info.serviceProvider().isEmpty())
        line.append("\n\t" + info.serviceProvider());

    ui->list->addItem(line);
}
