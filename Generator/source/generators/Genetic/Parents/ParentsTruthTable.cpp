// #include <vector>
// #include <map>
// #include <algorithm>
// #include <ctime>

// #include "ParentsTruthTable.h"
// #include "../../../AuxiliaryMethods.h"

// // std::vector<int> GetHemming(int i_t,
// //                             std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population)
// // {
// //   int count = 0;
// //   std::vector<std::vector<bool>> p1 = i_population[i_t].getChronosomeType().getOutTable();
// //   std::map<int, int> dict;
// //   std::vector<int> res;

// //   for (int i = 0; i < i_population.size(); ++i)
// //   {
// //     if (i != i_t)
// //     {
// //       count = 0;
// //       std::vector<std::vector<bool>> p2 = i_population[i].getChronosomeType().getOutTable();
// //       for (int j = 0; j < p1.size(); ++j)
// //       {
// //         for (int k = 0; k < p1[0].size(); ++k)
// //         {
// //           if (p2[j][k] == p1[j][k]) // what? is it Hemming dist?
// //             ++count;
// //         }
// //       }
// //       dict[i] = count;
// //     }
// //   }



// //   for (const auto &[key, value] : AuxMethods::sortDictByValue(dict))
// //     res.push_back(key);

// //   std::reverse(res.begin(), res.end());

// //   return res;
// // }

// // std::vector<int> ParentsPanmixia(
// //   ParentsParameters i_parentsParameters,
// //   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// // )
// // {
// //   std::srand(std::time(0));
// //   int parent1 = 0, parent2 = 0;
  
// //   while (i_population.size() > 2 && parent1 == parent2)
// //   {
// //     parent1 = rand() % i_population.size();
// //     parent2 = rand() % i_population.size();
// //   }

// //   return {parent1, parent2};
// // }

// // std::vector<int> ParentsInbrinding(
// //   ParentsParameters i_parentsParameters,
// //   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// // )
// // {
// //   std::srand(std::time(0));

// //   int parent1 = rand() % i_population.size();
// //   int parent2 = GetHemming(parent1, i_population).back();

// //   return {parent1, parent2};
// // }

// // std::vector<int> ParentsOutbrinding(
// //   ParentsParameters i_parentsParameters,
// //   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// // )
// // {
// //   std::srand(std::time(0));

// //   int parent1 = rand() % i_population.size();
// //   int parent2 = GetHemming(parent1, i_population).front();

// //   return {parent1, parent2};
// // }

// // std::vector<int> ParentsTournament(
// //   ParentsParameters i_parentsParameters,
// //   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// // )
// // {
// //   std::vector<int> beforeAdaptation = AuxMethods::getRandomIntList(
// //     i_parentsParameters.getTournamentNumber(),
// //     0,
// //     i_population.size(),
// //     false
// //   );

// //   std::map<int, double> adaptationIndex;

// //   for (const auto& k : beforeAdaptation)
// //     adaptationIndex[k] = i_population[k].getAdaptationIndex();

// //   std::vector<int> res;

// //   for (const auto &[key, value] : AuxMethods::sortDictByValue(adaptationIndex, false))
// //     res.push_back(key);
// //   std::reverse(res.begin(), res.end());

// //   return {res[0], res[1]};
// // }

// // //TODO: is this ParentTournament???
// // std::vector<int> ParentsRoulette(
// //   ParentsParameters i_parentsParameters,
// //   std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
// // )
// // {
// //   return ParentsTournament(i_parentsParameters, i_population);
// // }
