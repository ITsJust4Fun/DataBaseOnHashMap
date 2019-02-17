#ifndef ADDELEMENT_H
#define ADDELEMENT_H

#include <QObject>
#include <QWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class AddElement : public QWidget {
    Q_OBJECT
public:
    explicit AddElement(QWidget *parent = nullptr);
    void clear();
    ~AddElement();
private:
    int widgetHeight;
    int widgetWidth;
    int screenHeight;
    int screenWidth;
    QLabel* lName;
    QLabel* lNumber;
    QLabel* lFaculty;
    QLabel* lGroup;
    QLabel* lScore;
    QLabel* lMilitary;
    QLineEdit* Name;
    QLineEdit* Number;
    QLineEdit* Faculty;
    QLineEdit* Group;
    QLineEdit* Score;
    QLineEdit* Military;
    QPushButton* add;
    QVBoxLayout* layout;
    QList<QScreen*> screens;

signals:
    void element(const QString& Name, const QString& Number,
                 const QString& Faculty, const QString& Group,
                 const QString& Score, const QString& Military);
public slots:
    void addElem();
};

#endif // ADDELEMENT_H
