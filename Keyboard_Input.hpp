#ifndef _KEYBOARD_INPUT_H
#define _KEYBOARD_INPUT_H

#include <string>
#include <fstream>
#include <iostream>

#include "File_Input.hpp"

class KeyboardInput : public InputSource {
public:
    bool get_next(std::string& out) override;
};


#endif
