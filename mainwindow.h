// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadProfileButtons(const QString& filePath);

private slots:
    void on_scrollArea_2_customContextMenuRequested(const QPoint &pos);
    void on_CloseWindow_clicked();
    void on_LoadMap_clicked();
    void on_CreateNewMap_clicked();
    void on_LoadMyMaps_clicked();
    void on_RemoveConfigFile_clicked();
    void clearLayout(QLayout *layout);
private:
    Ui::MainWindow *ui;
    QVBoxLayout* mainLayout = nullptr;
    QString currentProfilePath;

    void clearMainLayout();
    void showProfileContent(const QString& filePath);
    void applyProfileWithPrivilege(const QString &sourceFile);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MAINWINDOW_H
