#ifndef _INPUTSOURCE_H
#define _INPUTSOURCE_H

class InputSource {
public:
    virtual ~InputSource() = default;
    virtual bool get_next(std::string& out) = 0;
};

#endif
