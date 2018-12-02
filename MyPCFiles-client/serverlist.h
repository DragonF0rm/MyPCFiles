#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <vector>
#include <memory>
#include "server.h"

using std::vector;
using std::shared_ptr;

class ServerList{
private:
    vector<shared_ptr<Server>> list;
public:
    ServerList();
    ServerList(const ServerList& other) = delete;
    ServerList(ServerList&& other) = delete;
    ~ServerList();

    ServerList&operator=(const ServerList& other) = delete;
    ServerList&operator=(ServerList&& other) = delete;

    auto begin() -> decltype(list.begin()) {return list.begin(); }
    auto end() -> decltype(list.end()) {return list.end(); }

    void add(shared_ptr<Server> server);
};

#endif // SERVERLIST_H
