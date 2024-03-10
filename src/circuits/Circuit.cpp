#include <cmath>
#include <mutex>
#include <cstdio>
#include <thread>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <filesystem>

#include "Circuit.h"
#include <additional/filesTools/FilesTools.h>
#include <additional/AuxiliaryMethods/AuxiliaryMethods.h>

Circuit::Circuit(OrientedGraph *i_graph, const std::vector<std::string> &i_logExpressions)
{
    d_graph = i_graph;
    std::clog << "Graph moved, update begins" << std::endl;
    d_graph->updateLevels();
    d_logExpressions = i_logExpressions;
}

void Circuit::computeHash()
{
    std::stringstream stream;
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_numInputs << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_numOutputs << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_maxLevel << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_numEdges << '\n';
    stream << std::setfill(' ') << std::setw(10) << std::fixed << std::setprecision(10) << d_circuitParameters.d_reliability << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_size << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_area << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_gates << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_sensitiveArea << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_sensitiveAreaPercent << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_sensitivityFactor << '\n';
    stream << std::setfill(' ') << std::setw(10) << d_circuitParameters.d_reliabilityPercent << '\n';

    for (auto [key, value] : d_circuitParameters.d_numElementsOfEachType)
        stream << "\t\t\"" << key << "\": " << value << '\n';

    for (auto [key, value] : d_circuitParameters.d_numEdgesOfEachType)
        stream << "\t\t\"" << key.first << "-" << key.second << "\": " << value << '\n';

    {
        // TODO: write logic of get SHA256
    }
}

void Circuit::updateCircuitsParameters(bool i_getAbcStats, std::string i_libraryName)
{
    if (d_graph->size() == 0)
        return;

    d_graph->updateLevels();
    std::clog << "Update ended, norm. calc started" << std::endl;

    d_circuitParameters.d_name = d_circuitName;

    std::vector<std::string> inputs = d_graph->getVerticesByType("input");
    std::vector<std::string> outputs = d_graph->getVerticesByType("output");

    d_circuitParameters.d_numInputs = 0; // inputs.size();
    // do we really need it?
    for (int i = 0; i < inputs.size(); ++i)
        if (inputs[i].find("'b") == std::string::npos)
            d_circuitParameters.d_numInputs++;

    d_circuitParameters.d_numOutputs = outputs.size();

    d_circuitParameters.d_maxLevel = d_graph->getMaxLevel();

    d_circuitParameters.d_numEdges = 0;
    for (const auto &row : d_graph->getAdjacencyMatrixReference())
        for (auto el : row)
            if (el)
                d_circuitParameters.d_numEdges++;

    if (inputs.size() <= 15 &&
        d_graph->d_vertices.size() - (d_graph->d_inputs.size() + d_graph->d_consts.size() + d_graph->d_outputs.size()) > 0)
        d_circuitParameters.d_reliability = 1 - calculateReliability(inputs.size()) / pow(2.0, float(inputs.size()));
    else
        d_circuitParameters.d_reliability = 1;

    d_circuitParameters.d_size = dict["size"];
    d_circuitParameters.d_area = dict["area"];
    d_circuitParameters.d_longestPath = dict["longestPath"];

    d_circuitParameters.d_gates = dict["gates"];
    d_circuitParameters.d_sensitivityFactor = dict["sensitive_factor"];
    d_circuitParameters.d_reliabilityPercent = dict["sensitivity_factor_percent"];
    d_circuitParameters.d_sensitiveArea = dict["sencitive_area"];
    d_circuitParameters.d_sensitiveAreaPercent = dict["sensitive_area_persent"];

    d_circuitParameters.d_numElementsOfEachType.clear();
    const std::vector<GraphVertex> &gv = d_graph->getVerticesReference();

    for (const auto &[key, value] : d_settings->getLogicOperations())
        d_circuitParameters.d_numElementsOfEachType[key] = 0;

    for (const auto &v : gv)
        for (const auto &[key, value] : d_settings->getLogicOperations())
            if (v.getOperation() == key)
                d_circuitParameters.d_numElementsOfEachType[v.getOperation()]++;

    d_circuitParameters.d_numEdgesOfEachType.clear();
    for (const auto &[key1, value1] : d_settings->getLogicOperations())
        for (const auto &[key2, vluae2] : d_settings->getLogicOperations())
            if (key1 != "output" && key2 != "input")
                d_circuitParameters.d_numEdgesOfEachType[std::make_pair(key1, key2)] = 0;

    for (int i = 0; i < gv.size(); ++i)
        for (int j = 0; j < gv.size(); ++j)
            if (d_graph->getAdjacencyMatrix(i, j))
                d_circuitParameters.d_numEdgesOfEachType[std::make_pair(gv[i].getOperation(), gv[j].getOperation())]++;

    if (i_getAbcStats)
    {
        std::clog << d_circuitName << " calc started" << std::endl;

        // Would be called after abc work
        d_circuitParameters.d_abcStats = AbcUtils::getStats(
            d_circuitName + ".v",
            i_libraryName,
            d_path,
            d_settings->getLibraryPath());

        std::clog << d_circuitName << " calc ended" << std::endl;
    }

    computeHash();
}

bool Circuit::graphToVerilog(const std::string &i_path, bool i_pathExists)
{
    if (d_graph->empty())
        return false;

    d_graph->numberVerticesCorrectly();

    /* if (!i_pathExists) // TODO: work with directory
           if (!FilesTools::isDirectoryExists(std::filesystem::current_path().string() + i_path))
               std::filesystem::create_directory(i_path);
       */

    static std::string filename;
    static std::string s;

    int previousSizeOfFileName = filename.size();

    filename = d_path + "/" + d_circuitName + ".v";

    std::vector<std::string> inputs = d_graph->getVerticesByType("input");
    std::vector<std::string> outputs = d_graph->getVerticesByType("output");
    std::vector<std::string> consts = d_graph->getVerticesByType("const");

    int pos = (s.find_last_of('/')) + 1;
    int pos2 = (filename.find_last_of('/')) + 1;

    if (previousSizeOfFileName == 0)
        s = std::filesystem::current_path().string() + "/" + filename; // static variable will be created one time and then will be used throught running of the program
    else
        s.replace(pos, previousSizeOfFileName, filename, pos2, previousSizeOfFileName);

    bool f = std::filesystem::exists(s);

    std::ofstream w(filename);
    for (const auto &expr : d_logExpressions)
        w << "//" << expr << '\n';

    w << "module " << d_circuitName << "(";

    std::string in = "";
    std::string out = "";

    const std::string inputModule = "\tinput";
    const std::string outputModule = "\toutput";

    for (const auto &in_i : inputs)
        if (in_i.find("x") != std::string::npos)
            in += " " + in_i + ",";

    if (in.length())
    {
        w << in;
        in[in.length() - 1] = ';';
    }

    bool first = true;
    for (const auto &output : outputs)
    {
        if (first)
        {
            out += " ";
            first = false;
        }
        else
        {
            out += ", ";
        }
        out += output;
    }
    w << out << " );\n\n";

    if (in.length())
        w << inputModule << in << '\n';
    w << outputModule << out << ";\n";

    if (d_graph->size() - inputs.size() - outputs.size() - consts.size() > 0)
    {
        w << "\n\twire";
        bool first = true;
        for (const auto &vert : d_graph->getVertices())
        {
            if (vert.getOperation() != "input" && vert.getOperation() != "output" && vert.getOperation() != "const")
            {
                if (first)
                {
                    w << " " << vert.getWireName();
                    first = false;
                }
                else
                {
                    w << ", " << vert.getWireName();
                }
            }
        }
        w << ";\n"
          << std::endl;
    }

    for (int j = 0; j < d_graph->size(); ++j)
    {
        if (d_graph->getVertice(j).getOperation() != "input")
        {
            std::vector<int> inps;
            for (int i = 0; i < d_graph->size(); ++i)
                if (d_graph->getAdjacencyMatrix(i, j))
                    inps.push_back(i);

            if (d_graph->getVertice(j).getOperation() != "output" && d_graph->getVertice(j).getOperation() != "const")
            {
                w << "\t" << d_graph->getVertice(j).getOperation() << " ( " << d_graph->getVertice(j).getWireName();
                // TODO: on prev line add instance name
                for (auto k : inps)
                    w << ", " << d_graph->getVertice(k).getWireName();
                w << ");" << std::endl;
            }
            else if (d_graph->getVertice(j).getOperation() == "output" && inps.size() > 0)
            {
                w << "\tassign " << d_graph->getVertice(j).getWireName() << " = " << d_graph->getVertice(inps[0]).getWireName() << ";" << std::endl;
            }
        }
    }

    w << "endmodule" << std::endl;
    w.close();

    return true;
}

bool Circuit::saveParameters(bool i_getAbcStats, bool i_generateAig, bool i_pathExists) const
{
    if (true)
    { /*
         if (!FilesTools::isDirectoryExists(std::filesystem::current_path().string() + d_path)) // TODO: make function isDirectory exists
         {
             //std::filesystem::create_directory(d_path);
         }
     */
    }

    std::string filename = d_path + "/" + d_circuitName + ".json";

    // if (std::filesystem::exists(filename))
    //     std::remove(filename.c_str());

    std::ofstream outputFile(filename);

    if (!outputFile)
        return false;

    outputFile << "{" << std::endl;

    outputFile << "\t\"name\": \"" << d_circuitParameters.d_name << "\"," << std::endl;
    outputFile << "\t\"numInputs\": \"" << d_circuitParameters.d_numInputs << "\"," << std::endl;
    outputFile << "\t\"numOutputs\": \"" << d_circuitParameters.d_numOutputs << "\"," << std::endl;
    outputFile << "\t\"maxLevel\": \"" << d_circuitParameters.d_maxLevel << "\"," << std::endl;
    outputFile << "\t\"numEdges\": \"" << d_circuitParameters.d_numEdges << "\"," << std::endl;
    // w << "\t\"\": \"" << d_circuitParameters. << "\"," << std::endl; TODO: what is this mean?
    outputFile << "\t\"reliability\": \"" << std::fixed << std::setprecision(20) << d_circuitParameters.d_reliability << "\"," << std::endl;
    outputFile << "\t\"size\": \"" << d_circuitParameters.d_size << "\"," << std::endl;
    outputFile << "\t\"area\": \"" << d_circuitParameters.d_area << "\"," << std::endl;
    outputFile << "\t\"longest_path\": \"" << d_circuitParameters.d_longestPath << "\"," << std::endl;
    outputFile << "\t\"gates\": \"" << d_circuitParameters.d_gates << "\"," << std::endl;
    outputFile << "\t\"sensitivity_factor\": \"" << d_circuitParameters.d_sensitivityFactor << "\"," << std::endl;
    outputFile << "\t\"sinsitivity_factor_percent\": \"" << d_circuitParameters.d_reliabilityPercent << "\"," << std::endl;
    outputFile << "\t\"sensitive_area\": \"" << d_circuitParameters.d_sensitiveArea << "\"," << std::endl;
    outputFile << "\t\"sensitive_area_percent\": \"" << d_circuitParameters.d_sensitiveAreaPercent << "\"," << std::endl;
    // w << "\t\"\": \"" << d_circuitParameters << "\"," << std::endl; // TODO: what is this mean?
    outputFile << "\t\"hash_code\": \"" << d_circuitParameters.d_hashCode << "\"," << std::endl;

    outputFile << "\t\"numElementsOfEachType\": {" << std::endl;

    bool first = true;
    for (const auto &[key, value] : d_circuitParameters.d_numElementsOfEachType)
    {
        if (value != 0)
        {
            if (first)
            {
                first = false;
                outputFile << "\t\t\"" << key << "\": " << value;
            }
            else
            {
                outputFile << "," << std::endl
                           << "\t\t\"" << key << "\": " << value;
            }
        }
    }
    outputFile << std::endl;

    outputFile << "\t}," << std::endl;

    outputFile << "\t\"numEdgesOfEachType\": {" << std::endl;
    first = true;
    for (const auto &[key, value] : d_circuitParameters.d_numEdgesOfEachType)
    {
        if (value != 0)
        {
            if (first)
            {
                first = false;
                outputFile << "\t\t\"" << key.first << "-" << key.second << "\": " << value;
            }
            else
            {
                outputFile << "," << std::endl
                           << "\t\t\"" << key.first << "-" << key.second << "\": " << value;
            }
        }
    }
    outputFile << std::endl;

    outputFile << "\t}";

    if (i_getAbcStats)
    {
        outputFile << "," << std::endl;

        CommandWorkResult statsRes = d_circuitParameters.d_abcStats;
        // we need it because we cannot change data in original d_abcStats, in it's map
        if (statsRes.correct)
        {
            statsRes.commandsOutput["inputsNumberCorrect"] =
                stoi(statsRes.commandsOutput["inputs"]) == d_circuitParameters.d_numInputs ? "true" : "false";

            statsRes.commandsOutput["outputsNumberCorrect"] =
                stoi(statsRes.commandsOutput["outputs"]) == d_circuitParameters.d_numOutputs ? "true" : "false";
        }

        // if we are going to add sth into this file, i_generateAig flag is true
        // and we will end the json
        saveAdditionalStats(statsRes, "", !i_generateAig);
    }

    // if we did not ended json yet and we need it, ending
    if (!i_generateAig && !i_getAbcStats)
        outputFile << std::endl
                   << "}";

    return true;
}

bool Circuit::checkExistingHash() // TODO: is it really need return true when hash wrong?
{
    std::string path = FilesTools::getParentDirOf(d_path);
    if (path == "" || !std::filesystem::exists(path))
        return false;

    std::ifstream r(path);

    std::string hash = "";

    r >> hash;

    while (r >> hash)
        if (hash != d_circuitParameters.d_hashCode)
            return false; // TODO: costul

    return false;
}

bool Circuit::generate(bool i_makeFirrtl, bool i_makeBench, bool i_getAbcStats, std::string i_libraryName, bool i_generateAig, bool i_pathExists)
{
    // creating all files in sub directories
    std::string d_path_temp = d_path + d_circuitName;
    d_path += d_circuitName + "/";

    std::filesystem::create_directories(d_path);

    if (i_libraryName.find(".lib") == std::string::npos)
        i_libraryName += ".lib";

    // if (!i_pathExists)
    // d_path += d_circuitName;
    std::clog << "Writing verilog for " << d_circuitName << std::endl;
    if (!graphToVerilog(d_path, i_pathExists))
        return false;

    std::clog << "Writing verilog ended " << d_circuitName << std::endl;

    if (i_makeFirrtl)
    {
        YosysUtils::writeFirrtl(
            d_circuitName + ".v",
            d_circuitName + ".fir",
            d_path);
    }
    if (i_makeBench)
    {
        AbcUtils::verilogToBench(
            d_circuitName + ".v",
            d_circuitName + ".bench",
            d_path);
    }

    if (i_generateAig)
    {
        std::vector<CommandWorkResult> res;
        std::mutex resWrite;

        // this lambda is used for writing reduced.json in thread
        auto saveOptimizationParameters = [path = this->d_path,
                                           circuitName = this->d_circuitName,
                                           libraryName = i_libraryName,
                                           libraryPath = d_settings->getLibraryPath(),
                                           &res,
                                           &resWrite](CommandWorkResult (*func)(std::string, std::string, std::string, std::string))
        {
            CommandWorkResult r = func(
                circuitName + ".v",
                libraryName,
                path,
                libraryPath);

            resWrite.lock();
            res.push_back(r);
            resWrite.unlock();
        };

        CommandWorkResult (*func)(std::string, std::string, std::string, std::string) = AbcUtils::optimizeWithLib;

        std::thread optimize1(
            saveOptimizationParameters,
            func);

        func = AbcUtils::resyn2;
        std::thread optimize2(
            saveOptimizationParameters,
            func);

        std::clog << "Update started" << std::endl;
        updateCircuitsParameters(i_getAbcStats, i_libraryName);

        std::clog << "Write started" << std::endl;
        if (!saveParameters(i_getAbcStats, i_generateAig))
        {
            std::cerr << "Json file was not written!" << std::endl;
            return false;
        }

        optimize1.join();
        optimize2.join();

        CommandWorkResult last_res = res.back();
        res.pop_back();

        for (auto subres : res)
        {
            std::string optType = subres.commandsOutput["optimization_type"];
            subres.commandsOutput.erase("optimization_type");

            saveAdditionalStats(subres, optType, false);
        }

        // last iteration has no false key, so for optimization
        // (remove check is current element the last one)
        // we move last cycle to here
        std::string optType = last_res.commandsOutput["optimization_type"];
        last_res.commandsOutput.erase("optimization_type");

        saveAdditionalStats(last_res, optType);
    }
    else
    {
        updateCircuitsParameters(i_getAbcStats, i_libraryName);

        if (!saveParameters(i_getAbcStats))
            return false;
    }
    // TODO: costul
    // if (checkExistingHash() || d_circuitParameters.d_reliability == 0 || d_circuitParameters.d_gates == 0)
    //{
    // if (!i_pathExists)
    //     std::filesystem::remove_all(d_path);
    //}
    // else
    //{
    //    std::string path = FilesTools::getParentDirOf(d_path) + "/hashCodes.txt";
    //    std::ofstream w(path, std::ios_base::out | std::ios_base::app);
    //    w << d_circuitParameters.d_hashCode;
    //}

    return true;
}

void Circuit::setTable(const TruthTable &i_tt)
{
    d_tTable = i_tt;
}

void Circuit::setPath(const std::string &i_path)
{
    d_path = i_path;
}

void Circuit::setCircuitName(const std::string &i_circName)
{
    d_circuitName = i_circName;
    d_graph.setName(i_circName);
}
GraphVertexBase* Circuit::getIndexOfWireName(const std::string &i_wireName)
{
    return d_graph->getVerticesByName(i_wireName);
}
