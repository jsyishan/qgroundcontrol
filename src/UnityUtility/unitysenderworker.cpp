#include "unitysenderworker.h"

UnitySenderWorker::UnitySenderWorker(): order(0) {

    app = qgcApp();

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 23333);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

UnitySenderWorker::~UnitySenderWorker() {
    this->exit();
    this->quit();
    socket->abort();
    socket->close();
    delete socket;
}

void UnitySenderWorker::read() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 port;
    socket->readDatagram(buffer.data(), buffer.size(), &sender, &port);

    qDebug() << "From: " << sender.toString() << ":" << port << "\n" << buffer << "\n";
}


void UnitySenderWorker::run() {
    QByteArray data;
    while (true) {
        data = getVehicleData(1);
        if (data == nullptr) continue;
        socket->writeDatagram(data, QHostAddress::LocalHost, 6666);
        msleep(20);
//        emit resultReady("!");
        order++;
    }
}

QByteArray UnitySenderWorker::getVehicleData(int id) {
    QByteArray data;
    Vehicle* v = app->toolbox()->multiVehicleManager()->activeVehicle();
//    qDebug() << (v ? "1" : "0");
    if (v == nullptr) return nullptr;

    data.append(v->pitch()->enumOrValueString() + " ");
    data.append(v->heading()->enumOrValueString() + " ");   // Yaw
    data.append(v->roll()->enumOrValueString() + " ");
    data.append(v->getFactGroup("gps")->getFact("lon")->enumOrValueString() + " ");
    data.append(v->altitudeRelative()->enumOrValueString() + " ");
    data.append(v->getFactGroup("gps")->getFact("lat")->enumOrValueString() + " ");
    data.append(QString::number(order));
    return data;
}
