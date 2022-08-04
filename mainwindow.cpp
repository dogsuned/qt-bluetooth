#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QMenu>
#include <QDebug>
#include <service.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), localDevice(new QBluetoothLocalDevice)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(8000);
    // discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    // connect(ui->inquiryType, SIGNAL(toggled(bool)), this, SLOT(setGeneralUnlimited(bool)));
    connect(ui->scan, SIGNAL(clicked()), this, SLOT(startScan()));

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
        this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    connect(ui->list, SIGNAL(itemActivated(QListWidgetItem*)),
        this, SLOT(itemActivated(QListWidgetItem*)));

    // connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
    //     this, SLOT(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));

    // hostModeStateChanged(localDevice->hostMode());
    // add context menu for devices to be able to pair device
    ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayPairingMenu(QPoint)));
    connect(localDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing))
        , this, SLOT(pairingDone(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    ui->logWindow->document()->setMaximumBlockCount(500);
}

MainWindow::~MainWindow()
{
    delete discoveryAgent;
    delete ui;
}

void MainWindow::startScan()
{
    ui->scan->setEnabled(false);
    ui->list->clear();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        if (info.name().toUpper().indexOf(info.address().toString()) < 0) {
            QString label = QString("%1 (%2)").arg(info.name()).arg(info.address().toString());
            QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);
            if (items.empty()) {
                QListWidgetItem *item = new QListWidgetItem(label);
                //    QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
                //    if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
                //        item->setForeground(QColor(Qt::green));
                //    else
                //        item->setForeground(QColor(Qt::black));
                log("find devices " + info.name() + "(" + info.address().toString() + ")");
                ui->list->addItem(item);
            }
        }
    }
}

void MainWindow::scanFinished()
{
   ui->scan->setEnabled(true);
}

void MainWindow::log(const QString &text)
{
    if (ui->logWindow->textColor() != QColor(160, 0, 0)) {
        ui->logWindow->setTextColor(QColor(160, 0, 0));
    }
    ui->logWindow->append(text);
}

void MainWindow::itemActivated(QListWidgetItem *item)
{
    QString text = item->text();

    int l (text.indexOf('(')), r(text.indexOf(')'));
    if (l < 0 || r < 0 || r <= l) {
        return;
    }

    QBluetoothAddress address (text.mid(l + 1, r - l - 1));
    QString name(text.left(text.indexOf(' ')));

    ServiceDiscoveryDialog d(name, address);
    d.exec();
}

void MainWindow::displayPairingMenu(const QPoint &pos)
{
    if (ui->list->count() == 0)
        return;
    QMenu menu(this);
    QAction *pairAction = menu.addAction("Pair");
    QAction *removePairAction = menu.addAction("Remove Pairing");
    QAction *chosenAction = menu.exec(ui->list->viewport()->mapToGlobal(pos));
    QListWidgetItem *currentItem = ui->list->currentItem();

    int l (currentItem->text().indexOf('(')), r(currentItem->text().indexOf(')'));
    if (l < 0 || r < 0 || r <= l) {
        return;
    }

    QBluetoothAddress address (currentItem->text().mid(l + 1, r - l - 1));
    if (chosenAction == pairAction) {
        qDebug("request pair");
        localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
    } else if (chosenAction == removePairAction) {
        qDebug("remove pair");
        localDevice->requestPairing(address, QBluetoothLocalDevice::Unpaired);
    }
}

void MainWindow::pairingDone(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    QList<QListWidgetItem *> items = ui->list->findItems(address.toString(), Qt::MatchContains);

    if (pairing == QBluetoothLocalDevice::Paired || pairing == QBluetoothLocalDevice::AuthorizedPaired ) {
        for (int var = 0; var < items.count(); ++var) {
            QListWidgetItem *item = items.at(var);
            item->setForeground(QColor(Qt::green));
        }
    } else {
        for (int var = 0; var < items.count(); ++var) {
            QListWidgetItem *item = items.at(var);
            item->setForeground(QColor(Qt::red));
        }
    }
}
