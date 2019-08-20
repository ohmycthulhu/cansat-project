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
    RESET = 1, // 1
    START_CAMERA, // 2
    START_RECORDING, // 3
    STOP_RECORDING, // 4
    FORCE_START_CAMERA, // 5
    SHUT_UP_BUZZER, // 6
    UNDEFINED = 0
};

class CommandsInterface {
private:
    const static char delimiter = '|';
    static void reset();

    static bool isHashValid(const STRING_TYPE& s);
    static Commands extractCommand (const STRING_TYPE& s);
    static Statuses execute(const Commands& command);
    static bool forceShutUpBuzzer;
public:
    // First operand will be used for defining the right command, the second for returning executed command
    static Statuses execute(const STRING_TYPE& s, Commands * executedCommand = nullptr);
    static Statuses execute (const Packet& state, Commands * executedCommand = nullptr);
    
}; 

}