#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QString>
#include"keyboardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadProfileButtons(const QString& filePath);
    void showProfileContent(const QString& filePath);
    void applyProfileWithPrivilege(const QString &sourceFile);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_CloseWindow_clicked();
    void on_LoadMap_clicked();
    void on_CreateNewMap_clicked();
    void on_scrollArea_2_customContextMenuRequested(const QPoint &pos);
    void on_LoadMyMaps_clicked();

    void on_RemoveConfigFile_clicked();

private:
    Ui::MainWindow *ui;
    QString currentProfilePath;

    KeyboardWidget *keyboardWidget;
};

#endif // MAINWINDOW_H
