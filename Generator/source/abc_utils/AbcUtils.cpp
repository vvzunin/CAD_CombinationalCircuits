#include <string>
#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <thread>

#include "AbcUtils.h"

// declare of value of static var
inline const std::string AbcUtils::abc_word = "abc ";

// TODO at this moment can execute only three commands, in fact one operation
// make it to read more, if it would ne neccessary
inline void AbcUtils::standartExecutor(
            std::string command,
            std::vector<StandartCommandInfo> info, 
            void (*on_finish) (bool)) 
{
    FILE *abc_output;
    char out[40];
    bool correct = true;

    // executing command with own read stream
    abc_output = popen(command.c_str(), "r");

    std::string result;
    // parsing output
    while (fgets(out, sizeof(out), abc_output)) {
        result += out;
    }

    // looking for each command execution
    int first_pos = result.find(abc_word, 0);

    // if there was an error
    if (first_pos != std::string::npos) {
        for (auto currentCommand : info) {
            int second_pos = result.find(abc_word, first_pos + 1);

            // if there was an error
            if (second_pos == std::string::npos) {
                std::cout << "Something went wrong during files parsing in AbcUtils\n";
                correct = false;
                break;
            }

            int delta = abc_len + currentCommand.sumLen;
            
            // If something went wrong during read
            // and abc wrote something
            if (second_pos - first_pos > delta) {
                std::cerr << "Incorrect " << currentCommand.info << ": " << result.substr(first_pos + delta, second_pos - first_pos - delta) << '\n';
                correct = false;
                break;
            }

            first_pos = second_pos;
        }
    }
    else {
        std::cout << "Something went wrong during files parsing in AbcUtils\n";
        correct = false;
    }

    if (on_finish)
        on_finish(correct);
}

inline std::vector<StandartCommandInfo> AbcUtils::parseCommand(
            std::string command,
            bool parseAll) 
{
    // we use the fact, that each command is surrounded by "
    int end, start;
    std::vector<StandartCommandInfo> info;

    start = command.find('"');
    end = command.find('"', start + 1);
    
    while (start != std::string::npos) {
        int command_name_start = command.find_first_not_of(" ", start + 1);
        int command_name_end = command.find(" ", command_name_start);

        // sumLen: end is bigger on 1 than real command ending, start is on 1 smaller,
        // but we have \n in the end, so we do not add 1
        // info: we use substr, "command_name_end - command_name_start + 1" will show
        // command name with a space
        StandartCommandInfo commandInfo = {
            .sumLen = end - start,
            .info = command.substr(command_name_start, command_name_end - command_name_start)
        };

        info.push_back(commandInfo);
        
        // next two "
        start = command.find('"', end + 1);
        end = command.find('"', start + 1);
    }

    return info;
}

// TODO I know, that creating such structs is a bit ugly. So, I'm going
// tp fix it, but at this moment it's 22:06 of 31.12.2023, and I'm going to have a rest
inline std::thread AbcUtils::verilogToAiger(
    std::string i_inpuFileName, std::string i_outpuFileName, void (*on_finish) (bool)) 
{
    // format command, then execute it with specified parametrs
    std::string command = "(echo \"read_verilog " + i_inpuFileName + "\"";
    command += "&& echo \"strash\" && echo \"";
    command += "write_aiger " + i_outpuFileName + "\") | abc";
    
    std::thread thread_executor(
        standartExecutor, 
        command, 
        parseCommand(command),
        on_finish
    );

    return thread_executor;
}

inline std::thread AbcUtils::verilogToAiger(
    std::string i_inpuFileName, std::string i_outpuFileName, std::string i_directory, void (*on_finish) (bool)) 
{
    if (i_directory[i_directory.size() - 1] != '/')
        i_directory += "/";

    return verilogToAiger(i_directory + i_inpuFileName, i_directory + i_outpuFileName, on_finish);
}

inline std::thread AbcUtils::aigerToVerilog(
    std::string i_inpuFileName, std::string i_outpuFileName, void (*on_finish) (bool)) 
{
    std::string command = "(echo \"read_aiger " + i_inpuFileName + "\"";
    command += "&& echo \"strash\" && echo \"";
    command += "write_verilog " + i_outpuFileName + "\") | abc";
    
    std::thread thread_executor(
        standartExecutor, 
        command, 
        parseCommand(command),
        on_finish
    );

    return thread_executor;
}

inline std::thread AbcUtils::aigerToVerilog(
    std::string i_inpuFileName, std::string i_outpuFileName, std::string i_directory, void (*on_finish) (bool)) 
{
    if (i_directory[i_directory.size() - 1] != '/')
        i_directory += "/";

    return aigerToVerilog(i_directory + i_inpuFileName, i_directory + i_outpuFileName, on_finish);
}


inline std::thread AbcUtils::balanceVerilog(std::string i_inpuFileName, void (*on_finish) (bool)) {
    std::string command = "(echo \"read_verilog " + i_inpuFileName + "\"";
    command += "&& echo \"balance\" && echo \"";
    command += "write_verilog " + i_inpuFileName + "\") | abc";
    
    std::thread thread_executor(
        standartExecutor, 
        command, 
        parseCommand(command),
        on_finish
    );

    return thread_executor;
}

inline std::thread AbcUtils::balanceVerilog(std::string i_inpuFileName, std::string i_directory, void (*on_finish) (bool)) {
    if (i_directory[i_directory.size() - 1] != '/')
        i_directory += "/";

    return balanceVerilog(i_directory + i_inpuFileName, on_finish);
}

inline std::thread AbcUtils::balanceAiger(std::string i_inpuFileName, void (*on_finish) (bool)) {
    std::string command = "(echo \"read_aiger " + i_inpuFileName + "\"";
    command += "&& echo \"balance\" && echo \"";
    command += "write_aiger " + i_inpuFileName + "\") | abc";
    
    std::thread thread_executor(
        standartExecutor, 
        command, 
        parseCommand(command),
        on_finish
    );

    return thread_executor;
}

inline std::thread AbcUtils::balanceAiger(std::string i_inpuFileName, std::string i_directory, void (*on_finish) (bool)) {
    if (i_directory[i_directory.size() - 1] != '/')
        i_directory += "/";

    return balanceAiger(i_directory + i_inpuFileName, on_finish);
}
