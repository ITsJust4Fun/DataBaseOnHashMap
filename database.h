#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "HashMap/HashMap.h"

namespace std {
    template<>
    struct hash<QString> {
        std::size_t operator()(const QString& s) const noexcept{
            const QChar* str = s.data();
            std::size_t hash = 5381;
            for(int i = 0; i < s.size(); ++i){
                hash = ((hash << 5) + hash) +
                        ((str->row() << 8) | (str++)->cell());
            }
            return hash;
        }
    };
}

class DataBase : public QObject {
    Q_OBJECT
private:
    DataBase(const DataBase& base) :
        hm(new HashMap<QString, HashMap<QString, QString>>),
        data(*hm) { Q_UNUSED(base); }
    void operator=(const DataBase& base) { Q_UNUSED(base) }
    HashMap<QString, HashMap<QString, QString>>* hm;
public:
    explicit DataBase(QObject *parent = nullptr);
    HashMap<QString, HashMap<QString, QString>>& data;
    void readFromJson(QFile&);
    void writeDataBase(QFile&);
    virtual ~DataBase();
signals:

public slots:
    void changeData(const QString& key, const QString& valKey, const QString& val);
    void removeKey(const QString& key);
};



#endif // DATABASE_H
