#include "keyboardwidget.h"

KeyboardWidget::KeyboardWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5,5,5,5);

    setupKeys();
}
void KeyboardWidget::setupKeys()
{
    auto addBtn = [&](const QString &text, int row, int col, int colspan = 1, int width = 70, int height = 50) -> QPushButton*
    {
        QPushButton *btn = new QPushButton(text, this);
        btn->setFixedSize(width, height);
        layout->addWidget(btn, row, col, 1, colspan);
        buttons.append(btn);

        return btn;
    };

    // Row 0: Esc, F1-F12, Mute, Insert, PrntScr, Delete
    QStringList row0 = {"Esc", "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","Mute","Insert","PrntScr","Delete"};
    int col = 0;
    for (const QString &key : row0) {
        int w = (key == "Esc" || key == "Mute" || key == "Insert" || key == "PrntScr" || key == "Delete") ? 70 : 60;
        addBtn(key, 0, col++, 1, w, 50);
    }

    // Row 1: ` 1 2 ... Backspace, Home
    addBtn("`", 1, 0, 1, 70);
    QStringList row1 = {"1","2","3","4","5","6","7","8","9","0","-","="};
    col = 1;
    for (const QString &key : row1) {
        addBtn(key, 1, col++, 1, 60);
    }
    addBtn("Backspace", 1, col++, 1, 110);
    addBtn("Home", 1, col++, 1, 70);

    // Row 2: Tab, Q-P, [, ], \
    addBtn("Tab", 2, 0, 1, 90);
    QStringList row2 = {"Q","W","E","R","T","Y","U","I","O","P","{","}","\\"};
    col = 1;
    for (const QString &key : row2) {
        addBtn(key, 2, col++, 1, 60);
    }
    addBtn("Pg Up", 2, col++, 1, 70);

    // Row 3: Caps Lock, A-L, ;, ', Enter, Pg Dn
    addBtn("Caps Lock", 3, 0, 1, 100);
    QStringList row3 = {"A","S","D","F","G","H","J","K","L",";", "\""};
    col = 1;
    for (const QString &key : row3) {
        addBtn(key, 3, col++, 1, 60);
    }
    addBtn("Enter", 3, col++, 1, 110);
    addBtn("Pg Dn", 3, col++, 1, 70);

    // Row 4: Shift, Z-M, ,, ., /, Shift, Up, End
    addBtn("Shift", 4, 0, 1, 110);
    QStringList row4 = {"Z","X","C","V","B","N","M",",",".","?"};
    col = 1;
    for (const QString &key : row4) {
        addBtn(key, 4, col++, 1, 60);
    }
    addBtn("Shift", 4, col++, 1, 110);
    addBtn("↑", 4, col++, 1, 60);
    addBtn("End", 4, col++, 1, 70);

    // Row 5: Ctrl, Fn, Alt, Space, Alt, Ctrl, Copilot, Left, Down, Right
    addBtn("Ctrl", 5, 0, 1, 70);
    addBtn("Fn", 5, 1, 1, 50);
    addBtn("Alt", 5, 2, 1, 70);
    addBtn("Space", 5, 3, 4, 280);
    addBtn("Alt", 5, 7, 1, 70);
    addBtn("Copilot", 5, 8, 1, 70);
    //addBtn("Menu", 5, 9, 1, 70);
    addBtn("←", 5, 10, 1, 60);
    addBtn("→", 5, 11, 1, 60);
    addBtn("↓", 5, 12, 1, 60);
}


