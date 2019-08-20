#include "common.hpp"
#include "packet.cpp"
#if IS_NOT_CONTROLLER
#include <string.h>
#endif

namespace commands {

enum class Statuses {
    OK, // 0
    FAILED, // 1
    HASH_FAILED, // 2
    NO_COMMAND // 3
};

enum class Commands: int {
    RESET = 1, // 1
    START_CAMERA, // 2
    START_RECORDING, // 3
    STOP_RECORDING, // 4
    FORCE_START_CAMERA, // 5
    SHUT_UP_BUZZER, // 6
    SOFT_RESET_CAMERA, // 7
    UNDEFINED = 0
};

// First operand will be used for defining the right command, the second for returning executed command
static Statuses execute(const STRING_TYPE& s, Commands * executedCommand = nullptr);
static Statuses execute (const Packet& state, Commands * executedCommand = nullptr); 

}