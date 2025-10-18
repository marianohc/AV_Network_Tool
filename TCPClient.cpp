#include "TCPClient.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "Helper_ANSI.hpp"

using boost::asio::ip::tcp;

TcpClient::TcpClient()
    : ioctx_(), socket_(ioctx_), recv_buffer_(1024), connected_to_server{false}, last_message_sent{false}
{
    work_guard_ = std::make_unique<
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(ioctx_.get_executor());
}

TcpClient::~TcpClient() {
    stop();
}

void TcpClient::connect(const std::string& host, unsigned short port) {
    tcp::resolver resolver(ioctx_);
    auto endpoints = resolver.resolve(host, std::to_string(port));

    // Iniciar conexión asíncrona
    boost::asio::async_connect(socket_, endpoints,
        [this](const boost::system::error_code& ec, const tcp::endpoint&) {
            on_connect(ec);
        });
}

void TcpClient::on_connect(const boost::system::error_code& ec) {
    if (!ec) {

        std::cout << ansi::clear;
        std::cout << ansi::green << "Conectado al servidor." << ansi::white << std::endl << std::endl;
        
        // Activar TCP keep-alive
        boost::asio::ip::tcp::socket::keep_alive option(true);
        socket_.set_option(option);

#ifdef __linux__
        // Ajustar parámetros del keep-alive (opcional)
        int fd = socket_.native_handle();
        int enable = 1, idle = 10, interval = 5, count = 3;
        setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
        setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
        setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
        setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
#endif

        // Arrancar el ciclo de lectura
        do_async_read();
    } else {
        std::cerr << "[TcpClient] Error al conectar: " << ec.message() << "\n";
    }

    // DEsbloqueo el main thread porque ya esta conectado.
    connected_to_server = true;
}

void TcpClient::start() {
    if (running_.exchange(true)) return;

    io_thread_ = std::thread([this] {
        try {
            ioctx_.run();
        } catch (const std::exception& e) {
            std::cerr << "[TcpClient] io_context.run() exception: " << e.what() << "\n";
        }
    });
}

void TcpClient::stop() {
    if (!running_.exchange(false)) return;

    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close(ec);
    work_guard_.reset();
    ioctx_.stop();

    if (io_thread_.joinable()) io_thread_.join();
}

void TcpClient::send(const std::vector<uint8_t>& data) {

    std::cout << "Aca 02" << std::endl;
    
    if (!running_ || !socket_.is_open()) return;

    for (auto b : data) {
        std::cout << b << std::endl;
    }

    auto buf = std::make_shared<std::vector<uint8_t>>(data);

    
    boost::asio::post(ioctx_, [this, buf]() {
        boost::asio::async_write(socket_, boost::asio::buffer(*buf),
            [this, buf](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                handle_write(ec, bytes_transferred);
            });
    });
}

void TcpClient::send(const std::string& data) {
    std::cout << "Aca 01" << std::endl;
    if (!running_ || !socket_.is_open()) return;

    auto buf = std::make_shared<std::string>(data);

    boost::asio::post(ioctx_, [this, buf]() {
        boost::asio::async_write(socket_, boost::asio::buffer(*buf),
            [this, buf](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                handle_write(ec, bytes_transferred);
            });
    });
}

void TcpClient::set_receive_callback(ReceiveCallback cb) {
    rx_callback_ = std::move(cb);
}

bool TcpClient::is_connected() const {
    return socket_.is_open();
}

void TcpClient::do_async_read() {
    if (!socket_.is_open()) return;

    socket_.async_read_some(boost::asio::buffer(recv_buffer_),
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handle_read(ec, bytes_transferred);
        });
}

void TcpClient::handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    if (ec) {
        if (ec != boost::asio::error::eof) {
            std::cerr << "[TcpClient] read error: " << ec.message() << "\n";
            stop();
        } 
        return;
    }

    if (bytes_transferred > 0) {
        std::string s(reinterpret_cast<const char*>(recv_buffer_.data()), bytes_transferred);
        if (rx_callback_) {
            try {
                rx_callback_(s);
            } catch (const std::exception& e) {
                std::cerr << "[TcpClient] receive callback threw: " << e.what() << "\n";
            }
        }
    }

    do_async_read(); // continuar leyendo
}


void TcpClient::handle_write(const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {

    if (ec) {
        std::cerr << "[TcpClient] write error: " << ec.message() << "\n";
    } else {
        last_message_sent = true;
    }
}


