#include "Keyboard_Input.hpp"


bool KeyboardInput::get_next(std::string& out) {
    
    if (!std::getline(std::cin, out))
        return false;  // fin de entrada
    return true;
}

