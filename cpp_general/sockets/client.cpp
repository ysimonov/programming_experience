/*
 * Example of TCP client
*/

#include <cstring>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// inputs are port number and ip addresses
int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Env variables must include <ipaddress> <port>\n";
        return EXIT_FAILURE;
    }

    auto& ip_address = argv[1];
    auto& port_num = argv[2];

    addrinfo hints;
    addrinfo* ptr_p = nullptr;  // used in iteration

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;        // address family
    hints.ai_socktype = SOCK_STREAM;    // type of socket
    hints.ai_flags = AI_PASSIVE;        // socket address intended for a bind

    int g_address = getaddrinfo(ip_address, port_num, &hints, &ptr_p);

    if (g_address != 0) {
        std::cerr << gai_strerror(g_address) << "\n";
        return EXIT_FAILURE;
    }

    if (ptr_p == nullptr) {
        std::cerr << "No address found!\n";
        return EXIT_FAILURE;
    }

    // create new socket descriptor
    int sock_fd = socket(ptr_p->ai_family, ptr_p->ai_socktype, ptr_p->ai_protocol);
    if (sock_fd == -1) {
        std::cerr << "Error while creating socket!\n";
        return EXIT_FAILURE;
    }

    // client doesn't have to have a fixed port number
    // therefore their is no call to bind, as it binds to any available socket ip address

    // connect() establishes a TCP connection to the specified server
    int conn_stat = connect(sock_fd, ptr_p->ai_addr, ptr_p->ai_addrlen);
    if (conn_stat == -1) {
        close(sock_fd);
        std::cerr << "Error while connecting to socket!\n";
        return EXIT_FAILURE;
    }

    std::string reply(15, ' ');
    // recv() call tries to get the response from server
    // the response can take multiple calls to recv() before it is completely received
    auto bytes_recv = recv(sock_fd, &reply.front(), reply.size(), 0);
    if (bytes_recv == -1) {
        std::cerr << "Error while receiving bytes!\n";
        return EXIT_FAILURE;
    }

    std::cout << "\nClient received: " << reply << std::endl;
    close(sock_fd);
    freeaddrinfo(ptr_p);

    return EXIT_SUCCESS;
}