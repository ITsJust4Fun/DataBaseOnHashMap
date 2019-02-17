#include "mergehandler.h"

MergeHandler::MergeHandler(QWidget *parent)
    : QWidget(parent) {
    screens = QGuiApplication::screens();
    screenHeight = screens[0]->size().height();
    screenWidth = screens[0]->size().width();
    widgetHeight = 150;
    widgetWidth = 300;
    setWindowTitle("Merge Handler");
    setWindowIcon(QIcon(":/icons/MergeDataBase_icon.png"));
    setMinimumSize(QSize(widgetWidth, widgetHeight));
    setGeometry((screenWidth / 2) - (widgetWidth / 2),
                (screenHeight / 2) - (widgetHeight / 2),
                widgetWidth, widgetHeight);
    layout = new QVBoxLayout;
    openFileButton = new QPushButton("Open");
    openFileButton->setFixedSize(QSize(70, 30));
    openFileButton->setToolTip("Open existed file");
    label = new QLabel;
    label->setText("Select or Drag'n'Drop file");
    layout->addWidget(label, 1, Qt::AlignCenter);
    layout->addWidget(openFileButton, 1, Qt::AlignCenter);
    setLayout(layout);
    setAcceptDrops(true);

    QObject::connect(openFileButton, SIGNAL(clicked()),
                     this, SLOT(openFile()));
}

void MergeHandler::initDataBase(DataBase* db){
    this->db = db;
}

void MergeHandler::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MergeHandler::dropEvent(QDropEvent *e) {
    fileName = e->mimeData()->urls().at(0).toLocalFile();
    makeMerge();
}

void MergeHandler::openFile() {
    fileName = QFileDialog::getOpenFileName(
                this,
                "Open JSON",
                QDir::currentPath(),
                "JSON files (*.json)");
    if(fileName == ""){
        return;
    }
    makeMerge();
}

void MergeHandler::makeMerge() {
    DataBase db1;
    QFile file(fileName);
    db1.readFromJson(file);
    for(auto it = db1.data.begin(); it != db1.data.end(); ++it) {
        if(db->data.contains(it.getKey())){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Merge Failed!", "Numbers collision!");
            messageBox.setFixedSize(600,200);
            return;
        }
    }
    for(auto it = db1.data.begin(); it != db1.data.end(); ++it) {
        db->data.put(it.getKey(), *it);
    }
    emit mergeCompleted();
}

QString MergeHandler::getFileName(){
    return fileName;
}

MergeHandler::~MergeHandler(){
    delete layout;
    delete openFileButton;
    delete label;
}
