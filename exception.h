//Simple Error Handler 2025.7.17
//
//Haider Bassem - Iraq - Wasit
//

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include<exception>
#include<QString>

class Exception : public std::exception
{
public:
    explicit Exception(const QString& message);
    const QString What() const noexcept;
    QString GetMessage() const;

private:
    QString msg;
};

#endif // EXCEPTION_H
