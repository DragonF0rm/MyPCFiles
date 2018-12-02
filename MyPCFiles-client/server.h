#ifndef SERVER_H
#define SERVER_H

#include <libssh/sftp.h>
#include <string>
#include <cstdarg>

#include <QString>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>

using std::string;

class Server {

public:
    Server(): username(""), host(""), port(0) {}
    Server(string _username, string _host, unsigned int _port):
        username(_username), host(_host), port(_port) {}
    Server(const Server& other) = delete;
    Server(Server&& other) = delete;
    ~Server();

    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;

    void connect();
    void disconnect();
    void showErrorDialog(QString message);
    QString strConcat(int n_args, ...);

    sftp_session getSftp() { return sftp; }
    ssh_session getSsh(){ return ssh; }

private:

    string username;
    string host;
    unsigned int port;
    ssh_session ssh;
    ssh_key publicKey;
    sftp_session sftp;

    bool verifyServer();
    bool auth();
    bool generateKeys();
};

#endif // SERVER_H
