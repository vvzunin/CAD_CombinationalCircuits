#pragma once

#include <string>
#include <memory>
#include <map>
#include <utility>
#include <vector>


enum generationTypes
{
    none,
    truthTableToGraphWithoutOptimization,
    truthTableToGraphWithOptimization  
};

enum LibrariesTypes
{
    SKY_LIB
};


/// class Settings
/// 
/// This is the detailed one. More details. Private...
/// @param d_name This is a string field for storing the name of the settings 
/// @param d_singleton Singleton ensures that only one instance of the class exists in the application
/// @param d_path The path to the settings file
/// @param d_csvdataset Contains the name of the CSV file for the dataset. By default, "dataset.csv"
/// @param d_fileName The name of the settings file. The default value is "settings.dat"
/// @param d_datasetPath This is a string field containing the path to the directory with the dataset. The default value is "./dataset"
/// @param d_libraryPath This is a string field that contains the path to the library directory. The default value is set to "Generator/libs"
/// @param d_pathToNadezhda This is a string field that contains the path to the directory with Nadezhda data. The default value is set to "./Generator/source/data/Nadezhda". However, this path is not in the project*
/// @param d_nadezhda ???
/// @param d_numThreads The number of threads that are used in the application. The default value is 4
/// @param d_logicOperations This is an associative std::map container that maps strings (keys) into pairs of strings and integers. It is used to store logical operations and their associated parameters, such as the symbolic representation of the operation and its priority
/// @param d_operationsToHierarchy This is an associative std::map container that maps integers to string vectors. It is used to store a hierarchy of logical operations
/// @param d_operationsToName It is used to match symbolic representations of logical operations and their names.
/// @param d_maxInputs The maximum number of inputs. The default value is 50
/// @param d_maxOutputs The maximum number of outputs. The default value is 50
/// 
///




class Settings
{
protected:
    Settings(const std::string &i_path) : d_path(i_path) {}

public:
    Settings(Settings &other) = delete;
    void operator=(const Settings &) = delete;
    static std::shared_ptr<Settings> getInstance(const std::string &i_value);
    void loadSettings();
    std::string getInstanceName() const;
    std::pair<std::string, int> getLogicOperation(const std::string &i_op);
    std::vector<std::string> getLogicOperationsKeys();
    std::string fromOperationsToName(const std::string &i_op) const;
    std::string getDatasetPath() const;
    std::string getGenerationMethodPrefix(const std::string &i_s) const;
    std::string getLibraryPath() const;
    std::string getLibraryNameFromEnum(const LibrariesTypes &library) const;
    int getMaxInputs() const;
    int getMaxOutputs() const;
    std::map<std::string, std::pair<std::string, int>> getLogicOperations() const;
    std::string getPathNadezhda() const;
    std::string getNadezhdaVar(const std::string &key) const;
    std::vector<std::string> fromOperationsToHierarchy(int key) const;

private:
    void SaveSettings();

    std::string d_name;
    static std::shared_ptr<Settings> d_singleton;
    std::string d_path;
    std::string d_csvdataset = "dataset.csv";
    std::string d_fileName = "settings.dat";
    std::string d_datasetPath = "./dataset";
    std::string d_libraryPath = "Generator/libs";
    // Settings d_instance;
    std::string d_pathToNadezhda = "./Generator/source/data/Nadezhda";
    std::map<std::string, std::string> d_nadezhda = {
        {"python", "python3"},
        {"resynthesis", "Nadezhda/Scripts/resynthesis_local_rewriting.pyc"},
        {"reliability", "Nadezhda/Scripts/check_reliability.pyc"},
        {"liberty", "Nadezda/Test/Nangate.lib"}};
    int d_numThreads = 4;
    std::map<std::string, std::pair<std::string, int>> d_logicOperations =
        {
            {"input", {"", 10}},
            {"output", {"=", 0}},
            {"const", {"1'b0", 9}},
            {"and", {"and", 4}},
            {"nand", {"nand", 3}},
            {"or", {"or", 2}},
            {"nor", {"nor", 1}},
            {"not", {"not", 7}},
            {"buf", {"buf", 8}},
            {"xor", {"xor", 6}},
            {"xnor", {"xnor", 5}}

    };
    std::map<int, std::vector<std::string>> d_operationsToHierarchy;
    std::map<std::string, std::string> d_operationsToName;
    int d_maxInputs = 50;
    int d_maxOutputs = 50;
};
