/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <string>
#include <sys/socket.h>

/**
 * Web socket to create connection
 */
class CSocket {
public:
    CSocket();

    explicit CSocket(const std::string &addr);

    ~CSocket();

    /**
     * Allows user to create connection with server
     * @param Address given to constructor address of the site we connecting to
     * @return true if the connection is open
     */
    bool Connect();

    int getSocket() const;

private:
    /**
     * Attempts to create socket (IPV4)
     * @return socket or -1 if it failed
     */
    static int createSocket();

    /**
     * Prepares parameters for server connection, checks the address is viable
     * @param sockAddrSize size of a server socket
     * @param serverAddr address we aren connection to
     * @var *host takes IPV4 or URL
     */
    bool serverPrepare(socklen_t &sockAddrSize, struct sockaddr_in &serverAddr);

    /**
     * Reshape address to pure form so that program can try and connect to it
     */
    void cleanAddress();

    int Socket = -1;
    std::string Address;
};
