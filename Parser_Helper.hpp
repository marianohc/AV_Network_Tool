#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>

class Parse_Helper {

public:
    static std::vector<uint8_t> parse_hex_string(const std::string& input) {
        std::vector<uint8_t> bytes;
        std::istringstream iss(input);
        std::string token;

        while (iss >> token) {
            if (token.size() != 2 || !std::isxdigit(token[0]) || !std::isxdigit(token[1])) {
                throw std::runtime_error("Formato inválido. Use pares hexadecimales separados por espacios (ej: '01 0A FE').");
            }
            uint8_t byte = static_cast<uint8_t> (std::stoi(token, nullptr, 16));
            bytes.push_back(byte);
        }

        std::cout << "El tamanio es: " << bytes.size() << std::endl;
        return bytes;
    }

    static std::vector<uint8_t> read_binary_file(const std::string& filename);
};
