#include <QtWidgets/QApplication>
#include "XImage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XImage w;
    w.show();

    return a.exec();
    return 0;
}
