#ifndef STUDENTSDATABASE_H
#define STUDENTSDATABASE_H

#include <QWidget>
#include <QObject>
#include <QScreen>
#include <QList>
#include <QGuiApplication>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextStream>
#include <QMenu>
#include <QMenuBar>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>

#include "database.h"
#include "addelement.h"
#include "mergehandler.h"

class StudentsDataBase : public QWidget {
    Q_OBJECT
public:
    StudentsDataBase(QWidget *parent = nullptr);
    ~StudentsDataBase();
private:
    void initWindow();
    void initTable();
    void initWindowElements();
    void fillTable();
    void addTableRow(const QString& name, const QString& number,
                const QString& faculty, const QString& group,
                const QString& score, const QString& military);
    void closeEvent(QCloseEvent*);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    const QString APP_NAME = "Strudents Database";
    int widgetHeight;
    int widgetWidth;
    int screenHeight;
    int screenWidth;
    QPushButton* add;
    QPushButton* del;
    QTableWidget* table;
    QList<QScreen*> screens;
    QGridLayout* gridlayout;
    QMenu* fileMenu;
    QMenu* aboutMenu;
    QMenuBar* menuBar;
    DataBase* db;
    QString fileName;
    QString dataKey;
    AddElement* addElement;
    QLineEdit* search;
    QLabel* lSearch;
    QPushButton* sortColumn;
    QPushButton* mergeButton;
    MergeHandler* mergeHandler;
    bool isCellClicked = false;
    bool isFileUnsaved = false;

public slots:
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void changeData(int x, int y);
    void saveOldKey(int x, int y);
    void addRow(const QString& Name, const QString& Number,
                const QString& Faculty, const QString& Group,
                const QString& Score, const QString& Military);
    void delRow();
    void sortByColumn();
    void findElements(const QString& text);
    void mergeCompleted();
    void showAbout();
};

#endif // STUDENTSDATABASE_H
