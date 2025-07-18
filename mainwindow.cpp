#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keyboardwidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QProcess>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    KeyboardWidget* keyboard = new KeyboardWidget(ui->centralwidget);
    keyboard->setGeometry(10, 450, 1450, 450);
    keyboard->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadProfileButtons(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file.");
        return;
    }

    QTextStream in(&file);
    bool inMainSection = false;

    QLayout* oldLayout = ui->scrollAreaWidgetContents->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#') || line.startsWith(';'))
            continue;

        if (!inMainSection) {
            if (line == "[main]") {
                inMainSection = true;
            }
            continue;
        }

        if (line.startsWith('[')) {
            break;
        }

        QStringList parts = line.split('=');
        if (parts.size() != 2)
            continue;

        QString left = parts[0].trimmed();
        QString right = parts[1].trimmed();

        QHBoxLayout* rowLayout = new QHBoxLayout;

        QPushButton* leftBtn = new QPushButton(left);
        QPushButton* rightBtn = new QPushButton(right);

        leftBtn->setFixedSize(350, 50);
        rightBtn->setFixedSize(350, 50);

        rowLayout->addWidget(leftBtn);
        rowLayout->addWidget(rightBtn);

        mainLayout->addLayout(rowLayout);
    }

    ui->scrollAreaWidgetContents->setLayout(mainLayout);
    file.close();
}

void MainWindow::showProfileContent(const QString& filePath)
{
    loadProfileButtons(filePath);
}

void MainWindow::applyProfileWithPrivilege(const QString &sourceFile)
{
    QFile file(sourceFile);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Failed to open source file.");
        return;
    }

    QByteArray content = file.readAll();
    file.close();

    QProcess process;
    process.start("pkexec", QStringList() << "tee" << "/etc/keyd/default.conf");
    if (!process.waitForStarted()) {
        QMessageBox::warning(this, "Error", "Failed to start pkexec process.");
        return;
    }

    process.write(content);
    process.closeWriteChannel();

    if (!process.waitForFinished()) {
        QMessageBox::warning(this, "Error", "Process did not finish correctly.");
        return;
    }

    if (process.exitCode() == 0) {
        QMessageBox::information(this, "Success", "Profile applied successfully.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to apply profile or cancelled.");
    }
}

void MainWindow::on_scrollArea_2_customContextMenuRequested(const QPoint &)
{
    QString profilesDir = QDir::homePath() + "/Skeyd/profiles/";
    QDir dir(profilesDir);

    if (!dir.exists()) {
        QMessageBox::warning(this, "Error", "Profiles directory does not exist.");
        return;
    }

    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);

    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);

    for (const QString &fileName : files) {
        QPushButton *btn = new QPushButton(fileName);
        btn->setFixedSize(400, 50);


        connect(btn, &QPushButton::clicked, this, [=]() {
            currentProfilePath = profilesDir + fileName;
            showProfileContent(currentProfilePath);
        });


        btn->installEventFilter(this);
        btn->setProperty("filepath", profilesDir + fileName);

        layout->addWidget(btn);
    }

    container->setLayout(layout);
    ui->scrollArea_2->setWidget(container);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        QPushButton *btn = qobject_cast<QPushButton*>(watched);
        if (btn) {
            QString filePath = btn->property("filepath").toString();
            applyProfileWithPrivilege(filePath);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_CloseWindow_clicked() { this->close(); }

void MainWindow::on_LoadMap_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File", QDir::homePath(), "*.txt");
    if (fileName.isEmpty()) return;

    QString destDir = QDir::homePath() + "/Skeyd/profiles/";
    QDir().mkpath(destDir);

    QFileInfo fileInfo(fileName);
    QString destFile = destDir + fileInfo.fileName();

    if (QFile::copy(fileName, destFile)) {
        QMessageBox::information(this, "Success", "File copied successfully.");
    } else {
        QMessageBox::warning(this, "Failure", "Failed to copy the file.");
    }
}

void MainWindow::on_CreateNewMap_clicked()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, "New File", "Enter file name:", QLineEdit::Normal, "", &ok);

    if (!ok || fileName.isEmpty()) return;

    if (!fileName.endsWith(".txt"))
        fileName += ".txt";

    QString destDir = QDir::homePath() + "/Skeyd/profiles/";
    QDir().mkpath(destDir);

    QString destFile = destDir + fileName;

    QFile file(destFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        QMessageBox::information(this, "Success", "File created successfully.");
    } else {
        QMessageBox::warning(this, "Failure", "Failed to create the file.");
    }
}

void MainWindow::on_LoadMyMaps_clicked()
{
    on_scrollArea_2_customContextMenuRequested(QPoint());
}

void MainWindow::on_RemoveConfigFile_clicked()
{
    if (currentProfilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected to delete.");
        return;
    }

    QFile file(currentProfilePath);
    if (!file.exists()) {
        QMessageBox::warning(this, "Error", "Selected file does not exist.");
        return;
    }

    if (!file.remove()) {
        QMessageBox::warning(this, "Error", "Failed to delete the file.");
        return;
    }

    QMessageBox::information(this, "Success", "File deleted successfully.");

    QWidget *emptyWidget = new QWidget();
    ui->scrollArea_2->setWidget(emptyWidget);

    currentProfilePath.clear();
}
