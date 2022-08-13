#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <ctime>

using boost::asio::ip::tcp;


//! TODO: figure out why "assign: Bad file descriptor" appears during execution

std::string getDateTime() {
    char datetime_string[200];

    time_t curr_time;
    std::tm* curr_tm = nullptr;

    time(&curr_time);
    curr_tm = localtime(&curr_time);

    strftime(datetime_string, 100, "Date %B %d, %Y | Time %T", curr_tm);

    auto sdatetime {std::string(datetime_string)};
    delete curr_tm;
    curr_tm = nullptr;

    return sdatetime;
}

// enable_shred_from_this allows to keep the TCP Connection alive
class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {

    public: 

        TCPConnection(boost::asio::io_context& io_context)
        : socket_(io_context) {}

        typedef boost::shared_ptr<TCPConnection> pointer;

        static pointer create(boost::asio::io_context& io_context) {
            return pointer(new TCPConnection(io_context));
        }
    
        tcp::socket& socket() {
            return socket_;
        }

        void start();
    
    private:

        void handleWrite(const boost::system::error_code& /*error*/,
            size_t /*bytes transferred*/) {}

        tcp::socket socket_;
        std::string message_;
};



void TCPConnection::start() {

    // timestamp
    message_ = getDateTime();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&TCPConnection::handleWrite, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

}

class TCPServer {

    public:
        TCPServer(boost::asio::io_context& io_context, const uint16_t port_number)
        : io_context_(io_context), acceptor_(io_context, tcp::v4(), port_number) {
            startAccept();
        }

    private:
        void startAccept();
        void handleAccept(TCPConnection::pointer /*new connection*/, 
            const boost::system::error_code& /*error*/);

        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;
};

void TCPServer::startAccept() {

    // socket connection
    TCPConnection::pointer new_connection = TCPConnection::create(io_context_);

    // asynchronous accept, waiting for a new connection
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&TCPServer::handleAccept, this, new_connection,
        boost::asio::placeholders::error));
}

void TCPServer::handleAccept(
    TCPConnection::pointer new_connection, 
    const boost::system::error_code& error
) {
    if (!error) {
        new_connection->start();
    }
    startAccept();
}

int main() {

    boost::asio::io_context io_context;
    static const uint16_t port_number = 1234;

    try {

        auto server = TCPServer(io_context, port_number);
        io_context.run();

    } catch (std::exception& e) {

        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}