#pragma once

#include <map>
#include <memory>
#include <vector>

#include <baseStructures/graph/OrientedGraph.h>
#include "baseStructures/truthTable/TruthTable.h"

class SimpleGenerators
{
public:
    SimpleGenerators();
    std::vector<std::string> cnfFromTruthTable(const TruthTable &i_table, bool i_tp = true);
    OrientedGraph generatorRandLevel(int i_maxLevel, int i_maxElements, int i_inputs, int i_outputs);
    OrientedGraph generatorNumOperation(
        int i_input,
        int i_output,
        std::map<std::string, int> i_logicOper,
        bool i_leaveEmptyOut = true);
    OrientedGraph generatorSummator(int bits, bool overflowIn, bool overflowOut, bool minus, bool act = false);
    OrientedGraph generatorComparison(int bits, bool compare0, bool compare1, bool compare2, bool act = false);
    OrientedGraph generatorEncoder(int bits);

    void setGatesInputsInfo(const std::map<std::string, std::vector<int>> &i_info) {
        d_gatesInputsInfo = i_info;
    }

    std::map<std::string, std::vector<int>> getGatesInputsInfo() const {
        return d_gatesInputsInfo;
    }
private:
    std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
    std::map<std::string, int> delNull(std::map<std::string, int> i_copyLogicOper);
    std::string randomGenerator(const std::map<std::string, int> &i_map);

    int getRangomAndNumber() const;
    int getRangomOrNumber() const;
    int getRangomNandNumber() const;
    int getRangomNorNumber() const;
    int getRangomXorNumber() const;
    int getRangomXnorNumber() const;

    std::map<std::string, std::vector<int>> d_gatesInputsInfo;
};
