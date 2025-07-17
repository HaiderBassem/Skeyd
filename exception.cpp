#include "exception.h"
#include<QString>
Exception::Exception(const QString& message)
    : msg(message) {}


const QString Exception::What() const noexcept
{
    return msg.toStdString().c_str();
}

QString Exception::GetMessage() const
{
    return msg;
}
