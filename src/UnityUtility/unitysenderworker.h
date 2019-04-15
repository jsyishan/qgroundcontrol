#ifndef UNITYSENDERWORKER_H
#define UNITYSENDERWORKER_H

#include <QThread>
#include <QUdpSocket>
#include "QGCApplication.h"
#include "Vehicle.h"

class UnitySenderWorker : public QThread
{
    Q_OBJECT
public:
    UnitySenderWorker();
    ~UnitySenderWorker();
    void run() override;

signals:
    void resultReady(const QString& s);
    void sendData();
public slots:
    void read();

private:
    QUdpSocket* socket;
    QGCApplication* app;

    QByteArray getVehicleData(int);
    int order;
};

#endif // UNITYSENDERWORKER_H
