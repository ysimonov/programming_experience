#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace boost;
using namespace std::chrono_literals;

std::vector<char> vBuffer(20 * 1024);

// Async read function
void GrabSomeData(asio::ip::tcp::socket& socket) {
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
                           [&](std::error_code ec, std::size_t length) {
                               if (!ec) {
                                   std::cout << "\n\nRead" << length
                                             << " bytes\n\n";

                                   for (int i = 0; i < length; ++i) {
                                       std::cout << vBuffer[i];
                                   }

                                   GrabSomeData(socket);
                               }
                           });
}

inline void wait_on_enter() {
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
    std::getline(std::cin, dummy);
}

int main() {
    system::error_code ec;

    // "context" is the platform-specific interface
    asio::io_context context;

    // give asio some fake job to do to stop it from exiting immediately
    asio::io_context::work idleWork(context);

    // start the context in its own thread
    std::thread thrContext = std::thread([&]() { context.run(); });

    // get the address of somewhere we want to connect (TCP) - IP + PORT
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec),
                                     80);

    if (!ec) {
        std::cout << "Address " << endpoint.address() << " and port "
                  << endpoint.port() << " are available." << std::endl;
    } else {
        std::cout << "Address " << endpoint.address() << " and/or port "
                  << endpoint.port() << " are occupied:\n"
                  << ec.message() << std::endl;
        return EXIT_FAILURE;
    }

    // create a socket, the context will deliver implementation
    // (networking socket)
    asio::ip::tcp::socket socket(context);

    // tell socket to try to connect
    socket.connect(endpoint, ec);

    if (!ec) {
        std::cout << "Connected!" << std::endl;
    } else {
        std::cout << "Failed to connect to address:\n"
                  << ec.message() << std::endl;
        return EXIT_FAILURE;
    }

    if (socket.is_open()) {
        // start sock async before starting request
        GrabSomeData(socket);

        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: david-barr.co.uk\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        std::this_thread::sleep_for(20000ms);
    }

    // press any key to continue
    wait_on_enter();
    return EXIT_SUCCESS;
}