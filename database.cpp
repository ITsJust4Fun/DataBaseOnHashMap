#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent),
    hm(new HashMap<QString, HashMap<QString, QString>>),
    data(*hm) {

}

void DataBase::changeData(const QString& key, const QString& valKey, const QString& val) {
    if(data.contains(key)){
        data[key].put(valKey, val);
    } else {
        HashMap<QString, QString> tmp;
        tmp.put(valKey, val);
        data.put(key, tmp);
    }
}

void DataBase::readFromJson(QFile& file) {
    data.clear();
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonObject root = doc.object();
    HashMap<QString, QString> hm;
    for(int i = 0; i < root.length(); i++){
        QString Number = root.keys().at(i);
        QJsonValue it = root.value(root.keys().at(i));
        QString Name = it.toObject().take("Name").toString();
        QString Faculty = it.toObject().take("Faculty").toString();
        QString Group = it.toObject().take("Group").toString();
        QString Score = it.toObject().take("Score").toString();
        QString Military = it.toObject().take("Military").toString();

        hm.put("Name", Name);
        hm.put("Faculty", Faculty);
        hm.put("Group", Group);
        hm.put("Score", Score);
        hm.put("Military", Military);
        data.put(Number, hm);
        hm.clear();
    }
}

void DataBase::writeDataBase(QFile& file) {
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outstream(&file);
    outstream.setCodec(QTextCodec::codecForName("UTF-8"));
    outstream << "{\n";
    auto it = data.begin();
    while(it != data.end()){
        outstream << QString("\t\"%1\": {\n").arg(it.getKey())
                  << QString("\t\t\"Name\": \"%1\",\n").arg((*it)["Name"])
                  << QString("\t\t\"Faculty\": \"%1\",\n").arg((*it)["Faculty"])
                  << QString("\t\t\"Group\": \"%1\",\n").arg((*it)["Group"])
                  << QString("\t\t\"Score\": \"%1\",\n").arg((*it)["Score"])
                  << QString("\t\t\"Military\": \"%1\"\n").arg((*it)["Military"])
                  << QString("\t}");
        if(++it != data.end()){
            outstream << ",\n";
        }
    }
    outstream << "\n}";
    file.close();
}

void DataBase::removeKey(const QString& key) {
    data.erase(key);
}

DataBase::~DataBase(){
    delete hm;
}
