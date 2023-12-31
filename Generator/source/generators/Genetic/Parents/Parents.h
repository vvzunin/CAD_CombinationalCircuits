#pragma once

#include <vector>

#include "ParentsParameters.h"
#include "../ChronosomeType.h"


template<typename Type, typename ParametersType>
std::vector<int> ParentsTypesWorker(ParentsParameters i_parentsParameters,
                              std::vector<ChronosomeType<Type, ParametersType>> i_population
);
