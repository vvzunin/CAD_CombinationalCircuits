#pragma once

#include <vector>
#include <ctime>

#include "SelectionParameters.h"
#include "SelectionsTruthTable.h"

// namespace SelectionsTruthTable

// {
// inline std::vector<ChronosomeType<TruthTable, TruthTableParameters>> SelectionBase(
//   SelectionParameters i_selectionParameters,
//   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// )
// {
//   std::srand(std::time(0));

//   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> survivors;
//   while (survivors.size() < i_selectionParameters.getNumOfSurvivors())
//   {
//     int r1 = 0, r2 = 0;
//     while (i_population.size() > 2 && r2 == r1)
//     {
//       r1 = rand() % i_population.size();
//       r2 = rand() % i_population.size();
//     }

//     if ((i_population[r1].getAdaptationIndex() < i_population[r2].getAdaptationIndex()) &&
//         (std::find(survivors.begin(), survivors.end(), i_population[r1]) == survivors.end()))
//     {
//       survivors.push_back(i_population[r1]);
//     } else if ((i_population[r1].getAdaptationIndex() > i_population[r2].getAdaptationIndex()) &&
//                (std::find(survivors.begin(), survivors.end(), i_population[r2]) == survivors.end()))
//     {
//       survivors.push_back(i_population[r2]);
//     } else if (i_population[r1].getAdaptationIndex() == i_population[r2].getAdaptationIndex())
//     {
//       if (std::find(survivors.begin(), survivors.end(), i_population[r1]) == survivors.end())
//         survivors.push_back(i_population[r1]);
//       else if (std::find(survivors.begin(), survivors.end(), i_population[r1]) == survivors.end())
//         survivors.push_back(i_population[r1]);
//       else if (i_population.size() < i_selectionParameters.getNumOfSurvivors())
//         survivors.push_back(i_population[r1]);
//     }
//   }
//   return survivors;
// }
// }
