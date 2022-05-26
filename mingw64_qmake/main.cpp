#include "qstylewidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

QString getStyleSheet(QString path)
{
    QString style;
    QFileInfo fileInfo(path);
    if (fileInfo.exists())
    {
        static QDateTime lastTime;
        QDateTime time =  fileInfo.lastModified();
        if (lastTime != time)
        {
            lastTime = time;
            qDebug() << "update style :" << path << time.toString();
            QFile file(path);
            if (file.open(QIODevice::ReadOnly))
            {
                style = file.readAll();
                file.close();
            }
        }
    }
    return style;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStyleWidget s;
    s.show();

//    QMainWindow w;
//    w.show();

//    QString style = getStyleSheet(s.getStylePath());
//    w.setStyleSheet(style);

    return a.exec();
}
