#ifndef _FILE_STREAM_H
#define _FILE_STREAM_H

#include <string>
#include <fstream>
#include <iostream>
#include "InputSource.hpp"

class FileInput : public InputSource {
public:
    explicit FileInput(const std::string& path);
    bool get_next(std::string& out) override;

private:
    std::ifstream file_;
};


#endif
