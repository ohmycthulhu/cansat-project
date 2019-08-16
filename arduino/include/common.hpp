#ifndef __COMMON__
#define __COMMON__

#ifndef STRING_TYPE
    #ifdef linux
        #include <string>
        typedef std::string STRING_TYPE;
        // #define STRING_TYPE std::string
    #else
        typedef String STRING_TYPE;
    #endif
#endif

#ifndef IS_CONTROLLER
    #ifdef linux
        #define IS_CONTROLLER false
    #else
        #define IS_CONTROLLER true
    #endif
    #define IS_NOT_CONTROLLER !IS_CONTROLLER
#endif

#if IS_NOT_CONTROLLER
#include <stdlib.h>
#include <ctime>
#else
#include <SoftwareSerial.h>
#endif

inline float randomNumber(float max, float min = 0) {
#if IS_CONTROLLER
    return random(min * 100, max * 100) / 100.0f;
#else
    return rand() % ((int)(max - min) * 100) / 100.0f + min;
#endif
}

inline unsigned long getTime() {
#if IS_CONTROLLER
    return micros();
#else
    return (clock() * 1e6) / CLOCKS_PER_SEC;
#endif
}

#endif

