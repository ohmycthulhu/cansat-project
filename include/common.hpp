#ifndef STRING_TYPE
    #ifdef __GNUC__
        #include <string.h>
        #define STRING_TYPE std::string
    #else
        #define STRING_TYPE string
    #endif
#endif

#ifndef IS_CONTROLLER
    #ifdef __GNUC__
        #define IS_CONTROLLER false
    #else
        #define IS_CONTROLLER true
    #endif
    #define IS_NOT_CONTROLLER !IS_CONTROLLER
#endif