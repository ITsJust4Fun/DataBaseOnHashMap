#include "studentsdatabase.h"

StudentsDataBase::StudentsDataBase(QWidget *parent)
    : QWidget(parent) {
    initWindow();
    db = new DataBase;
    table = new QTableWidget;
    mergeHandler = new MergeHandler;
    mergeHandler->initDataBase(db);
    initTable();
    initWindowElements();
    QObject::connect(table, SIGNAL(cellChanged(int, int)),
                     this, SLOT(changeData(int, int)));
    QObject::connect(table, SIGNAL(cellDoubleClicked(int, int)),
                     this, SLOT(saveOldKey(int, int)));
    QObject::connect(add, SIGNAL(clicked()),
                     addElement, SLOT(show()));
    QObject::connect(addElement, SIGNAL(element(const QString&, const QString&,
                                                 const QString&, const QString&,
                                                 const QString&, const QString&)),
                     this, SLOT(addRow(const QString&, const QString&,
                                       const QString&, const QString&,
                                       const QString&, const QString&)));
    QObject::connect(del, SIGNAL(clicked()),
                     this, SLOT(delRow()));
    QObject::connect(sortColumn, SIGNAL(clicked()),
                     this, SLOT(sortByColumn()));
    QObject::connect(search, SIGNAL(textChanged(const QString&)),
                     this, SLOT(findElements(const QString&)));
    QObject::connect(mergeButton, SIGNAL(clicked()),
                     mergeHandler, SLOT(show()));
    QObject::connect(mergeHandler, SIGNAL(mergeCompleted()),
                     this, SLOT(mergeCompleted()));

    setAcceptDrops(true);
}

void StudentsDataBase::initWindow() {
    screens = QGuiApplication::screens();
    screenHeight = screens[0]->size().height();
    screenWidth = screens[0]->size().width();
    widgetHeight = 400;
    widgetWidth = 800;
    if(screenWidth < widgetWidth) {
        widgetWidth = 600;
    }
    setWindowTitle(APP_NAME);
    setWindowIcon(QIcon(":/icons/DataBase_icon.png"));
    setMinimumSize(QSize(widgetWidth, widgetHeight));
    setGeometry((screenWidth / 2) - (widgetWidth / 2),
                (screenHeight / 2) - (widgetHeight / 2),
                widgetWidth, widgetHeight);
}

void StudentsDataBase::initTable() {
    table->setColumnCount(6);
    table->setShowGrid(true);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setHorizontalHeaderLabels(QStringList() << "Ф.И.О."
                                                   << "Номер"
                                                   << "Факультет"
                                                   << "Рабочая группа"
                                                   << "Средний балл"
                                                   << "Военная кафедра"
                                     );
    table->horizontalHeader()->setMinimumSectionSize(110);
    table->horizontalHeader()->sortIndicatorOrder();
}

void StudentsDataBase::initWindowElements() {
    addElement = new AddElement;
    gridlayout = new QGridLayout;
    fileMenu = new QMenu("File");
    fileMenu->addAction("Open", this, SLOT(fileOpen()), Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Save", this, SLOT(fileSave()), Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save as", this, SLOT(fileSaveAs()), Qt::CTRL + Qt::SHIFT +Qt::Key_S);
    fileMenu->addAction("Quit", this, SLOT(close()), Qt::CTRL + Qt::Key_Q);
    aboutMenu = new QMenu("About");
    aboutMenu->addAction("About", this, SLOT(showAbout()));
    menuBar = new QMenuBar;
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(aboutMenu);
    add = new QPushButton("+");
    add->setToolTip("Add new entry");
    del = new QPushButton("-");
    del->setToolTip("Delete selected entry");
    mergeButton = new QPushButton("Merge With");
    mergeButton->setToolTip("This button merges existed database with another");
    search = new QLineEdit;
    search->setToolTip("What you need to find?");
    lSearch = new QLabel("Search:");
    sortColumn = new QPushButton("Sort Items");
    sortColumn->setToolTip("This button sorts selected column\nYou need click on cell in this column");
    search->setFixedSize(QSize(200, 25));
    sortColumn->setFixedSize(QSize(100, 25));
    mergeButton->setFixedSize(QSize(100, 25));
    add->setFixedSize(QSize(20, 20));
    del->setFixedSize(QSize(20, 20));
    gridlayout->addWidget(mergeButton, 0, 11, 1, 1, Qt::AlignCenter);
    gridlayout->addWidget(sortColumn, 0, 12, 1, 1, Qt::AlignRight);
    gridlayout->addWidget(lSearch, 0, 13, 1, 1, Qt::AlignLeft);
    gridlayout->addWidget(search, 0, 14, 1, 1, Qt::AlignRight);
    gridlayout->addWidget(add, 1, 0, 1, 1, Qt::AlignCenter);
    gridlayout->addWidget(del, 2, 0, 1, 1, Qt::AlignCenter);
    gridlayout->addWidget(table, 1, 1, 15, 15);
    gridlayout->setMargin(5);
    gridlayout->setSpacing(10);
    gridlayout->setColumnStretch(1, 1);
    gridlayout->setRowStretch(3, 1);
    gridlayout->setMenuBar(menuBar);

    setLayout(gridlayout);
}

void StudentsDataBase::fillTable(){
    for(auto it = db->data.begin(); it != db->data.end(); ++it) {
        addTableRow((*it)["Name"], it.getKey(), (*it)["Faculty"],
                (*it)["Group"], (*it)["Score"], (*it)["Military"]);
    }
}

void StudentsDataBase::addTableRow(const QString& name, const QString& number,
            const QString& faculty, const QString& group,
            const QString& score, const QString& military) {

    table->insertRow(table->rowCount());
    QTableWidgetItem* item0 = new QTableWidgetItem(name);
    table->setItem(table->rowCount() - 1, 0, item0);
    QTableWidgetItem* item1 = new QTableWidgetItem(number);
    item1->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 1, item1);
    QTableWidgetItem* item2 = new QTableWidgetItem(faculty);
    item2->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 2, item2);
    QTableWidgetItem* item3 = new QTableWidgetItem(group);
    item3->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 3, item3);
    QTableWidgetItem* item4 = new QTableWidgetItem(score);
    item4->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 4, item4);
    QTableWidgetItem* item5 = new QTableWidgetItem(Qt::CheckStateRole);
    item5->setText(military);
    if(military == ""){
        item5->setCheckState(Qt::Unchecked);
    } else {
        item5->setCheckState(Qt::Checked);
    }
    item5->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->rowCount() - 1, 5, item5);
    table->resizeColumnsToContents();
}

void StudentsDataBase::fileOpen() {
    if(isFileUnsaved){
        QMessageBox::StandardButton res = QMessageBox::question( this, APP_NAME, tr("Save database?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (res == QMessageBox::Yes) {
            if(fileName != ""){
                fileSave();
            } else {
                fileSaveAs();
                if(fileName == ""){
                    return;
                }
            }
        } else if (res == QMessageBox::No) {

        } else {
            return;
        }
    }
    fileName = QFileDialog::getOpenFileName(
              this,
              "Open JSON",
              QDir::currentPath(),
              "JSON files (*.json)");
    if(fileName == ""){
        return;
    }
    isFileUnsaved = false;
    setWindowTitle("[" + fileName + "] - " + APP_NAME);
    table->clear();
    table->setRowCount(0);
    initTable();
    QFile* file = new QFile(fileName);
    db->readFromJson(*file);
    delete file;
    fillTable();
}

void StudentsDataBase::fileSave() {
    if(fileName == ""){
        fileSaveAs();
        return;
    }
    isFileUnsaved = false;
    setWindowTitle("[" + fileName + "] - " + APP_NAME);
    QFile* file = new QFile(fileName);
    db->writeDataBase(*file);
    delete file;
}

void StudentsDataBase::fileSaveAs() {
    QString _fileName = QFileDialog::getSaveFileName(this, tr("Save JSON"),
                               QDir::currentPath(),
                               tr("JSON files (*.json)"));
    if(_fileName == "") {
        return;
    }
    fileName = _fileName;
    isFileUnsaved = false;
    setWindowTitle("[" + fileName + "] - " + APP_NAME);
    QFile* file = new QFile(fileName);
    db->writeDataBase(*file);
    delete file;
}

void StudentsDataBase::saveOldKey(int x, int y) {
    if(y == 1) {
        dataKey = table->item(x, 1)->text();
    }
    isCellClicked = true;
    Q_UNUSED(y);
}

void StudentsDataBase::changeData(int x, int y) {
    if(!isCellClicked) {
        return;
    }
	if (table->item(x, y) == nullptr ||
		table->item(x, 1) == nullptr) {
		isCellClicked = false;
		return;
	}
    QString value = table->item(x, y)->text();
    QString key = table->item(x, 1)->text();
    if(y == 0){
        db->data[key]["Name"] = value;
    } else if(y == 1) {
		if (!db->data.contains(dataKey)) {
			isCellClicked = false;
			return;
		}
        if(db->data.contains(key)) {
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","This Number already exists!");
            messageBox.setFixedSize(600,200);
            isCellClicked = false;
            table->item(x, y)->setText(dataKey);
            return;
        }
        HashMap<QString, QString> tmp = db->data[dataKey];
        db->data.erase(dataKey);
        db->data.put(key, tmp);
    } else if(y == 2) {
        db->data[key]["Faculty"] = value;
    } else if(y == 3) {
        db->data[key]["Group"] = value;
    } else if(y == 4) {
        db->data[key]["Score"] = value;
    } else if(y == 5) {
        db->data[key]["Military"] = value;
        if(value == "") {
            table->item(x, y)->setCheckState(Qt::Unchecked);
        } else {
            table->item(x, y)->setCheckState(Qt::Checked);
        }
    }
    isCellClicked = false;
    if(!isFileUnsaved){
        isFileUnsaved = true;
        setWindowTitle("[" + fileName + "]* - " + APP_NAME);
    }
}

void StudentsDataBase::addRow(const QString& Name, const QString& Number,
            const QString& Faculty, const QString& Group,
            const QString& Score, const QString& Military){
    if(db->data.contains(Number)){
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","This Number already exists!");
        messageBox.setFixedSize(600,200);
        return;
    }
    HashMap<QString, QString> hm;
    hm.put("Name", Name);
    hm.put("Faculty", Faculty);
    hm.put("Group", Group);
    hm.put("Score", Score);
    hm.put("Military", Military);
    db->data.put(Number, hm);
    findElements(search->text());
    addElement->clear();
    addElement->close();
    if(fileName == ""){
        setWindowTitle("[New File]* - " + APP_NAME);
        isFileUnsaved = true;
    } else if (!isFileUnsaved) {
        isFileUnsaved = true;
        setWindowTitle("[" + fileName + "]* - " + APP_NAME);
    }
}

void StudentsDataBase::delRow() {
    if(table->currentRow() == -1) {
        return;
    }
    if(table->item(table->currentRow(), 1) == nullptr){
        return;
    }
    if(fileName == "") {
        setWindowTitle("[New File]* - " + APP_NAME);
        isFileUnsaved = true;
    } else if (!isFileUnsaved) {
        isFileUnsaved = true;
        setWindowTitle("[" + fileName + "]* - " + APP_NAME);
    }
    db->data.erase(table->item(table->currentRow(), 1)->text());
    table->removeRow(table->currentRow());
}

void StudentsDataBase::sortByColumn() {
    if(table->currentColumn() < 0 || table->currentColumn() > 5) {
        return;
    }
    table->sortByColumn(table->currentColumn(), Qt::AscendingOrder);
}

void StudentsDataBase::findElements(const QString& text) {
    table->clear();
    table->setRowCount(0);
    initTable();
    if(text == ""){
        fillTable();
        return;
    }
    for(auto it = db->data.begin(); it != db->data.end(); ++it) {
        bool flag = false;
        if(it.getKey().contains(text)){
            flag = true;
        } else if((*it)["Name"].contains(text)) {
            flag = true;
        } else if((*it)["Faculty"].contains(text)) {
            flag = true;
        } else if((*it)["Group"].contains(text)) {
            flag = true;
        } else if((*it)["Score"].contains(text)) {
            flag = true;
        } else if((*it)["Military"].contains(text)) {
            flag = true;
        }
        if(flag){
            addTableRow((*it)["Name"], it.getKey(), (*it)["Faculty"],
                    (*it)["Group"], (*it)["Score"], (*it)["Military"]);
        }
    }
}

void StudentsDataBase::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void StudentsDataBase::dropEvent(QDropEvent *e) {
    if(isFileUnsaved) {
        QMessageBox::StandardButton res = QMessageBox::question( this, APP_NAME, tr("Save data base?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (res == QMessageBox::Yes) {
            if(fileName != ""){
                fileSave();
            } else {
                fileSaveAs();
                if(fileName == ""){
                    return;
                }
            }
        } else if (res == QMessageBox::No) {

        } else {
            return;
        }
    }
    QString fileName = e->mimeData()->urls().at(0).toLocalFile();
    this->fileName = fileName;
    isFileUnsaved = false;
    setWindowTitle("[" + fileName + "] - " + APP_NAME);
    table->clear();
    table->setRowCount(0);
    initTable();
    QFile* file = new QFile(fileName);
    db->readFromJson(*file);
    delete file;
    fillTable();
}

void StudentsDataBase::mergeCompleted() {
    if(fileName == "") {
        fileName = mergeHandler->getFileName();
        if(isFileUnsaved) {
            setWindowTitle("[" + fileName + "]* - " + APP_NAME);
        } else {
            setWindowTitle("[" + fileName + "] - " + APP_NAME);
        }
    } else {
        setWindowTitle("[" + fileName + "]* - " + APP_NAME);
        isFileUnsaved = true;
    }
    mergeHandler->close();
    table->clear();
    table->setRowCount(0);
    initTable();
    fillTable();
}

void StudentsDataBase::closeEvent(QCloseEvent* e) {
    if(isFileUnsaved){
        QMessageBox::StandardButton res = QMessageBox::question( this, APP_NAME, tr("Save database?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (res == QMessageBox::Yes) {
            if(fileName != ""){
                fileSave();
            } else {
                fileSaveAs();
                if(fileName == ""){
                    e->ignore();
                    return;
                }
            }
            e->accept();
        } else if (res == QMessageBox::No) {
            e->accept();
        } else {
            e->ignore();
        }
    }
}

void StudentsDataBase::showAbout(){
    QMessageBox messageBox;
    messageBox.information(nullptr, "About",
             "Курсовая работа:\nРодимова Артёма\nГруппа: ИКПИ-71");
}

StudentsDataBase::~StudentsDataBase() {
    delete db;
    delete table;
    delete gridlayout;
    delete fileMenu;
    delete aboutMenu;
    delete menuBar;
    delete add;
    delete del;
    delete addElement;
    delete search;
    delete lSearch;
    delete sortColumn;
    delete mergeHandler;
}
