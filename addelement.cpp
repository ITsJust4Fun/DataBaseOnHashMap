#include "addelement.h"

AddElement::AddElement(QWidget *parent) : QWidget(parent) {
    screens = QGuiApplication::screens();
    screenHeight = screens[0]->size().height();
    screenWidth = screens[0]->size().width();
    widgetHeight = 300;
    widgetWidth = 300;
    setWindowTitle("Add Element");
    setWindowIcon(QIcon(":/icons/AddToDataBase_icon.png"));
    setMinimumSize(QSize(300, 300));
    setGeometry((screenWidth / 2) - (widgetWidth / 2),
                (screenHeight / 2) - (widgetHeight / 2),
                widgetWidth, widgetHeight);
    layout = new QVBoxLayout;
    lName = new QLabel("Name:");
    lNumber = new QLabel("Number:");
    lFaculty = new QLabel("Faculty:");
    lGroup = new QLabel("Group:");
    lScore = new QLabel("Score:");
    lMilitary = new QLabel("Military:");
    Name = new QLineEdit;
    Number = new QLineEdit;
    Faculty = new QLineEdit;
    Group = new QLineEdit;
    Score = new QLineEdit;
    Military = new QLineEdit;
    add = new QPushButton("Add");
    add->setBaseSize(QSize(50, 20));
    layout->addWidget(lName, 1);
    layout->addWidget(Name, 1);
    layout->addWidget(lNumber, 1);
    layout->addWidget(Number, 1);
    layout->addWidget(lFaculty, 1);
    layout->addWidget(Faculty, 1);
    layout->addWidget(lGroup, 1);
    layout->addWidget(Group, 1);
    layout->addWidget(lScore, 1);
    layout->addWidget(Score, 1);
    layout->addWidget(lMilitary, 1);
    layout->addWidget(Military, 1);
    layout->addWidget(add, 1, Qt::AlignCenter);
    setLayout(layout);
    QObject::connect(add, SIGNAL(clicked()),
                     this, SLOT(addElem()));
}

void AddElement::clear() {
    Name->setText("");
    Number->setText("");
    Faculty->setText("");
    Group->setText("");
    Score->setText("");
    Military->setText("");
}

void AddElement::addElem() {
    emit element(Name->text(), Number->text(), Faculty->text(),
                 Group->text(), Score->text(), Military->text());
}

AddElement::~AddElement() {
    delete lName;
    delete lNumber;
    delete lFaculty;
    delete lGroup;
    delete lScore;
    delete lMilitary;
    delete Name;
    delete Number;
    delete Faculty;
    delete Group;
    delete Score;
    delete Military;
    delete add;
    delete layout;
}
