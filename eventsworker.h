// eventsworker.h
#include <QThread>
#include "events.h"

class EventsWorker : public QThread {
    Q_OBJECT
public:
    EventsWorker(QObject *parent = nullptr) : QThread(parent) {}

protected:
    void run() override {
        events.run();
    }

private:
    Events events;
};
