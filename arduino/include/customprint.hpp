#ifndef __CUSTOMPRINT_
#define __CUSTOMPRINT
#include "common.hpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#endif

class CustomPrint {
public:
    template<typename T>
    inline const CustomPrint& operator<<(const T& value) const {
    #if IS_CONTROLLER
        Serial.print(value);
    // Print for controller
    #else
        std::cout << value;
    #endif
        return *this;
    }
};

#endif