#include "skeydmanager.h"

SkeydManager::SkeydManager(QObject *parent)
    : QObject(parent)
{
}

void SkeydManager::start()
{
    run();
}

void SkeydManager::run()
{
    // تنفيذ عمليات، ثم إرسال إشارة عند حدث معين

    // مثال:
    emit keyLogged("KeyA", "Pressed");

    // لو حصل خطأ
    // emit errorOccurred("An error happened");
}
