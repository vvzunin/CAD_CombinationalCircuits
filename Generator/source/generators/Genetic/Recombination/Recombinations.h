#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "RecombinationParameters.h"
#include "RecombinationsTruthTable.h"

#include "../ChronosomeType.h"

template<typename Type, typename ParametersType>
std::vector<ChronosomeType<Type, ParametersType>> RecombinationType
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<Type, ParametersType>> i_population
);

template<>
inline std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationType
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  RecombinationTypes help_1 = i_recombinationParameters.getRecombinationType();
  RecombinationTypes help_2 = CrossingEachExitInTurnMany;
  if (help_1 == CrossingEachExitInTurnMany){
    std::cout << "1" << std::endl;
    std::vector<ChronosomeType<TruthTable, TruthTableParameters>> newPopulation = RecombinationsTruthTable::RecombinationCrossingEachExitInTurnMany(i_recombinationParameters, i_population);

    return RecombinationsTruthTable::RecombinationCrossingEachExitInTurnMany(
      i_recombinationParameters,
      i_population
    );
  }
  else{
    std::cout << "2"<< std::endl;
  }
  std::cout << "3"<< std::endl;
    
  else if (i_recombinationParameters.getRecombinationType() == RecombinationTypes::CrossingUniform)
    return RecombinationsTruthTable::RecombinationCrossingUniform(
      i_recombinationParameters,
      i_population
    );
  else if (i_recombinationParameters.getRecombinationType() == RecombinationTypes::CrossingTriadic)
    return RecombinationsTruthTable::RecombinationCrossingTriadic(
      i_recombinationParameters,
      i_population
    );
  else if (i_recombinationParameters.getRecombinationType() == RecombinationTypes::CrossingReducedReplacement)
    return RecombinationsTruthTable::RecombinationCrossingReducedReplacement(
      i_recombinationParameters,
      i_population
    );
  else if (i_recombinationParameters.getRecombinationType() == RecombinationTypes::CrossingShuffling)
    return RecombinationsTruthTable::RecombinationCrossingShuffling(
      i_recombinationParameters,
      i_population
    );
}