#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
using ip::tcp;

// compile with -pthread flag

int main() {
    boost::asio::io_service io_service;
    // create socket
    tcp::socket socket(io_service);
    // connect to end point
    std::string id_address{"127.0.0.1"};
    uint16_t port_number = 1234;
    // create TCP endpoint associated with the socket
    auto endpoint = tcp::endpoint(boost::asio::ip::address::from_string(id_address), port_number);
    // connect socket to a specified endpoint
    socket.connect(endpoint);
    // send a message to server
    const std::string msg{"Hello from Client!\n"};
    const auto buffered_msg{boost::asio::buffer(msg)};
    boost::system::error_code error;
    boost::asio::write(socket, buffered_msg, error);
    if (!error) {
        std::cout << "Message was successfully delivered!\n";
    }
    // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        std::cout << "Failed to receive a message from the server!\n";
    } else {
        const std::string data = boost::asio::buffer_cast<const char *>(receive_buffer.data());
        std::cout << "Received a message from the server: " << data << std::endl;
    }
    return EXIT_SUCCESS;
}