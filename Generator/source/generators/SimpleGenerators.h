#pragma once

#include "../graph/OrientedGraph.h"
#include "./TruthTable.h"

class SimpleGenerators
{
public:
  SimpleGenerators();
  std::vector<std::string> cnfFromTruthTable(const TruthTable& i_table, bool i_tp = true);
  OrientedGraph generatorRandLevel(int i_maxLevel, int i_maxElements, int i_inputs, int i_outputs);
  OrientedGraph generatorNumOperation(
    int i_input,
    int i_output,
    std::map<std::string, int> i_logicOper,
    bool i_leaveEmptyOut = true
  );
  OrientedGraph generatorSummator(int bits, bool overflowIn, bool overflowOut, bool minus, bool act = false);
  OrientedGraph generatorСomparison(int bits, bool compare0, bool compare1, bool compare2, bool act = false);
  OrientedGraph generatorDecoder(int bits);
private:
  Settings* d_settings = Settings::getInstance("GraphVertex");
  std::map<std::string, int> delNull(std::map<std::string, int> i_copyLogicOper);
  std::string randomGenerator(const std::map<std::string, int>& i_map);
;};
