#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <stdio.h>

int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     QWidget w;
     w.show();
     return a.exec();
}
