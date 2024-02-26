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
#include <reliability/Reliability.h>
#include <optimization_utils/AbcUtils.h>
#include <optimization_utils/YosysUtils.h>
#include <filesTools/FilesTools.h>
#include <AuxiliaryMethods/AuxiliaryMethods.h>

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

int Circuit::calculateReliability(int inputs_size)
{
    // std::cout << R.calcReliabilityBase() << std::endl;
    // std::cout << R.valveRating() << std::endl;

    // // Algorithm for evaluating circuit reliability
    // std::vector<std::vector<bool>> vec(1 + (int) pow(2.0, float(inputs_size)));
    // std::vector<bool> first(inputs_size, false);

    // vec.push_back(first);

    // for (int i = 0; i < pow(2.0, float(inputs_size)); i++)
    // {
    //     std::vector<bool> next = vec.back();

    //     for (int m = inputs_size - 1; m >= 0; m--)
    //     {
    //         if (next[m])
    //         {
    //             next[m] = false;
    //         }
    //         else
    //         {
    //             next[m] = true;
    //             break;
    //         }
    //     }
    //     vec.push_back(next);
    // }
    /*
    for (int i = 0; i < pow(2.0, float(inputs_size)); i++)
    {
            for (int j = 0; j < inputs_size; j++)
            {
                    if (vec[i][j] == false) std::cout << 0 << "    ";
                    else std::cout << 1 << "    ";
            }
            std::cout << "\n";

    }*/

    int numberOfIncoincidences = 0;
    int pos = -1;
    std::vector<int> vec_index = d_graph->getVertices("input");

    for (u_int64_t i = 0; i < pow(2.0, float(inputs_size)); ++i)
    {
        // std::vector<bool> tmp = vec.back();
        // std::vector<bool> tmp_wrong = vec.back();
        if (pos != -1)
            d_graph->d_vertices[pos].wrongVertex = false;

        // vec.pop_back()
        u_int64_t data = i;
        for (int j : vec_index)
        {
            d_graph->d_vertices[j].setValue(data % 2);
            data >>= 1;
        }

        //    std::vector<bool> res_1 = {};
        for (int m = 1; m < d_graph->getMaxLevel(); m++)
        {
            std::vector<int> vec_2 = d_graph->getVerticesByLevel_2(m);
            for (int n : vec_2)
            {
                std::vector<int> tmp_2 = d_graph->d_listOfEdgesToFrom[n];
                std::vector<bool> vec_3;
                vec_3.reserve(tmp_2.size());

                for (int j : tmp_2)
                {
                    vec_3.push_back(d_graph->d_vertices[j].getValue());
                }

                d_graph->d_vertices[n].setValue(d_graph->calc(vec_3, (d_graph->d_vertices[n]).getOperation()));
                //        res_1.push_back(d_graph->d_vertices[vec_2[n]].getValue());
            }
        }

        std::vector<int> vec_outputs_indices_without_error = d_graph->getVertices("output");
        std::vector<bool> result_without_error;
        result_without_error.reserve(vec_outputs_indices_without_error.size());

        for (int j : vec_outputs_indices_without_error)
        {
            bool cur_value = d_graph->d_vertices[(d_graph->d_listOfEdgesToFrom[j])[0]].getValue();

            d_graph->d_vertices[j].setValue(cur_value);
            result_without_error.push_back(cur_value);
        }

        pos = -1;

        /* Working version
for (int j = 0; j < d_graph->d_vertices.size(); j++)
{
        if (d_graph->d_vertices[j].getOperation() != "input" && d_graph->d_vertices[j].getOperation() != "const" && d_graph->d_vertices[j].getOperation() != "output")
        {
                d_graph->d_vertices[j].wrongVertex = true;
                pos = j;
                break;
        }
}*/

        std::vector<int> indecies__;
        indecies__.reserve(
            d_graph->d_vertices.size() - (d_graph->d_inputs.size() + d_graph->d_outputs.size() + d_graph->d_consts.size()));

        for (auto j = 0; j < d_graph->d_vertices.size(); j++)
        {
            if (d_graph->d_vertices[j].getOperation() != "input" && d_graph->d_vertices[j].getOperation() != "output" && d_graph->d_vertices[j].getOperation() != "const")
            {
                indecies__.push_back(j);
            }
        }

        // Choose random wrong vertex.
        int t = AuxMethods::getRandInt(0, indecies__.size());
        d_graph->d_vertices[indecies__[t]].wrongVertex = true;
        pos = indecies__[t];

        data = i;
        for (int j : vec_index)
        {
            d_graph->d_vertices[j].setValue(data % 2);
            data >>= 1;
        }

        //    std::vector<bool> res_1 = {};
        for (int m = 1; m < d_graph->getMaxLevel(); m++)
        {
            std::vector<int> vec_2 = d_graph->getVerticesByLevel_2(m);
            for (int n : vec_2)
            {
                std::vector<int> tmp_2 = d_graph->d_listOfEdgesToFrom[n];
                std::vector<bool> vec_3;
                vec_3.reserve(tmp_2.size());

                for (int j : tmp_2)
                {
                    vec_3.push_back(d_graph->d_vertices[j].getValue());
                }
                d_graph->d_vertices[n].setValue(d_graph->calc(vec_3, (d_graph->d_vertices[n]).getOperation()));
                //        res_1.push_back(d_graph->d_vertices[vec_2[n]].getValue());
            }
        }

        std::vector<int> vec_outputs_indices_with_error = d_graph->getVertices("output");
        std::vector<bool> result_with_error;
        result_with_error.reserve(vec_outputs_indices_with_error.size());

        for (int j : vec_outputs_indices_with_error)
        {
            bool cur_value = d_graph->d_vertices[(d_graph->d_listOfEdgesToFrom[j])[0]].getValue();

            d_graph->d_vertices[j].setValue(cur_value);
            result_with_error.push_back(cur_value);
        }

        /*
        for (int j = 0; j < result_with_error.size(); j++)
        {
                std::cout << "with_error: " << result_with_error[j] << "\twithout_error: " << result_without_error[j] << "\n";
        }
        std::cout << "next: " << "\n";
        */
        if (result_with_error != result_without_error)
            numberOfIncoincidences++;
        /*
        for (int j = 0; j < vec_outputs_indices.size(); j++)
        {
                std::cout << "iteration number: " << i << d_graph->d_listOfEdgesToFrom[vec_outputs_indices[j]].size() << "\t value: " << d_graph->d_vertices[(d_graph->d_listOfEdgesToFrom[vec_outputs_indices[j]])[0]].getValue() << "\n";
        }
        std::cout << "\n\n";
        */
    }

    return numberOfIncoincidences;
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

    // Reliability R(d_graph, 0.5);
    std::map<std::string, double> dict; // = R.runNadezhda(d_path, d_circuitName); // what? d_path

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

void Circuit::saveAdditionalStats(CommandWorkResult i_res, std::string i_optimizationName, bool i_lastData) const
{
    std::ofstream outJson;

    outJson.open((d_path + "/" + d_circuitName + ".json"), std::ios_base::app);
    if (!outJson)
    {
        std::cerr << "No json file to write" << std::endl;
        return;
    }

    outJson << "\t\"abcStats" << i_optimizationName << "\": {" << std::endl;

    if (i_res.correct)
    {
        bool first = true;
        for (const auto &data : i_res.commandsOutput)
        {
            if (first)
            {
                first = false;
                outJson << "\t\t\"" << data.first << "\": " << data.second;
            }
            else
            {
                outJson << "," << std::endl
                        << "\t\t\"" << data.first << "\": " << data.second;
            }
        }
        outJson << std::endl;
    }
    else
    {
        for (int i = i_res.commandsOutput["error"].find('"');
             i != std::string::npos; i = i_res.commandsOutput["error"].find('"', i + 1))
            i_res.commandsOutput["error"].erase(i, 1);

        for (int i = i_res.commandsOutput["error"].find('\n');
             i != std::string::npos; i = i_res.commandsOutput["error"].find('\n', i + 1))
            i_res.commandsOutput["error"][i] = ';';

        outJson << "\t\t"
                << "\"error\": \"" << i_res.commandsOutput["error"] << "\",\n";
        outJson << "\t\t"
                << "\"fileRead\": \"" << i_res.commandsOutput["fileRead"] << "\"\n";
    }

    outJson << "\t}";
    if (i_lastData)
        outJson << std::endl
                << "}";
    else
        outJson << ',' << std::endl;

    std::clog << d_circuitName << " " << i_optimizationName << " ended\n";
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
}

bool Circuit::addVertex(const std::string i_vertexName, const std::string &i_operation, const std::string &i_wireName)
{
    return d_graph->addVertex(i_vertexName, i_operation, i_wireName);
}

int Circuit::getIndexOfWireName(const std::string &i_wireName)
{
    return d_graph->getIndexOfWireName(i_wireName);
}

GraphVertex Circuit::getVertice(int i) const
{
    return d_graph->getVertice(i);
}

bool Circuit::addEdge(const std::string &i_vertexFrom, const std::string &i_vertexTo, bool i_isExpression)
{
    return d_graph->addEdge(i_vertexFrom, i_vertexTo, i_isExpression);
}

bool Circuit::addDoubleEdge(const std::string &i_vertexFromFirst, const std::string &i_vertexFromSecond, const std::string &i_vertexTo, bool i_isExpression)
{
    return d_graph->addDoubleEdge(i_vertexFromFirst, i_vertexFromSecond,
                                  i_vertexTo, i_isExpression);
}

void Circuit::setVerticeOperation(int i_vertice, const std::string &i_operation)
{
    d_graph->setVerticeOperation(i_vertice, i_operation);
}
