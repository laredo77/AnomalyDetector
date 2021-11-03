/*
 * Server.h
 *
 * Author: Itamar Laredo
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "CLI.h"

using namespace std;

/*
 * A socket IO implemented class.
 * this call implement the socket input and output functions.
 * If client use the CLI through server the connection will
 * be via this socket io.
 */
class SocketIO: public DefaultIO {
    int client;
public:
    SocketIO(int clientID) {
        client = clientID;
    }

    virtual void write(string text) {
        send(client, text.c_str(), strlen(text.c_str()), 0);
    }

    virtual void write(float f) {
        stringstream ss;
        ss << f;
        send(client, ss.str().c_str(), ss.str().length(), 0);
    }

    virtual string read() {
        string s;
        char buffer;
        while (true) {
            recv(client, &buffer, 1, 0);
            if (buffer == '\n')
                break;
            s += buffer;
        }
        return s;
    }

    virtual void read(float* f){
        float buffer;
        recv(client, &buffer, sizeof(buffer), 0);
    }
};

/*
 * handle a client class.
 */
class ClientHandler {
    public:
    virtual void handle(int clientID)=0;
};

/*
 * The server can handle few clients at the same time.
 * each client get socket io, and then the server calls
 * cli.start to start the conversation with the client.
 */
class AnomalyDetectionHandler:public ClientHandler {
public:
    virtual void handle(int clientID) {
        SocketIO sio(clientID);
        CLI cli(&sio);
        cli.start();
    }
};

/*
 * Server class.
 */
class Server {
	thread* t; // the thread to run the start() method in
    sockaddr_in m_server;
    sockaddr_in m_client;
    bool should_stop;
    int fd; // file descriptor
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
