#include "dynoview.h"
#include "canhandler.h"
#include "startup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Startup s;

    s.show();

    return a.exec();
}
