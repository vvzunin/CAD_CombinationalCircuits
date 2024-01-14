#include <string>
#include <iostream>
#include <vector>
#include <thread>

#include "YosysUtils.h"


// declare of value of static var
inline std::string YosysUtils::d_utilWord = "yosys";
inline std::string YosysUtils::d_className = "YosysUtils";
inline int YosysUtils::d_utilLen = 7;


inline void YosysUtils::standartExecutor(
            std::string i_command,
            std::vector<StandartCommandInfo> i_info, 
            void (*i_onFinish) (bool)) 
{
    FILE *abcOutput;
    char out[80];
    bool correct = true;

    // executing i_command with own read stream
    abcOutput = popen(i_command.c_str(), "r");

    std::string result;
    // parsing output
    while (fgets(out, sizeof(out), abcOutput)) {
        result += out;
    }

    // looking for each i_command execution
    int firstPos = result.find(d_utilWord, 0);

    // if there was an error
    if (firstPos != std::string::npos) {
        for (auto currentCommand : i_info) {
            int secondPos = result.find(d_utilWord, firstPos + 1);

            int errorPos = result.find(currentCommand.incorrectWord, firstPos);

            // if there was an error
            if (secondPos == std::string::npos && errorPos == std::string::npos) {
                std::cout << "Something went wrong during files parsing in " << d_className << '\n';
                correct = false;
                break;
            }
            
            // If something went wrong during read
            // and abc wrote something
            if (errorPos != std::string::npos && (errorPos < secondPos || secondPos == std::string::npos)) {
                int endPos = secondPos - errorPos - 1;
                if (secondPos == std::string::npos)
                    endPos = result.size() - errorPos - 1;

                std::cerr << "Incorrect " << currentCommand.info << ": " << result.substr(errorPos, endPos) << '\n';
                correct = false;
                break;
            }

            firstPos = secondPos;
        }
    }
    else {
        std::cout << "Something went wrong during files parsing in " << d_className << '\n';
        correct = false;
    }

    if (i_onFinish)
        i_onFinish(correct);
}


inline std::vector<StandartCommandInfo> YosysUtils::parseCommand(
    std::string i_command,
    bool i_parseAll)
{
        // we use the fact, that each i_command is surrounded by "
    int end, start;
    std::vector<StandartCommandInfo> info;

    start = i_command.find('"');
    end = i_command.find('"', start + 1);
    
    while (start != std::string::npos) {
        int commandNameStart = i_command.find_first_not_of(" ", start + 1);
        int commandNameEnd = i_command.find(" ", commandNameStart);

        // sumLen: end is bigger on 1 than real i_command ending, start is on 1 smaller,
        // but we have \n in the end, so we do not add 1
        // info: we use substr, "commandNameEnd - commandNameStart + 1" will show
        // i_command name with a space
        StandartCommandInfo commandInfo = {
            .sumLen = end - start,
            .info = i_command.substr(commandNameStart, commandNameEnd - commandNameStart)
        };

        if (i_parseAll) {
            commandInfo.incorrectWord = "ERROR";
        }

        info.push_back(commandInfo);
        
        // next two "
        start = i_command.find('"', end + 1);
        end = i_command.find('"', start + 1);
    }

    return info;
}


inline std::thread YosysUtils::optVerilog(
    std::string i_inputFileName,
    std::string i_outputFileName,
    void (*i_onFinish) (bool)) 
{
    // format i_command, then execute it with specified parametrs
    // REMEMBER 
    std::string i_command = "(echo \"read_verilog " + i_inputFileName + "\"";
    i_command += "&& echo \"opt\" && echo \"";
    i_command += "write_verilog " + i_outputFileName + "\") | yosys 2>&1";
    
    std::thread threadExecutor(
        standartExecutor, 
        i_command, 
        parseCommand(i_command, true),
        i_onFinish
    );

    return threadExecutor;
}

inline std::thread YosysUtils::optVerilog(
    std::string i_inputFileName, 
    std::string i_outputFileName,
    std::string i_directory,
    void (*i_onFinish) (bool))
{
    if (i_directory[i_directory.size() - 1] != '/')
        i_directory += "/";

    return optVerilog(i_directory + i_inputFileName, i_directory + i_outputFileName, i_onFinish);
}
