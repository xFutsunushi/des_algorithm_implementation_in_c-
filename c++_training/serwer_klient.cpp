#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void runServer() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));

    while (true) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        std::string message = "Hello from server!\n";

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
}

void runClient() {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "12345");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    while (true) {
        boost::asio::streambuf buffer;
        boost::system::error_code error;
        boost::asio::read_until(socket, buffer, "\n", error);

        if (error == boost::asio::error::eof)
            break; // Połączenie zamknięte
        else if (error)
            throw boost::system::system_error(error);

        std::istream is(&buffer);
        std::string message;
        std::getline(is, message);
        std::cout << "Received from server: " << message << "\n";
    }
}

int main() {
    char choice;
    std::cout << "Uruchomić jako serwer (s) czy klient (k)? ";
    std::cin >> choice;

    if (choice == 's') {
        runServer();
    } else if (choice == 'k') {
        runClient();
    } else {
        std::cout << "Nieprawidłowy wybór.\n";
    }

    return 0;
}
