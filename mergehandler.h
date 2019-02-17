#ifndef MERGEHANDLER_H
#define MERGEHANDLER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>

#include "database.h"

class MergeHandler : public QWidget {
    Q_OBJECT
public:
    explicit MergeHandler(QWidget *parent = nullptr);
    void initDataBase(DataBase* db);
    QString getFileName();
    ~MergeHandler();
private:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void makeMerge();
    int widgetHeight;
    int widgetWidth;
    int screenHeight;
    int screenWidth;
    DataBase* db;
    QList<QScreen*> screens;
    QVBoxLayout* layout;
    QPushButton* openFileButton;
    QLabel* label;
    QString fileName;
signals:
    void mergeCompleted();
public slots:
    void openFile();
};

#endif // MERGEHANDLER_H
