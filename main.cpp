#include <QApplication>
#include <QThread>
#include <QCoreApplication>
#include "eventsworker.h"
#include "skeydmanager.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    EventsWorker worker;
    worker.start();
    SkeydManager manager;
    QThread workerThread;


    manager.moveToThread(&workerThread);


    QObject::connect(&workerThread, &QThread::started, &manager, &SkeydManager::run);

    QObject::connect(&manager, &SkeydManager::keyLogged, [](const QString &k1, const QString &k2){
        qDebug() << "Key logged:" << k1 << k2;
    });

    QObject::connect(&manager, &SkeydManager::errorOccurred, [](const QString &msg){
        qWarning() << "Error:" << msg;
    });

    QObject::connect(&workerThread, &QThread::finished, &manager, &QObject::deleteLater);

    workerThread.start();

    int ret = app.exec();

    workerThread.quit();
    workerThread.wait();

    return ret;
}
