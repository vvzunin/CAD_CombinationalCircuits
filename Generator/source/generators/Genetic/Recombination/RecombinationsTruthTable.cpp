#include "RecombinationsTruthTable.h"
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
)
{
  // TODO: is it need? Parents<TruthTable, TruthTableParameters> p;
  int input = i_population[0].getChronosomeType().getInput();
  int output = i_population[0].getChronosomeType().getOutput();
  int size = i_population[0].getChronosomeType().size();
  int refPoints = i_recombinationParameters.getRefPoints();

  std::vector<int> referencePoints;
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];

    referencePoints = AuxMethods::getRandomIntList(refPoints, 0, size, false);

    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChronosomeType().getOutTable());
    parents.push_back(i_population[child2].getChronosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>());
    int stage = 0;

    for (int j = 0; j < size; ++j)
    {
      if (stage < referencePoints.size() && j == referencePoints[stage])
        stage++;
      child[j] = parents[stage % parents.size()][j];
    }

    TruthTable tt(input, output, child);
    ChronosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingUniform
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  //Parents<TruthTable, TruthTableParameters> p;
  int input = i_population[0].getChronosomeType().getInput();
  int output = i_population[0].getChronosomeType().getOutput();
  int size = i_population[0].getChronosomeType().size();

  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];
  
    TruthTable mask(
      input, 
      output, 
      i_recombinationParameters.getMaskProbability()
    );

    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChronosomeType().getOutTable());
    parents.push_back(i_population[child2].getChronosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));

    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        child[i][j] = parents[mask.getOutTable(i,j) ? 1 : 0][i][j];
      }
    }

    TruthTable tt(input, output, child);
    ChronosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingTriadic
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  int input = i_population[0].getChronosomeType().getInput();
  int output = i_population[0].getChronosomeType().getOutput();
  int size = i_population[0].getChronosomeType().size();

  int child3;
  std::srand(std::time(0));

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    child3 = 0;
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];

    while (child1 == child3 || child2 == child3)
      child3 = rand() % i_population.size();

    TruthTable mask(
      input,
      output,
      MutationTruthTable::MutationTable(
        i_population[child3].getChronosomeType().getOutTable(),
        i_recombinationParameters.getMaskProbability()
      )
    );

    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChronosomeType().getOutTable());
    parents.push_back(i_population[child2].getChronosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));

    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        child[i][j] = parents[mask.getOutTable(i,j) ? 0 : 1][i][j];
      }
    }

    TruthTable tt(
      input,
      output,
      child
    );
  
    ChronosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    i_population.push_back(tmp); // what????
  }

  i_population.clear(); // what??
  return i_population; // what????
}

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingReducedReplacement
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  std::srand(std::time(0));

  int input = i_population[0].getChronosomeType().getInput();
  int output = i_population[0].getChronosomeType().getOutput();
  int size = i_population[0].getChronosomeType().size();
  int referencePoint = rand() % size;

  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];
  
    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChronosomeType().getOutTable());
    parents.push_back(i_population[child2].getChronosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));

    for (int j = 0; j < output; ++j)
    {
      while (parents[0][referencePoint][j] == parents[1][referencePoint][j])
        referencePoint = rand() % size;

      for (int i = 0; i < size; ++i)
        child[i][j] = parents[i < referencePoint ? 0 : 1][i][j];
    }
    TruthTable tt(input, output, child);
    ChronosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChronosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingShuffling
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  std::srand(std::time(0));

  int input = i_population[0].getChronosomeType().getInput();
  int output = i_population[0].getChronosomeType().getOutput();
  int size = i_population[0].getChronosomeType().size();
  int referencePoint = rand() % size;

  std::vector<ChronosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];
  
    referencePoint = rand() % size;
    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChronosomeType().getOutTable());
    parents.push_back(i_population[child2].getChronosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));
  
    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        if (rand() % 2)
          swap(parents[0][i][j], parents[1][i][j]);
      }
    }

    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        child[i][j] = parents[i < referencePoint ? 0 : 1][i][j];
      }
    }
    TruthTable tt(input, output, child);
    ChronosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}
//namespace end
}