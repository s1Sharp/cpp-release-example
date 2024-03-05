#include "ui_mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

#include <fancy_version.h>

class MyWindow : public QWidget {
public:
    MyWindow(QWidget *parent = nullptr, QPushButton* pushButton = nullptr) : QWidget(parent) {
        if (parent && pushButton)
            connect(pushButton, &QPushButton::clicked, this, &MyWindow::showNotification);
    }

public slots:
    void showNotification() {
        QMessageBox::information(this, "App version", "Version: " + QString::fromStdString(get_version()));
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mw;
    Ui::MainWindow w;
    w.setupUi(&mw);

    MyWindow myW(&mw, w.pushButton);
    mw.show();

    return a.exec();
}