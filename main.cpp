#include "dynoview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dynoview w;
    w.show();
    return a.exec();
}
