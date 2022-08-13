#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

// compile with -pthread flag

/*
Server receives messages from the client
and responds back.
*/
std::string readMessage(tcp::socket& socket) {
    // buffer of the data that is being communicated
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    // convert data to std::string
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    return data;
}

void sendMessage(tcp::socket& socket, const std::string& message) {
    const std::string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}

int main() {
    // io object is needed whenever program is using asio
    boost::asio::io_service io_service;
    // listen for a new connection
    uint16_t port_number = 1234;
    // tcp v4 indicates IPv4 connection on port 1234
    auto tcp_endpoint = tcp::endpoint(tcp::v4(), port_number);
    // acceptor is used to listen for connection request by the client
    tcp::acceptor acceptor(io_service, tcp_endpoint);
    // create a socket
    tcp::socket socket(io_service);
    // wait for a connection
    acceptor.accept(socket);
    // read response
    std::string message = readMessage(socket);
    // write response
    sendMessage(socket, "Hello from Server!");
    return EXIT_SUCCESS;
}
