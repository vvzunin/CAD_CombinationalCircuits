#pragma once

#include <string>
#include <thread>

/*
This class is made for basic work with abc.
Using it, it is possible to read verlog file,
read aiger file, make strash and to write file into
verilog or aiger
*/

// so, this struct is used for sending info about multi-line commands
// for abc
struct StandartCommandInfo {
    // int filenameLen;
    // int commandLen;
    // int spacesAndNewlines;
    // int flagsLen;
    int sumLen;
    std::string info;
};

class AbcUtils {
    public:
        static std::thread verilogToAiger(
            std::string i_inpuFileName, 
            std::string i_outpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread verilogToAiger(
            std::string i_inpuFileName, 
            std::string i_outpuFileName, 
            std::string i_directory,
            void (*on_finish) (bool) = NULL
        );

        static std::thread aigerToVerilog(
            std::string i_inpuFileName, 
            std::string i_outpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread aigerToVerilog(
            std::string i_inpuFileName, 
            std::string i_outpuFileName, 
            std::string i_directory,
            void (*on_finish) (bool) = NULL
        );

        static std::thread balanceVerilog(
            std::string i_inpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread balanceVerilog(
            std::string i_inpuFileName, 
            std::string i_directoryv,
            void (*on_finish) (bool) = NULL
        );

        static std::thread balanceAiger(
            std::string i_inpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread balanceAiger(
            std::string i_inpuFileName, 
            std::string i_directory,
            void (*on_finish) (bool) = NULL
        );

        static std::thread strashVerilog(
            std::string i_inpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread strashVerilog(
            std::string i_inpuFileName, 
            std::string i_directory,
            void (*on_finish) (bool) = NULL
        );

        static std::thread strashAiger(
            std::string i_inpuFileName,
            void (*on_finish) (bool) = NULL
        );
        static std::thread strashAiger(
            std::string i_inpuFileName, 
            std::string i_directory,
            void (*on_finish) (bool) = NULL
        );

        // parser, which gets info about elements in command
        static std::vector<StandartCommandInfo> parseCommand(
            std::string command,
            bool parseAll = false
        );
    private:
        // len of "abc 0*> "
        static const int abc_len = 8;
        // "abc *" itself
        static const std::string abc_word;

        static void standartExecutor(
            std::string command,
            std::vector<StandartCommandInfo> info, 
            void (*on_finish) (bool) = NULL
        );
};

