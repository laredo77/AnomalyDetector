/*
 * Server.cpp
 *
 * Author: Itamar Laredo
 */
#include "Server.h"

/*
 * Constructor of server class.
 * This function open socket for optional client connection.
 */
Server::Server(int port) throw(const char *) {

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        close(fd); // release fd
        throw "socket failed";
    }

    m_server.sin_family = AF_INET; // IP version 4
    m_server.sin_addr.s_addr = INADDR_ANY;
    m_server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &m_server, sizeof(m_server)) < 0)
        throw "bind failure";

    if (listen(fd, 3) < 0)
        throw "listen failure";

    this->should_stop = false;
}

/*
 * Waiting for clients. timeout is set for 1 second.
 * If client connect, the server open new thread the will
 * handle the client and then it ready to connect new client.
 */
void Server::start(ClientHandler &ch) throw(const char *) {

    t = new thread([&ch, this]() { // lambda expression
        timeval timeout;
        while (!should_stop) {
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            cout << "waiting for client" << endl;
            socklen_t clientSize = sizeof(this->m_client);
            int aClient = accept(this->fd, (struct sockaddr *) &this->m_client, &clientSize);
            if (aClient < 0)
                throw "accept failure";
            cout << "client connected" << endl;
            t = new thread([&ch, aClient]() {
                ch.handle(aClient);
                close(aClient);
            });
        }
        close(fd);
    });
}

/*
 * when connection finish, this function will close the
 * thread and stop accepting clients loop.
 */
void Server::stop() {
    should_stop = true;
    t->join(); // do not delete this!
}

Server::~Server() {
}

