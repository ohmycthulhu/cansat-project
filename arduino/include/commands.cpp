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
    Statuses CommandsInterface::execute(const STRING_TYPE& s) {
        if (!isHashValid(s)) {
            return Statuses::HASH_FAILED;
        }
        Commands c = extractCommand(s);
        return execute(c);
    }
    Statuses CommandsInterface::execute(const Packet& s) {
        return Statuses::NO_COMMAND; // TODO: Write conditions for commands
    }
    Statuses CommandsInterface::execute(const Commands& state) {
        switch (state)
        {
        case Commands::RESET:
            reset();
            return Statuses::OK;

        default:
            return Statuses::NO_COMMAND;
        }
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
        if (i & (int)Commands::RESET) {
            return Commands::RESET;
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