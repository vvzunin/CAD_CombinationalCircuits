#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>


#include "ParentsParameters.h"
#include "../ChronosomeType.h"
#include "ParentsTruthTable.h"


template<typename Type, typename ParametersType>
std::vector<int> ParentsTypesWorker(ParentsParameters i_parentsParameters,
                              std::vector<ChronosomeType<Type, ParametersType>> i_population
);
template<>
inline std::vector<int> ParentsTypesWorker(ParentsParameters i_parentsParameters,
                              std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  if (i_parentsParameters.getParentsType() == ParentsTypes::Panmixia)
    return ParentsPanmixia(i_parentsParameters, i_population);
  else if (i_parentsParameters.getParentsType() == ParentsTypes::Inbringing)
    return ParentsInbrinding(i_parentsParameters, i_population);
  else if (i_parentsParameters.getParentsType() == ParentsTypes::Outbrinding)
    return ParentsOutbrinding(i_parentsParameters, i_population);
  else if (i_parentsParameters.getParentsType() == ParentsTypes::Tournament)
    return ParentsTournament(i_parentsParameters, i_population);
  //TODO: info about ParentsRoulette
  std::cout << "UNUSUAL ParentTypes IN ParentsTypes" << std::endl;
}