#include "common.hpp"
#include "packet.cpp"
#if IS_NOT_CONTROLLER
#include <string.h>
#endif

namespace commands {

enum class Statuses {
    OK,
    FAILED,
    HASH_FAILED,
    NO_COMMAND
};

enum class Commands: int {
    DETACH = 0b01,
    UNDEFINED = 0b00
};

class CommandsInterface {
private:
    const static char delimiter = '|';
    static void detach();

    static bool isHashValid(const STRING_TYPE& s);
    static Commands extractCommand (const STRING_TYPE& s);
    static Statuses execute(const Commands& command);
public:
    static Statuses execute(const STRING_TYPE& s);
    static Statuses execute (const Packet& state);
    
}; 

}