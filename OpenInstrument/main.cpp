#include "OpenInstrument.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenInstrument w;
    w.show();
    return a.exec();
}
