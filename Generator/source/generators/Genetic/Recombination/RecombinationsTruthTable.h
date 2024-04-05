#pragma once

#include <vector>

#include "RecombinationParameters.h"
#include "../ChronosomeType.h"
#include "../Parents/ParentsParameters.h"
#include "../../TruthTable.h"
#include "../Parents/Parents.h"
#include "../Mutations/MutationTruthTable.h"
#include "../../../AuxiliaryMethods.h"
#include <ctime>

namespace RecombinationsTruthTable
{
std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingEachExitInTurnMany
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingUniform
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingTriadic
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingReducedReplacement
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingShuffling
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
);
//namespace end
}