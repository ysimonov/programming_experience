#include <cstring>  // sizeof()
#include <iostream>
#include <string>

// socket(), getaddinfo(), friends
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // close()

#include <limits>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Specify port number" << std::endl;
        return -1;
    }

    const auto &port_num = argv[1];
    const static uint64_t num_conn = 8;

    addrinfo hints;
    addrinfo *ptr_res = nullptr;  // hold
    addrinfo *ptr_p = nullptr;    // iterate

    memset(&hints, 0, sizeof(hints));

    // man socket
    hints.ai_family = AF_UNSPEC;      // don't specify IP version to use
    hints.ai_socktype = SOCK_STREAM;  // SOCK_STREAM is TCP, SOCK_DGRAM is UDP
    hints.ai_flags = AI_PASSIVE;

    // man getaddrinfo
    int get_addr = getaddrinfo(nullptr, port_num, &hints, &ptr_res);
    if (get_addr != 0) {
        std::cerr << gai_strerror(get_addr) << std::endl;
        freeaddrinfo(ptr_res);
        return -2;
    }

    std::cout << "Finding address ..." << std::endl;

    uint32_t num_of_addr = 0;
    char ip_str[INET6_ADDRSTRLEN];  // both ipv4/6 can be stored in the variable

    // getaddinfo() gives list of addresses that can be iterated over
    // then bind to available socket address
    ptr_p = ptr_res;
    while (ptr_p != nullptr) {
        void *ptr_addr;
        std::string ip_ver;

        // if ipv4
        if (ptr_p->ai_family == AF_INET) {
            ip_ver = "IPv4";
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(ptr_p->ai_addr);
            ptr_addr = &(ipv4->sin_addr);
            ++num_of_addr;
        }

        // if ipv6
        else {
            ip_ver = "IPv6";
            sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(ptr_p->ai_addr);
            ptr_addr = &(ipv6->sin6_addr);
            ++num_of_addr;
        }

        // convert addresses from binary to text
        inet_ntop(ptr_p->ai_family, ptr_addr, ip_str, sizeof(ip_str));
        std::cout << "(" << num_of_addr << ") " << ip_ver << ": " << ip_str << std::endl;

        // move to the next addr
        ptr_p = ptr_p->ai_next;
    }

    // if no addr found
    if (!num_of_addr) {
        std::cout << "Found no valid host address\n";
        freeaddrinfo(ptr_res);
        return -3;
    }

    // ask user to select an address
    std::cout << "Enter host address number to bind with: ";
    uint32_t choice = 0;

    while (1) {
        if (!(std::cin >> choice)) {
            std::cout << "Failed to read!\n";
            std::cin.clear();
            // ignore rest of the line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (choice > (num_of_addr + 1) || choice < 1) {
            std::cout << "Wrong choice, please enter address number again: ";
        } else {
            break;
        }
    };

    ptr_p = ptr_res;

    // create new socket
    int sock_fd = socket(ptr_p->ai_family, ptr_p->ai_socktype, ptr_p->ai_protocol);
    if (sock_fd == -1) {
        std::cerr << "Error occured while creating socket!\n";
        freeaddrinfo(ptr_res);
        return -4;
    }

    // bind address to socket that was created
    int bind_r = bind(sock_fd, ptr_p->ai_addr, ptr_p->ai_addrlen);
    if (bind_r == -1) {
        std::cerr << "Error while binding socket!\n";
        close(sock_fd);  // close socket
        freeaddrinfo(ptr_res);
        return -5;
    }

    // start listening to socket connections
    int listen_r = listen(sock_fd, num_conn);
    if (listen_r == -1) {
        std::cerr << "Error while listening to the socket!\n";
        close(sock_fd);  // close socket
        freeaddrinfo(ptr_res);
        return -6;
    }

    // structure to hold client's address
    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    const std::string response{"Hi, how are you?"};

    // infinite loop to communicate with incoming connections
    // this connection takes client connections one at a time (sequential)
    // it is possible to create parallel connections using fork()
    while (1) {
        // socket descriptor
        int new_fd = accept(sock_fd, (sockaddr *)&client_addr, &client_addr_size);

        if (new_fd == -1) {
            std::cerr << "Error while acception on socket\n";
            continue;
        }

        // number of bytes sent
        auto bytes_sent = send(new_fd, response.data(), response.length(), 0);
        close(new_fd);
    }

    close(sock_fd);
    freeaddrinfo(ptr_res);

    return 0;
}