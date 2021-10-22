#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>
#include <iostream>
#include "client/Client.h"

namespace glob_str { std::string DEFAULT_CONN_STR; }//не надо так,надо перегружать qapp и работать внутри него ToDo поправить
void handler(int);

void textChanged(const QString &str);

int main(int argc, char *argv[]) {
    glob_str::DEFAULT_CONN_STR = "127.0.0.1:1236";
    QApplication app(argc, argv);
    QWidget window;
    auto *label = new QLabel(QApplication::translate("windowlayout", "connection string:"));
    auto *lineEdit = new QLineEdit();
    lineEdit->setText(QString::fromUtf8(glob_str::DEFAULT_CONN_STR.c_str()));

    QPushButton btnConnect("Connect", nullptr);
    QObject::connect(&btnConnect, &QPushButton::clicked, handler);
    QObject::connect(lineEdit, &QLineEdit::textChanged, textChanged);

    auto *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(&btnConnect);
    window.setLayout(layout);
    window.setWindowTitle(
            QApplication::translate("windowlayout", "Window layout"));
    window.show();

    return app.exec();
}

void errmsg(const char *msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void handler(int) {
    try {
        Client client(glob_str::DEFAULT_CONN_STR);
    } catch (const boost::system::system_error &e) {
        errmsg(e.what());
    } catch (const std::runtime_error &e) {
        errmsg(e.what());
    }
}

void textChanged(const QString &str) {
    glob_str::DEFAULT_CONN_STR = str.toStdString();
}