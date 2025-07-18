#ifndef SKEYDMANAGER_H
#define SKEYDMANAGER_H

#include <QObject>
#include <QString>

class SkeydManager : public QObject
{
    Q_OBJECT

public:
    explicit SkeydManager(QObject *parent = nullptr);
    void start();

signals:
    void errorOccurred(const QString &message);
    void keyLogged(const QString &key1, const QString &key2);

public slots:
    void run();

};

#endif // SKEYDMANAGER_H
