#include "RecombinationsTruthTable.h"
#include "../Parents/Parents.h"
#include "../Mutations/MutationTruthTable.h"
#include "../../../AuxiliaryMethods.h"
#include <ctime>

std::vector<ChromosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingEachExitInTurnMany
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  // TODO: is it need? Parents<TruthTable, TruthTableParameters> p;
  int input = i_population[0].getChromosomeType().getInput();
  int output = i_population[0].getChromosomeType().getOutput();
  int size = i_population[0].getChromosomeType().size();
  int refPoints = i_recombinationParameters.getRefPoints();

  std::vector<int> referencePoints;
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> survivors;

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
    parents.push_back(i_population[child1].getChromosomeType().getOutTable());
    parents.push_back(i_population[child2].getChromosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>());
    int stage = 0;

    for (int j = 0; j < size; ++j)
    {
      if (stage < referencePoints.size() && j == referencePoints[stage])
        stage++;
      child[j] = parents[stage % parents.size()][j];
    }

    TruthTable tt(input, output, child);
    ChromosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChromosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingUniform
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  //Parents<TruthTable, TruthTableParameters> p;
  int input = i_population[0].getChromosomeType().getInput();
  int output = i_population[0].getChromosomeType().getOutput();
  int size = i_population[0].getChromosomeType().size();

  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> survivors;

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
    parents.push_back(i_population[child1].getChromosomeType().getOutTable());
    parents.push_back(i_population[child2].getChromosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));

    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        child[i][j] = parents[mask.getOutTable(i,j) ? 1 : 0][i][j];
      }
    }

    TruthTable tt(input, output, child);
    ChromosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChromosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingTriadic
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> i_population
)
{
  int input = i_population[0].getChromosomeType().getInput();
  int output = i_population[0].getChromosomeType().getOutput();
  int size = i_population[0].getChromosomeType().size();

  int child3;

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
      child3 = AuxMethods::getRandInt(0, i_population.size());

    TruthTable mask(
      input,
      output,
      MutationTable(
        i_population[child3].getChromosomeType().getOutTable(),
        i_recombinationParameters.getMaskProbability()
      )
    );

    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChromosomeType().getOutTable());
    parents.push_back(i_population[child2].getChromosomeType().getOutTable());
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
  
    ChromosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    i_population.push_back(tmp); // what????
  }

  i_population.clear(); // what??
  return i_population; // what????
}

std::vector<ChromosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingReducedReplacement
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> i_population
)
{

  int input = i_population[0].getChromosomeType().getInput();
  int output = i_population[0].getChromosomeType().getOutput();
  int size = i_population[0].getChromosomeType().size();
  int referencePoint = AuxMethods::getRandInt(0, size);

  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];
  
    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChromosomeType().getOutTable());
    parents.push_back(i_population[child2].getChromosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));

    for (int j = 0; j < output; ++j)
    {
      while (parents[0][referencePoint][j] == parents[1][referencePoint][j])
        referencePoint = AuxMethods::getRandInt(0, size);

      for (int i = 0; i < size; ++i)
        child[i][j] = parents[i < referencePoint ? 0 : 1][i][j];
    }
    TruthTable tt(input, output, child);
    ChromosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}

std::vector<ChromosomeType<TruthTable, TruthTableParameters>> RecombinationCrossingShuffling
(
  RecombinationParameters i_recombinationParameters,
  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> i_population
)
{

  int input = i_population[0].getChromosomeType().getInput();
  int output = i_population[0].getChromosomeType().getOutput();
  int size = i_population[0].getChromosomeType().size();
  int referencePoint = AuxMethods::getRandInt(0, size);

  std::vector<ChromosomeType<TruthTable, TruthTableParameters>> survivors;

  for (int cr = 0; cr < i_recombinationParameters.getRecombinationParameters(); ++cr)
  {
    std::vector<int> parentsInt = ParentsTypesWorker(
      i_recombinationParameters.getParentsParameters(),
      i_population
    );
    int child1 = parentsInt[0];
    int child2 = parentsInt[1];
  
    referencePoint = AuxMethods::getRandInt(0, size);
    std::vector<std::vector<std::vector<bool>>> parents;
    parents.push_back(i_population[child1].getChromosomeType().getOutTable());
    parents.push_back(i_population[child2].getChromosomeType().getOutTable());
    std::vector<std::vector<bool>> child(size, std::vector<bool>(output));
  
    for (int i = 0; i < size; ++i)
    {
      for (int j = 0; j < output; ++j)
      {
        if (AuxMethods::getRandInt(0, 1, true))
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
    ChromosomeType<TruthTable, TruthTableParameters> tmp("ind" + std::to_string(cr + 1), tt);
    survivors.push_back(tmp);
  }
  return survivors;
}
