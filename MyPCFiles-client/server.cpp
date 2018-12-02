#include <server.h>

void Server::connect(){
    //Необходимо создать ssh и затем sftp подключение
    //Затем необходимо последовательно вызвать функции verifyServer и auth (подробнее см. документацию libssh)
    //Если это первое подключение к этому серверу, т.е. оно проводилось с
    // помощью логина и пароля, необходимо вызвать функцию generateKeys

    ssh = ssh_new();

    if (ssh == nullptr)
        return;

    int rc;

    ssh_options_set(ssh, SSH_OPTIONS_HOST, &host);
    rc = ssh_connect(ssh);

    if (rc != SSH_OK) {
        SERVER_ERROR(2, "Error connecting to ", ssh_get_error(ssh));
        return;
    }

    sftp = sftp_new(ssh);
    if (sftp == nullptr) {;
        SERVER_ERROR(2, "Error allocating SFTP session: ", ssh_get_error(ssh));
        return;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK) {

        SERVER_ERROR(2, "Error allocating SFTP session: ", ssh_get_error(sftp));
        sftp_free(sftp);
        return;
    }

    auth();
}

void Server::disconnect(){
    //Необходимо закрыть sftp и ssh подключение
    //Если на данный момент на сервер или с сервере загружаются файлы, то нужно создать
    // диалог, запрашивающий подтверждение пользователя
    //В зависимости от ответа, необходимо либо отменить закрытие подключения, либо
    //прервать загрузку файлов и отключиться от сервера
    // Этот метод пока не до конца реализовано!!!
    ssh_key_free(publicKey);
    ssh_disconnect(ssh);
    ssh_free(ssh);
    sftp_free(sftp);
    connected = false;
}

bool Server::verifyServer(){
    //TODO check server eventide
    //Функция производит верификацию сервера (см. документацию libssh, там есть пример)

    enum ssh_known_hosts_e state;
    unsigned char *hash = nullptr;
    size_t hlen;
    string hexa;
    int rc;

    if (!generateKeys())
        return false;

    rc = ssh_get_publickey_hash(publicKey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);

    if (rc < 0)
        return false;

    state = ssh_session_is_known_server(ssh);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
            break;

        case SSH_KNOWN_HOSTS_CHANGED:
            SERVER_ERROR("Host key for server changed. For security reasons, connection will be stopped");
            ssh_clean_pubkey_hash(&hash);
            return false;

        // The host key for this server was not found but an othe type of key exists;
        // An attacker might change the default server key to
        // confuse your client into thinking the key does not exist
        case SSH_KNOWN_HOSTS_OTHER:
            SERVER_ERROR("The host key for this server was not found!!!");
            ssh_clean_pubkey_hash(&hash);
            return false;

        case SSH_KNOWN_HOSTS_NOT_FOUND:
            SERVER_ERROR("Could not find known host file!");
            return false;

        case SSH_KNOWN_HOSTS_UNKNOWN: {
            hexa = ssh_get_hexa(hash, hlen);
            ssh_clean_pubkey_hash(&hash);

            QMessageBox msgBox;

            msgBox.setText("The server is unknown. Do you trust the host key?");
            msgBox.setDetailedText(QString::fromStdString(string("Public key hash: ") + hexa));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);

            int ret = msgBox.exec();

            switch (ret) {
              case QMessageBox::Yes:
                    rc = ssh_session_update_known_hosts(ssh);
                    if (rc < 0) {
                        SERVER_ERROR(2, "Error: ", strerror(errno));
                        return false;
                    }
                  break;
              case QMessageBox::No:
                  return false;
            }

            break;
        }

        case SSH_KNOWN_HOSTS_ERROR:
            SERVER_ERROR(2, "Error: ", ssh_get_error(ssh));
            ssh_clean_pubkey_hash(&hash);
            return false;
    }

    ssh_clean_pubkey_hash(&hash);

    return true;
}

bool Server::auth(){
    //TODO provide autitification on the server
    //Функция производит авторизацию пользователя на сервере (см. документацию libssh, там естьб пример)
    //Прежде всего, необходимо попытаться авторизироваться по ключу
    //Если авторизация по ключу не прошла, необходимо авторизироваться по логину и
    //паролю, их нужно запросить у пользователя с помощью диалога

    int rc;
    string password;

    if (!verifyServer())
        return false;

    rc = ssh_userauth_publickey_auto(ssh, nullptr, nullptr);
    if (rc == SSH_AUTH_ERROR) {

        bool ok;
        QString text = QInputDialog::getText(nullptr, "Inter the password",
                                             "Please enter the password:", QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);

        if (ok && !text.isEmpty()) {
            password = text.toUtf8().constData();

            rc = ssh_userauth_password(ssh, nullptr, password.c_str());
            if (rc != SSH_AUTH_SUCCESS) {
                SERVER_ERROR(2, "Error authenticating with password: ", ssh_get_error(ssh));
                return false;
            }
        } else
            return false;
    }
    connected = true;
    return true;
}

bool Server::generateKeys(){
    int rc;

    rc = ssh_get_server_publickey(ssh, &publicKey);
    if (rc < 0)
        return false;

    return true;
}

void Server::SERVER_ERROR(int n_args, ...) {
    va_list ap;
    va_start(ap, n_args);

    string res = "";

    for(int i = 0; i < n_args; i++)
       res += va_arg(ap, char *);

    SERVER_ERROR(res);
}

void Server::SERVER_ERROR(string message){
    QErrorMessage dialog;
    dialog.setModal(true);
    dialog.showMessage(QString::fromStdString(message));
    dialog.exec();
}
