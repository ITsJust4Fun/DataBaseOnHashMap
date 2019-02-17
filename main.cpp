#include "studentsdatabase.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    StudentsDataBase w;
    w.show();

    return a.exec();
}
