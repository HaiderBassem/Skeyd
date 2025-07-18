// KeyboardWidget.h
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>

class KeyboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit KeyboardWidget(QWidget *parent = nullptr);

private:
    QGridLayout *layout;
    QList<QPushButton*> buttons;

    void setupKeys();
};
