#ifndef _ARGS_HELPER_H
#define _ARGS_HELPER_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>

class ArgsHelper {
public:
    ArgsHelper(int argc, char* argv[]) {
        if (argc != 3) {
            print_usage();
            valid_ = false;
            return;
        }

        host_ = argv[1];
        std::string port_str = argv[2];

        if (!is_valid_ip(host_)) {
            std::cerr << "Error: dirección IP inválida -> " << host_ << "\n";
            valid_ = false;
            return;
        }

        if (!str_to_int(port_str, port_)) {
            std::cerr << "Error: puerto inválido -> " << port_str << "\n";
            valid_ = false;
            return;
        }

        if (port_ < 1 || port_ > 65535) {
            std::cerr << "Error: puerto fuera de rango (1–65535)\n";
            valid_ = false;
            return;
        }

        valid_ = true;
    }

    bool is_valid() const { return valid_; }
    std::string host() const { return host_; }
    int port() const { return port_; }

    static void print_usage() {
        std::cout << "Uso: tcpip_tool <IP> <puerto>\n";
        std::cout << "Ejemplo: tcpip_tool 192.168.0.10 8080\n";
    }

private:
    bool valid_ = false;
    std::string host_;
    int port_ = 0;

    static bool is_valid_ip(const std::string& ip_str) {
        try {
            boost::asio::ip::make_address(ip_str);
            return true;
        } catch (...) {
            return false;
        }
    }

    static bool str_to_int(const std::string& s, int& out) {
        try {
            size_t idx;
            int val = std::stoi(s, &idx);
            if (idx != s.size()) return false;
            out = val;
            return true;
        } catch (...) {
            return false;
        }
    }
};

#endif
