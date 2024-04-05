#pragma once

#include <string>
#include <map>

#include "./Genetic/GeneticParameters.h"

class CNNFromTruthTableParameters
{
public:
  bool getCNFF() { return d_CNFF; }
  bool getCNFT() { return d_CNFT; }
  void setCNFF(bool i_CNFF) { d_CNFF = i_CNFF; }
  void setCNFT(bool i_CNFT) { d_CNFT = i_CNFT; }
  void setLimit(bool i_limit) { d_generateLimitations = i_limit; }
private:
  bool d_generateLimitations = false;
  bool d_CNFF = false;
  bool d_CNFT = false;
};

class GeneratorRandLevelParameters
{
public:
  int getMaxLevel() const
  {
    return d_maxLevel;
  }

  void setMaxLevel(int i_maxLevel)
  {
    d_maxLevel = i_maxLevel;
  }

  int getMaxElements() const
  {
    return d_maxElements;
  }

  void setMaxElements(int i_maxElements)
  {
    d_maxElements = i_maxElements;
  }

private:
  int d_maxLevel = 0;
  int d_maxElements = 0;
};

class GeneratorNumOperationParameters
{
public:
  int getLogicOper(const std::string& i_op) const
  {
    if (d_logicOper.find(i_op) != d_logicOper.end())
      return d_logicOper.at(i_op);
    return -1;
  }

  std::map<std::string, int> getLogicOpers() const
  {
    return d_logicOper;
  }
  void setLogicOper(const std::pair<std::string, int>& i_p)
  {
    d_logicOper[i_p.first] = i_p.second;
  }

  void setLogicOper(const std::map<std::string, int>& i_p)
  {
    d_logicOper = i_p;
  }

  bool getLeaveEmptyOut() const
  {
    return d_leaveEmptyOut;
  }

  void setLeaveEmptyOut(bool i_leo)
  {
    d_leaveEmptyOut = i_leo;
  }

private:
  std::map<std::string, int> d_logicOper;
  bool d_leaveEmptyOut = true;
};

class GeneratorSummatorParameters{
public:
    bool getOverFlowIn() {return d_OverFlowIn;}
    bool getOverFlowOut() {return d_OverFlowOut;}
    bool getMinus() {return d_minus;}
    void setOverFlowIn(bool i_overflowIn) {d_OverFlowIn = i_overflowIn;}
    void setOverFlowOut(bool i_overflowOut) {d_OverFlowOut = i_overflowOut;}
    void setMinus(bool i_minus) {d_minus = i_minus;}
private:
    bool d_OverFlowIn = false;
    bool d_OverFlowOut = false;
    bool d_minus = false;
};

class GeneratorComparisonParameters
{
  public:
    bool getCompare0() {return d_compare0;}
    bool getCompare1() {return d_compare1;}
    bool getCompare2() {return d_compare2;}
    bool setCompare0(bool i_compare0) {d_compare0 = i_compare0;}
    bool setCompare1(bool i_compare1) {d_compare1 = i_compare1;}
    bool setCompare2(bool i_compare2) {d_compare2 = i_compare2;}
  private:
    bool d_compare0 = false;
    bool d_compare1 = false;
    bool d_compare2 = false;
};

class GenerationParameters
{
public:

  GenerationParameters(
    const std::string& i_name,
    const std::string& i_requestId,
    int i_inputs,
    int i_outputs,
    int i_iteration,
    int i_maxLevel,
    int i_maxElements) :
    d_name(i_name),
    d_requestId(i_requestId),
    d_inputs(i_inputs),
    d_outputs(i_outputs),
    d_iteration(i_iteration),
    d_geneticParameters(GeneticParameters(d_inputs, d_outputs))
  {
    d_generatorRandLevelParameters.setMaxLevel(i_maxLevel);
    d_generatorRandLevelParameters.setMaxElements(i_maxElements);
  };

  std::string getName() const
  {
    return d_name;
  }

  void setName(const std::string& i_name)
  {
    d_name = i_name;
  }

  std::string getRequestId() const
  {
    return d_requestId;
  }

  int getInputs() const
  {
    return d_inputs;
  }

  void setInputs(int i_inputs)
  {
    d_inputs = i_inputs;
  }

  int getOutputs() const
  {
    return d_outputs;
  }

  void setOutputs(int i_outputs)
  {
    d_outputs = i_outputs;
  }

  int getIteration() const
  {
    return d_iteration;
  }

  void setIteration(int i_iteration)
  {
    d_iteration = i_iteration;
  }

  CNNFromTruthTableParameters getCNF() const { return d_cnfFromTruthTableParameters; }
  GeneratorRandLevelParameters getRandLevel() const { return d_generatorRandLevelParameters; }
  GeneratorNumOperationParameters getNumOperations() const { return d_generatorNumOperationParameters; }
  GeneratorSummatorParameters getSummator() const {return d_generatorSummatorParameters; }
  GeneratorComparisonParameters getComparison() const {return d_generatorComparisonParameters; }
  GeneticParameters getGenetic() const { return d_geneticParameters; }
  void setCNFF(bool i_CNFF){ d_cnfFromTruthTableParameters.setCNFF(i_CNFF); }
  void setCNFT(bool i_CNFT){ d_cnfFromTruthTableParameters.setCNFT(i_CNFT); }
  void setLimit(bool i_limit){ d_cnfFromTruthTableParameters.setLimit(i_limit); }
  void setNumOperationParameters(const std::map<std::string, int>& i_m, bool i_LeaveEmptyOut)
  {
	d_generatorNumOperationParameters.setLogicOper(i_m);
  	d_generatorNumOperationParameters.setLeaveEmptyOut(i_LeaveEmptyOut);
  }
  void setSummatorParameters(bool i_overflowIn, bool i_overflowOut, bool i_minus)
  {
      d_generatorSummatorParameters.setOverFlowIn(i_overflowIn);
      d_generatorSummatorParameters.setOverFlowOut(i_overflowOut);
      d_generatorSummatorParameters.setMinus(i_minus);
  }
  void setComparisonParameters(bool i_compare0, bool i_compare1, bool i_compare2)
  {
    d_generatorComparisonParameters.setCompare0(i_compare0);
    d_generatorComparisonParameters.setCompare1(i_compare1);
    d_generatorComparisonParameters.setCompare2(i_compare2);
  }
  void setNumOfCycles(int i_numOfCycles){ d_geneticParameters.setNumOfCycles(i_numOfCycles); }
  void setPopulationSize(int i_populationSize) { d_geneticParameters.setPopulationSize(i_populationSize); }
  void setRecombinationParameters(ParentsTypes i_parentsTypes, int i_tournamentNumber, RecombinationTypes i_recombinationType, int i_refPoints, double maskProbability, int i_recombinationNumber);
  void setMutationParameters(MutationTypes i_mutationTipe, double i_probabilityGen, int i_exchangeType, double i_probabilityTruthTable);
  void setSelectionParameters(SelectionTypes i_selectionType, int i_numOfSurvivors);
  void setKeyEndProcessIndex(double i_keyEndProcessIndex) { d_geneticParameters.setKeyEndProcessIndex(i_keyEndProcessIndex); }
private:
  std::string d_name = "";
  std::string d_requestId;
  int d_inputs = 0;
  int d_outputs = 0;
  int d_iteration = 0;

  CNNFromTruthTableParameters d_cnfFromTruthTableParameters;
  GeneratorRandLevelParameters d_generatorRandLevelParameters;
  GeneratorNumOperationParameters d_generatorNumOperationParameters;
  GeneratorSummatorParameters d_generatorSummatorParameters;
  GeneratorComparisonParameters d_generatorComparisonParameters;
  GeneticParameters d_geneticParameters = GeneticParameters(2, 3);
};
