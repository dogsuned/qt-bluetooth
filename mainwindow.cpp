#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), localDevice(new QBluetoothLocalDevice)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    // connect(ui->inquiryType, SIGNAL(toggled(bool)), this, SLOT(setGeneralUnlimited(bool)));
    connect(ui->scan, SIGNAL(clicked()), this, SLOT(startScan()));

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    // connect(ui->list, SIGNAL(itemActivated(QListWidgetItem*)),
    //         this, SLOT(itemActivated(QListWidgetItem*)));

    // connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
    //         this, SLOT(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));

    // hostModeStateChanged(localDevice->hostMode());
    // // add context menu for devices to be able to pair device
    // ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayPairingMenu(QPoint)));
    // connect(localDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing))
    //     , this, SLOT(pairingDone(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    ui->logWindow->document()->setMaximumBlockCount(500);
}

MainWindow::~MainWindow()
{
    delete discoveryAgent;
    delete ui;
}

void MainWindow::startScan()
{
   discoveryAgent->start();
   ui->scan->setEnabled(false);
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &info)
{
   QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
   QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);
   if (items.empty()) {
       QListWidgetItem *item = new QListWidgetItem(label);
    //    QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
    //    if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
    //        item->setForeground(QColor(Qt::green));
    //    else
    //        item->setForeground(QColor(Qt::black));
        log("find devices " + info.address().toString() + info.name());
       ui->list->addItem(item);
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
