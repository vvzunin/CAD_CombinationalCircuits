#pragma once

#include <vector>

#include "../Settings.h"
#include "./Genetic/GeneticParameters.h"
#include "./Genetic/Chronosome.h"

class TruthTable : Chronosome<TruthTableParameters>
{
public:
  TruthTable();
  void generateRandom(TruthTableParameters i_gp) override;
  void generateTable(double i_p = 0);
  TruthTable(const Chronosome<TruthTableParameters>& i_chr);
  TruthTable(int i_input, int i_output, const std::vector<std::vector<bool>>& i_array = {});
  TruthTable(const TruthTable& i_tt, std::vector<std::vector<bool>> i_array = {});
  TruthTable(int i_input, int i_output, double i_p = 0.5);
  int getInput() const;
  int getOutput() const;
  int size() const;
  std::vector<std::vector<bool>> getOutTable() const;
  bool getOutTable(int i, int j) const;
  std::vector<std::vector<bool>> convToBinary() const;
  void printTable() const;
  bool operator== (const TruthTable& r) const;
  // TruthTable(const TruthTable& other)
  //   :d_input( other.d_input )
  //   , d_output( other.d_output )
  //   , d_size( other.d_size )
  //   , d_array( other.d_array )
  //   , d_settings( Settings::getInstance("TruthTable") )
  // {

  // }

private:
  int d_input;
  int d_output;
  int d_size;
  std::vector<std::vector<bool>> d_array;
  Settings* d_settings = Settings::getInstance("TruthTable");
};
