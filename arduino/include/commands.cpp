#ifndef __COMMANDS__
#define __COMMANDS__
#include "commands.hpp"
#include "md5.hpp"
#if IS_NOT_CONTROLLER
#include <iostream>
#else
#include <EEPROM.h>
#include <MD5.h>
#endif

namespace commands {
    Statuses CommandsInterface::execute(const STRING_TYPE& s, Commands * executedCommand) {
        if (!isHashValid(s)) {
            return Statuses::HASH_FAILED;
        }
        auto command = extractCommand(s);
        if (executedCommand != nullptr) {
            *executedCommand = command;
        }
        return execute(command);
    }
    Statuses CommandsInterface::execute(const Packet& s, Commands * executedCommand) {
        if (executedCommand != nullptr) {
            *executedCommand = Commands::UNDEFINED;
        }
        return Statuses::NO_COMMAND; // TODO: Write conditions for commands
    }
    Statuses CommandsInterface::execute(const Commands& state) {
        switch (state)
        {
        case Commands::RESET:
            reset();
            break;
        case Commands::FORCE_START_CAMERA:
            sensors::Sensors::startCamera(true);
            break;
        case Commands::START_CAMERA:
            sensors::Sensors::startCamera();
            break;
        case Commands::START_RECORDING:
            sensors::Sensors::startRecording();
            break;
        case Commands::STOP_RECORDING:
            sensors::Sensors::stopRecording();
            break;
        default:
            return Statuses::NO_COMMAND;
        }
        return Statuses::OK;
    }
    bool CommandsInterface::isHashValid(const STRING_TYPE& s) {
    /*
        Command string is structured like that: 
        command_id|payload|hash
        For checking hash, we're dividing message by last delimiter (|), and getting command_id|payload and hash
        Then we are calculating hash of first and checking if hashes are equal
    */
    #if IS_NOT_CONTROLLER
        auto lastDelimiterIndex = s.find_last_of(delimiter);
        auto hash = s.substr(lastDelimiterIndex + 1, s.size());
        auto body = s.substr(0, lastDelimiterIndex);
        md5 hashGenerator(body.begin(), body.end());
        return hashGenerator.hex_digest<STRING_TYPE>() == hash;
    #else
        auto lastDelimiterIndex = s.lastIndexOf(delimiter);
        auto hash = s.substring(lastDelimiterIndex + 1, s.length());
        auto body = s.substring(0, lastDelimiterIndex);
        
        unsigned char* h_c = MD5::make_hash(const_cast<char*>(body.c_str()));
        char* r = MD5::make_digest(h_c, 16);
        free(h_c);
        STRING_TYPE h = r;
        free(r);

        return h == hash;
    #endif
    
    }
    Commands CommandsInterface::extractCommand (const STRING_TYPE& s) {
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
            case (int)Commands::RESET:
                return Commands::RESET;
            case (int)Commands::START_CAMERA:
                return Commands::START_CAMERA;
            case (int)Commands::START_RECORDING:
                return Commands::START_RECORDING;
            case (int)Commands::STOP_RECORDING:
                return Commands::STOP_RECORDING;
            case (int)Commands::FORCE_START_CAMERA:
                return Commands::FORCE_START_CAMERA;
            default:
                return Commands::UNDEFINED;
        }
        return Commands::UNDEFINED;
    }

    void CommandsInterface::reset() {
    #if IS_CONTROLLER
        sensors::Sensors::reset();
    #else
        std::cout << "Reseting" << std::endl;
    #endif
    }

}
#endif