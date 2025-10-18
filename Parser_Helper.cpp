#include <fstream>

std::vector<uint8_t> Parse_Helper::read_binary_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    return std::vector<uint8_t>(std::istreambuf_iterator<char>(file),
                                std::istreambuf_iterator<char>());
}
