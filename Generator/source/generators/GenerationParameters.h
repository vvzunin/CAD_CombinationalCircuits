#pragma once

#include <map>
#include <vector>
#include <string>

#include "./Genetic/GeneticParameters.h"


/// class CNNFromTruthTableParameters
/// @param d_generateLimitations It may refer to conditions or restrictions that can be applied to combinational logic, for example, imposing restrictions on input or output signals, or restrictions on the use of certain logical operations
/// @param d_CNFF A Boolean value indicating whether the form of the normal conjunctive normal form (CNF) should be used to generate a combinational scheme.
/// @param d_CNFT A Boolean value indicating whether to use the normal disjunctive normal form (DNF) to generate a combinational scheme
///  */

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

/// class GeneratorRandLevelParameters
/// @param d_maxLevel It can mean the maximum number of levels in the graph or structure that the level generator generates.
/// @param d_maxElements It can mean the maximum number of logical elements or notes that can be created at each level of the graph structure
/// */


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

/// class GeneratorNumOperationParameters
/// @param d_logicOper This is used to store information about logical operations that can be used to generate combinational circuits. Map <name_log_Op, key(value)>
/// @param d_leaveEmptyOut A Boolean value indicating whether to leave an empty output node (for example, if it is not associated with any logical operation)
/// */

class GeneratorNumOperationParameters
{
public:
    int getLogicOper(const std::string &i_op) const
    {
        if (d_logicOper.find(i_op) != d_logicOper.end())
            return d_logicOper.at(i_op);
        return -1;
    }

    std::map<std::string, int> getLogicOpers() const
    {
        return d_logicOper;
    }
    void setLogicOper(const std::pair<std::string, int> &i_p)
    {
        d_logicOper[i_p.first] = i_p.second;
    }

    void setLogicOper(const std::map<std::string, int> &i_p)
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

/// GeneratorSummatorParameters
/// @param OverFlowIn A Boolean value indicating whether overflow of input values is allowed. It relate to the behavior of the adder when the maximum value of the summed inputs is exceeded*
/// @param OverFlowOut A Boolean value indicating whether overflow of the output value is allowed. The adder can generate an additional bit to handle overflow. Default - false
/// @param minus In the context of combinational circuits, this may mean that the adder can work not only with the addition operation, but also with subtractions*
/// */

class GeneratorSummatorParameters
{
public:
    bool OverFlowIn = false;
    bool OverFlowOut = false;
    bool minus = false;
};

/// class GeneratorComparisonParameters
/// @param compare0 A Boolean value indicating whether a comparison with zero is enabled. In the context of combinational circuits, this may mean that the circuit must perform a comparison with a zero value.
/// @param compare1 A Boolean value indicating whether a comparison with a unit is enabled. In the context of combinational circuits, this may mean that the circuit must perform a comparison with a single value
/// @param compare2 A Boolean value indicating whether a comparison with two is enabled. In the context of combinational circuits, this may mean that the circuit must perform a comparison with a double value
/// 
/// */

class GeneratorComparisonParameters
{
public:
    bool compare0 = false;
    bool compare1 = false;
    bool compare2 = false;
};

/// class GeneratorEncoderParameters
/// The existence of this class is unknown ???
/// 
/// */

class GeneratorEncoderParameters
{
};

/// class GenerationParameters
/// @param d_name Generation name
/// @param d_requestId request ID
/// @param d_libraryName library name***
/// @param d_inputs number of inputs
/// @param d_outputs number of outputs
/// @param d_iteration ???
/// @param d_calculateStatsAbc A Boolean value indicating whether ABC statistics should be calculated
/// @param d_makeOptimizedFiles A Boolean value indicating whether optimized files should be created
/// @param d_makeFirrtl A Boolean value indicating whether FIRRTL files should be created
/// @param d_makeBench A Boolean value indicating whether to create Bench files
/// @param d_gatesInputsInfo A mapping where the key is a string representing information about the inputs, and the value is a vector of integers representing information about the inputs for the gates***
/// 
/// 
/// */

class GenerationParameters
{
public:
    GenerationParameters(
        const std::string &i_name,
        const std::string &i_requestId,
        int i_inputs,
        int i_outputs,
        int i_iteration,
        std::string i_libraryName = "",
        bool i_calculateStatsAbc = false,
        bool i_makeOptimizedFiles = false,
        bool i_makeFirrtl = false,
        bool i_makeBench = false) : d_name(i_name),
                                    d_requestId(i_requestId),
                                    d_inputs(i_inputs),
                                    d_outputs(i_outputs),
                                    d_iteration(i_iteration),
                                    d_libraryName(i_libraryName),
                                    d_calculateStatsAbc(i_calculateStatsAbc),
                                    d_makeOptimizedFiles(i_makeOptimizedFiles),
                                    d_makeFirrtl(i_makeFirrtl),
                                    d_makeBench(i_makeBench),
                                    d_geneticParameters(GeneticParameters(d_inputs, d_outputs)){};

    std::string getName() const
    {
        return d_name;
    }

    void setName(const std::string &i_name)
    {
        d_name = i_name;
    }

    std::string getRequestId() const
    {
        return d_requestId;
    }

    std::string getLibraryName() const
    {
        return d_libraryName;
    }

    void setLibraryName(std::string i_libraryName)
    {
        d_libraryName = i_libraryName;
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

    bool getCalculateStatsAbc() const
    {
        return d_calculateStatsAbc;
    }

    void setCalculateStatsAbc(bool i_calculateStatsAbc)
    {
        d_calculateStatsAbc = i_calculateStatsAbc;
    }

    bool getMakeOptimizedFiles() const
    {
        return d_makeOptimizedFiles;
    }

    void setMakeOptimizedFiles(bool i_makeOuptimizedFiles)
    {
        d_makeOptimizedFiles = i_makeOuptimizedFiles;
    }

    bool getMakeFirrtl() const
    {
        return d_makeFirrtl;
    }

    void setMakeFirrtl(bool i_makeFirrtl)
    {
        d_makeFirrtl = i_makeFirrtl;
    }

    bool getMakeBench() const
    {
        return d_makeBench;
    }

    void setMakeBench(bool i_makeBench)
    {
        d_makeBench = i_makeBench;
    }

    std::map<std::string, std::vector<int>> getGatesInputsInfo() const
    {
        return d_gatesInputsInfo;
    }

    void setGatesInputInfo(const std::map<std::string, std::vector<int>> &i_gatesInputsInfo)
    {
        d_gatesInputsInfo = i_gatesInputsInfo;
    }

    CNNFromTruthTableParameters getCNF() const { return d_cnfFromTruthTableParameters; }
    GeneratorRandLevelParameters getRandLevel() const { return d_generatorRandLevelParameters; }
    GeneratorNumOperationParameters getNumOperations() const { return d_generatorNumOperationParameters; }
    GeneratorSummatorParameters getSummator() const { return d_generatorSummatorParameters; }
    GeneratorComparisonParameters getComparison() const { return d_generatorComparisonParameters; }
    GeneratorEncoderParameters getEncoder() const { return d_generatorEncoderParameters; }
    GeneticParameters getGenetic() const { return d_geneticParameters; }
    void setCNFF(bool i_CNFF) { d_cnfFromTruthTableParameters.setCNFF(i_CNFF); }
    void setCNFT(bool i_CNFT) { d_cnfFromTruthTableParameters.setCNFT(i_CNFT); }
    void setLimit(bool i_limit) { d_cnfFromTruthTableParameters.setLimit(i_limit); }
    void setRandLevelParameters(int i_maxLevel,
                                int i_maxElements)
    {
        d_generatorRandLevelParameters.setMaxLevel(i_maxLevel);
        d_generatorRandLevelParameters.setMaxElements(i_maxElements);
    }
    void setNumOperationParameters(const std::map<std::string, int> &i_m, bool i_LeaveEmptyOut)
    {
        d_generatorNumOperationParameters.setLogicOper(i_m);
        d_generatorNumOperationParameters.setLeaveEmptyOut(i_LeaveEmptyOut);
    }
    void setNumOfCycles(int i_numOfCycles) { d_geneticParameters.setNumOfCycles(i_numOfCycles); }
    void setPopulationSize(int i_populationSize) { d_geneticParameters.setPopulationSize(i_populationSize); }
    void setRecombinationParameters(ParentsTypes i_parentsTypes, int i_tournamentNumber, RecombinationTypes i_recombinationType, int i_refPoints, double maskProbability, int i_recombinationNumber);
    void setMutationParameters(MutationTypes i_mutationTipe, double i_probabilityGen, int i_exchangeType, double i_probabilityTruthTable);
    void setSelectionParameters(SelectionTypes i_selectionType, int i_numOfSurvivors);
    void setKeyEndProcessIndex(double i_keyEndProcessIndex) { d_geneticParameters.setKeyEndProcessIndex(i_keyEndProcessIndex); }

private:
    std::string d_name = "";
    std::string d_requestId;
    std::string d_libraryName;
    int d_inputs = 0;
    int d_outputs = 0;
    int d_iteration = 0;
    bool d_calculateStatsAbc;
    bool d_makeOptimizedFiles;
    bool d_makeFirrtl;
    bool d_makeBench;
    std::map<std::string, std::vector<int>> d_gatesInputsInfo;

    CNNFromTruthTableParameters d_cnfFromTruthTableParameters;
    GeneratorRandLevelParameters d_generatorRandLevelParameters;
    GeneratorNumOperationParameters d_generatorNumOperationParameters;
    GeneratorSummatorParameters d_generatorSummatorParameters;
    GeneratorComparisonParameters d_generatorComparisonParameters;
    GeneratorEncoderParameters d_generatorEncoderParameters;
    GeneticParameters d_geneticParameters = GeneticParameters(2, 3);
};
