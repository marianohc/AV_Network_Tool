#pragma once

#include <string>

class AppContext {
public:
    void set_token_id(const std::string& token) { m_token_id = token; }
    const std::string& get_token_id() const { return m_token_id; }

    void set_server_ip(const std::string& ip) { server_ip_ = ip; }
    const std::string& server_ip() const { return server_ip_; }

    void set_server_port(uint16_t port) { server_port_ = port; }
    uint16_t server_port() const { return server_port_; }

    AppContext() {
        m_token_id = "No definido";
    } 


private:
    std::string m_token_id;
    std::string server_ip_;
    uint16_t server_port_ = 0;
};
