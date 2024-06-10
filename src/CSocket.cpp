/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>

#include "CSocket.h"

#define PORT 80
#define BUFFER_SIZE 1024

//--------------------------------------------------------------------------------------//

CSocket::CSocket() = default;

//--------------------------------------------------------------------------------------//

CSocket::CSocket(const std::string &addr) : Address(addr) {
    cleanAddress();
}

//--------------------------------------------------------------------------------------//

CSocket::~CSocket() = default;

//--------------------------------------------------------------------------------------//

bool CSocket::Connect() {
    if ((Socket = createSocket()) == -1)return false;

    socklen_t sockAddrSize;
    struct sockaddr_in serverAddr;

    // Check the address to the server is viable
    if (!serverPrepare(sockAddrSize, serverAddr)) {
        std::cerr << "Invalid Address, Could not connect\n";
        return false;
    }

    // Open connection with the server
    if (connect(Socket, (struct sockaddr *) &serverAddr, sockAddrSize) < 0) {
        std::cerr << "Can not create connection!" << std::endl;
        if (close(Socket))std::cerr << "Socket was not closed correctly\n";
        return false;
    }

    std::cout << "SOCKET CONNECTED\n";

    return true;
}

//--------------------------------------------------------------------------------------//

int CSocket::createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Could not create a socket\n";
        return -1;
    }
    return sock;
}

//--------------------------------------------------------------------------------------//

bool CSocket::serverPrepare(socklen_t &sockAddrSize, struct sockaddr_in &serverAddr) {
    sockAddrSize = sizeof(struct sockaddr_in);

    // make sure its empty
    bzero((char *) &serverAddr, sockAddrSize);
    serverAddr.sin_family = AF_INET; //protocol IPV4
    serverAddr.sin_port = htons(PORT); // known port 80

    struct hostent *host;
    host = gethostbyname(Address.c_str());
    if (host == nullptr)return false; // Address is not viable
    memcpy(&serverAddr.sin_addr, host->h_addr, host->h_length);

    return true;
}

//--------------------------------------------------------------------------------------//

int CSocket::getSocket() const {
    return Socket;
}

//--------------------------------------------------------------------------------------//

void CSocket::cleanAddress() {
    size_t found;
    if ((found = Address.find("http://")) != std::string::npos)
        Address.erase(found, 7);
    if ((found = Address.find("www.")) != std::string::npos)
        Address.erase(found, 4);
    if ((found = Address.find('/')) != std::string::npos)
        Address.erase(found);
}

//--------------------------------------------------------------------------------------//