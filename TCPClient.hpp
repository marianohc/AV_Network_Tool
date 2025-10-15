#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <memory>

class TcpClient {
public:
    using ReceiveCallback = std::function<void(const std::string&)>;

    TcpClient();
    ~TcpClient();

    void connect(const std::string& host, unsigned short port);
    void start();
    void stop();

    void send(const std::vector<uint8_t>& data);
    void send(const std::string& data);

    void set_receive_callback(ReceiveCallback cb);
    bool is_connected() const;

private:
    void on_connect(const boost::system::error_code& ec);
    void do_async_read();
    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);

    boost::asio::io_context ioctx_;
    boost::asio::ip::tcp::socket socket_;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> work_guard_;
    std::thread io_thread_;
    std::atomic<bool> running_{false};
    std::vector<uint8_t> recv_buffer_;
    ReceiveCallback rx_callback_;
};


/*
#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <boost/asio.hpp>

// A simple, reusable TCP client class built on Boost.Asio.
// - connect(host, port) -> synchronous connect, returns bool success
// - start() -> starts internal io_context thread and begins async receive
// - send(data) -> posts an async write to the socket (thread-safe)
// - stop() -> stops io_context and joins thread
// - set_receive_callback(cb) -> callback invoked on received data
//
// The implementation below is intentionally minimal but production-oriented:
// it uses an internal io_context with a work guard, async reads in a loop,
// and posts async_writes to the io_context so send() may be called from any thread.

class TcpClient {
public:
    using ReceiveCallback = std::function<void(const std::string&)>;

    TcpClient();
    ~TcpClient();

    // Non-copyable
    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    // Connect synchronously to host:port. Returns true on success.
    // This should be called before start() in the simple usage pattern below.
    bool connect(const std::string& host, unsigned short port, std::string& err_out);

    void on_connect(const boost::system::error_code& ec);

    // Start internal io thread and begin async operations.
    void start();

    // Stop IO and join thread. Safe to call multiple times.
    void stop();

    // Send data (binary-safe). This posts the write to the internal io_context,
    // so it is safe to call from the main thread or any other thread.
    void send(const std::vector<uint8_t>& data);
    void send(const std::string& data);

    // Set the callback invoked when data is received. The callback will be
    // invoked from the internal IO thread. If you need to update UI or main-thread
    // state, dispatch to the main thread from the callback.
    void set_receive_callback(ReceiveCallback cb);

    // Check connection status
    bool is_connected() const;

private:
    void do_async_read();
    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
    boost::asio::io_context ioctx_;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> work_guard_;
    boost::asio::ip::tcp::socket socket_;
    std::thread io_thread_;
    std::atomic<bool> running_{false};

    // receive buffer
    std::vector<uint8_t> recv_buffer_;
    ReceiveCallback rx_callback_;
};

#endif
*/