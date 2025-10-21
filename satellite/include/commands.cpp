#ifndef __COMMANDS__
#define __COMMANDS__
#include "commands.hpp"
#if IS_NOT_CONTROLLER
#include <iostream>
#else
#include <EEPROM.h>
#include <MD5.h>
#endif

#define CASE_COMMAND(X,Y) case (int)X::Y: return X::Y;

namespace commands {

    const static char delimiter = '|';
    bool forceShutUpBuzzer = false;

    void reset();
    Commands extractCommand (const STRING_TYPE& s);
    Statuses execute(const Commands& state);

    Statuses execute(const STRING_TYPE& s, Commands * executedCommand) {
        auto command = extractCommand(s);
        if (executedCommand != nullptr) {
            *executedCommand = command;
        }
        return execute(command);
    }
    Statuses execute(const Packet& s, Commands * executedCommand) {
        if (executedCommand != nullptr) {
            *executedCommand = Commands::UNDEFINED;
        }
        if (s.height < 30 && s.satState == 1) {
            if (!forceShutUpBuzzer) {
                sensors::startBuzzer();
            }
        } else {
            forceShutUpBuzzer = false;
            sensors::stopBuzzer();
        }
        return Statuses::NO_COMMAND;
    }
    Statuses execute(const Commands& state) {
        switch (state)
        {
        case Commands::RESET:
            reset();
            break;
        case Commands::FORCE_START_CAMERA:
            sensors::startCamera(true);
            break;
        case Commands::START_CAMERA:
            sensors::startCamera();
            break;
        case Commands::START_RECORDING:
            sensors::startRecording();
            break;
        case Commands::STOP_RECORDING:
            sensors::stopRecording();
            break;
        case Commands::SHUT_UP_BUZZER:
            forceShutUpBuzzer = true;
            sensors::stopBuzzer();
            break;
        case Commands::SOFT_RESET_CAMERA:
            sensors::isPowered = false;
            sensors::isRecording = false;
            EEPROM.write(sensors::cameraStateAddress, 0);
            break;
        default:
            return Statuses::NO_COMMAND;
        }
        return Statuses::OK;
    }

    Commands extractCommand (const STRING_TYPE& s) {
    /*
        Command string is structured like that: 
        command_id|payload|hash
        For extracting command, we're dividing message by first delimiter (|)
        and checking string part before first delimiter
    */
   #if IS_NOT_CONTROLLER
        auto firstDelimiterIndex = s.find(delimiter);
        STRING_TYPE commandId = s.substr(0, firstDelimiterIndex);
        int i = atoi(commandId.c_str());
    #else
        int i = s.substring(0, s.indexOf(delimiter)).toInt();
    #endif
        switch(i) {            
            CASE_COMMAND (Commands, RESET)
            CASE_COMMAND (Commands, START_CAMERA)
            CASE_COMMAND (Commands, START_RECORDING)
            CASE_COMMAND (Commands, STOP_RECORDING)
            CASE_COMMAND (Commands, FORCE_START_CAMERA)
            CASE_COMMAND (Commands, SHUT_UP_BUZZER)
            CASE_COMMAND (Commands, SOFT_RESET_CAMERA)
            default:
                return Commands::UNDEFINED;
        }
        return Commands::UNDEFINED;
    }

    void reset() {
    #if IS_CONTROLLER
        sensors::reset();
    #else
        std::cout << "Reseting" << std::endl;
    #endif
    }

}
#endif