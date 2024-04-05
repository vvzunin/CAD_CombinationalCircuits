#include <random>
#include <vector>
#include <ctime>
#include <iostream>

#include "TruthTable.h"

TruthTable::TruthTable()
{
  d_settings = Settings::getInstance("TruthTable");
}

TruthTable::TruthTable(const Chronosome<TruthTableParameters>& i_chr)
{
  d_settings = Settings::getInstance("TruthTable");
}

void TruthTable::generateRandom(TruthTableParameters i_gp)
{
  std::srand(std::time(0));
  if (i_gp.getInputs() == 0)
  {
    i_gp = TruthTableParameters();
    i_gp.setInputs(rand() % d_settings->getMaxInputs());
    i_gp.setOutputs(rand() % d_settings->getMaxOutputs());
  }

  d_input = i_gp.getInputs();
  d_output = i_gp.getOutputs();
  d_size = 1u << d_input;

  d_array.clear();
  d_array.resize(d_size);

  for (int i = 0; i < d_size; ++i)
  {
    d_array[i].resize(i_gp.getOutputs());
    for (int j = 0; j < i_gp.getOutputs(); ++j)
      d_array[i][j] = (rand() % 2) == 1;
  }
}

std::vector<std::vector<bool>> TruthTable::convToBinary() const
{
  std::vector<std::vector<bool>> bin(d_size, std::vector<bool>(d_input));
  for (int i = 0; i < d_size; ++i)
  {
    for (int j = d_input - 1, tmp = i; j >= 0; --j)
    {
      bin[i][j] = (tmp % 2) == 1;
      tmp /= 2;
    }
  }
  return bin;
}

TruthTable::TruthTable(int i_input, int i_output, const std::vector<std::vector<bool>>& i_array) :
  d_input(i_input),
  d_output(i_output)
{
  d_size = 1u << d_input;
  if (i_array.size() == 0 || i_array.size() != d_size || i_array[0].size() != d_output)
    generateRandom(TruthTableParameters(d_input, d_output));
  else
    d_array = i_array;
}

TruthTable::TruthTable(const TruthTable& i_tt, std::vector<std::vector<bool>> i_array)
{
  d_input = i_tt.d_input;
  d_output = i_tt.d_output;
  d_size = i_tt.d_size;
  if (i_array.empty() || i_array.size() != d_size || i_array[0].size() != d_output){
    generateRandom( TruthTableParameters(d_input, d_output));
  }
  else{
    d_array = i_array;
  }
}

TruthTable::TruthTable(int i_input, int i_output, double i_p) :
  d_input(i_input),
  d_output(i_output)
{
  d_size = (1u << i_input);
  generateTable(i_p);
}

int TruthTable::getInput() const
{
  return d_input;
}

int TruthTable::getOutput() const
{
  return d_output;
}

int TruthTable::size() const
{
  return d_size;
}

std::vector<std::vector<bool>> TruthTable::getOutTable() const
{
  return d_array;
}

bool TruthTable::getOutTable(int i, int j) const
{
  return d_array.at(i).at(j);
}

void TruthTable::generateTable(double i_p)
{
  
  std::srand(std::time(0));
  if (i_p == 0)
  {
    d_array.clear();
    d_array.resize(d_size);
    for (int i = 0; i < d_size; ++i)
    {
      d_array[i].resize(d_output);
      for (int j = 0; j < d_output; ++j)
        d_array[i][j] = (rand() % 2) == 1;
    }
  }
  if (i_p > 0 && i_p <= 1)
  {
    d_array.clear();
    d_array.resize(d_size);
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    for (int i = 0; i < d_size; ++i)
    {
      d_array[i].resize(d_output);
      for (int j = 0; j < d_output; ++j)
        d_array[i][j] = distribution(generator) < i_p;
    }
  }
}

void TruthTable::printTable() const
{
  if (d_array.empty())
  {
    std::cout << "Пустой массив";
  }
  for (const std::vector<bool>& inner_vec : d_array)
  {
    std::cout << "(";
    for (bool val : inner_vec)
    {
      std::cout << val << ";";
    }
    std::cout << ")" << std::endl;
  }
}

bool TruthTable::operator== (const TruthTable& r) const
{
  return std::tie(d_input, d_output, d_size, d_array) ==
    std::tie(r.d_input, r.d_output, r.d_size, r.d_array);
}
