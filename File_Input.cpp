#include "File_Input.hpp"

FileInput::FileInput(const std::string& path) : file_(path) {
    
    if (!file_.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
}

bool FileInput::get_next(std::string& out) {
    if (!std::getline(file_, out))
        return false;  // fin de archivo
    return true;
}

