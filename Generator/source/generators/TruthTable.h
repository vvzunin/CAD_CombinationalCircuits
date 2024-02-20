#pragma once

#include <vector>
#include <memory>

#include <settings/Settings.h>
#include "./Genetic/GeneticParameters.h"
#include "./Genetic/Chronosome.h"

/// class TruthTable
/// @param d_input The number of input variables of the truth table.
/// @param d_output The number of output variables (results) of the truth table
/// @param d_size Size (number of rows) of the truth table
/// @param d_array The vector of Boolean value vectors (std::vector<std::vector<bool>>), which is the truth table itself, where each internal vector represents one row of the truth table.
/// @param d_settings A pointer to an object of the Settings class, which is a singleton and is used to store settings related to TruthTable.
/// 
/// </summary>

class TruthTable : Chronosome<TruthTableParameters>
{
public:
    TruthTable();
    void generateRandom(TruthTableParameters i_gp) override;
    void generateTable(double i_p = 0);
    TruthTable(const Chronosome<TruthTableParameters> &i_chr);
    TruthTable(int i_input, int i_output, const std::vector<std::vector<bool>> &i_array = {});
    TruthTable(const TruthTable &i_tt, std::vector<std::vector<bool>> i_array = {});
    TruthTable(int i_input, int i_output, double i_p = 0.5);
    int getInput() const;
    int getOutput() const;
    int size() const;
    std::vector<std::vector<bool>> getOutTable() const;
    bool getOutTable(int i, int j) const;
    std::vector<std::vector<bool>> convToBinary() const;
    void printTable() const;
    bool operator==(const TruthTable &r) const;

private:
    int d_input;
    int d_output;
    int d_size;
    std::vector<std::vector<bool>> d_array;
    std::shared_ptr<Settings> d_settings = Settings::getInstance("TruthTable");
};
