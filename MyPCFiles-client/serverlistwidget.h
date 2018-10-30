#ifndef SERVERLISTWIDGET_H
#define SERVERLISTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QListWidget>
#include "server.h"

class ServerListWidget: public QListWidget{

    Q_OBJECT

private:
    QVector<Server> list;

    void save();
    void load();

public:
    ServerListWidget(QWidget* parent = nullptr);
    ServerListWidget(const ServerListWidget& other) = delete;
    ServerListWidget(ServerListWidget&& other) = delete;
    ~ServerListWidget();

    ServerListWidget&operator=(const ServerListWidget& other) = delete;
    ServerListWidget&operator=(ServerListWidget&& other) = delete;

    void add(Server server);
    void remove(size_t pos);
    void change(size_t pos, QString _host, unsigned int _port, QString _username);

};
#endif // SERVERLISTWIDGET_H
