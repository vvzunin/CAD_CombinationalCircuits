#pragma once

#include <vector>

#include "../../TruthTable.h"
#include "../ChronosomeType.h"


namespace MutationTruthTable
{
std::vector<std::vector<bool>> MutationTable(std::vector<std::vector<bool>> i_table,
                                            double i_probability);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationBinary(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationDensity(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationAccessionDel(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationInsertDel(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationExchange(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationDelete(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationDelete(
  MutationParameters i_mutationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);
//namespace end
}