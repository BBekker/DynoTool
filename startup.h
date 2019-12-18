#ifndef STARTUP_H
#define STARTUP_H

#include "canhandler.h"
#include "dynoview.h"

#include <QDialog>

namespace Ui {
class Startup;
}

class Startup : public QDialog
{
    Q_OBJECT

public slots:
    void addScreen(int id);

public:
    explicit Startup(QWidget *parent = nullptr);
    ~Startup();



private:
    Ui::Startup *ui;
    dynoview *dynoviews[10] = {0};
    CANHandler *canHandler;
};

#endif // STARTUP_H
