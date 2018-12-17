#ifndef SFTPINTERFACE_H
#define SFTPINTERFACE_H

#include <QList>
#include "server.h"
#include "string"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"

using std::string;

class SFTPInterface {
private:
    Server* server;
    QList<sftp_attributes> files;

public:
    SFTPInterface(Server* _server = nullptr): server(_server) {}
    SFTPInterface(const SFTPInterface&) = delete;
    SFTPInterface(SFTPInterface&&) = delete;
    ~SFTPInterface();

    SFTPInterface&operator=(const SFTPInterface&) = delete;
    SFTPInterface&operator=(SFTPInterface&&) = delete;

    void open_connection();
    void close_connection();
    void changeDir(string path);
    void upload(string fileName);
    void download(string fileName);
};

#endif // SFTPINTERFACE_H
