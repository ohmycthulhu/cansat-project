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

#define CASE_COMMAND(X,Y) case (int)X::Y: return X::Y;

namespace commands {

    const static char delimiter = '|';
    bool forceShutUpBuzzer = false;

    void reset();
    Commands extractCommand (const STRING_TYPE& s);
    // bool isHashValid(const STRING_TYPE& s);
    Statuses execute(const Commands& state);

    Statuses execute(const STRING_TYPE& s, Commands * executedCommand) {
        /*
        if (false && !isHashValid(s)) {
            return Statuses::HASH_FAILED;
        }
        */
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
        if (s.getHeight() < 30 && s.getState() == 1) {
            if (!forceShutUpBuzzer) {
                sensors::startBuzzer();
            }
        } else {
            forceShutUpBuzzer = false;
            sensors::stopBuzzer();
        }
        return Statuses::NO_COMMAND; // TODO: Write conditions for commands
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
    /*
        Hash check is `temporary` disabled
    bool isHashValid(const STRING_TYPE& s) {
    /*
        Command string is structured like that: 
        command_id|payload|hash
        For checking hash, we're dividing message by last delimiter (|), and getting command_id|payload and hash
        Then we are calculating hash of first and checking if hashes are equal
    */
   /*
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
    
    }*/
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