#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client w;
    w.setWindowTitle("Jeevan");
    w.setWindowIcon(QIcon(":img/logo.png"));
    w.show();

    return a.exec();
}
