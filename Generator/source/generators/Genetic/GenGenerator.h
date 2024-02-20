#pragma once

#include <string>
#include <memory>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
#include <string>
#include <utility>
#include <iostream>

#include "../TruthTable.h"
#include "../SimpleGenerators.h"
#include "../Parser.h"
#include "../../circuits/Circuit.h"
#include "filesTools/FilesTools.h"
#include "ChronosomeType.h"

#include "Recombination/Recombinations.h"
#include "Selections/Selections.h"
#include "Recombination/Recombinations.h"
#include "Mutations/Mutation.h"

int getNumFolderFromString(const std::string& path);//Linker without this line throw error.


/// class GeneticGenerator
/// @param d_inputs A field that stores the number of input parameters for the genetic algorithm generator
/// @param d_outputs A field that stores the number of output parameters for the genetic algorithm generator
/// @param d_numCross The field that stores the number of crossovers in the genetic generator
/// @param d_population The field that stores the population of chromosomes in the genetic generator. A population is a set of chromosomes that is used in the process of generating new objects
/// @param d_settings The field that stores a pointer to the settings object for the genetic generator
/// @param d_parameters The field that stores the parameters for the genetic generator
/// @param d_mainPath The field that stores the main path for the genetic generator
/// @param d_foldersCount The field that stores the number of folders for the genetic generator
/// 
/// */

template<typename Type, typename ParametersType>
class GeneticGenerator
{
public:
  GeneticGenerator(const ParametersType& i_parameters,
                   std::pair<int, int> i_inout,
                   const std::string& i_mainPath = "");
  std::vector<ChronosomeType<Type, ParametersType>> generate();

private:
  int d_inputs;
  int d_outputs;
  int d_numCross;
  std::vector<ChronosomeType<Type, ParametersType>> d_population;
  std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
  ParametersType d_parameters;
  std::string d_mainPath;
  int d_foldersCount = 0;

  void savePopulation(
    const std::vector<ChronosomeType<Type, ParametersType>>& i_population
  );
  void createPopulation();
  double endProcessFunction() const;
};
template<typename Type, typename ParametersType>
GeneticGenerator<Type, ParametersType>::GeneticGenerator(
    const ParametersType& i_parameters,
    std::pair<int, int> i_inout,
    const std::string& i_mainPath) :
    d_parameters(i_parameters),
    d_inputs(i_inout.first),
    d_outputs(i_inout.second),
    d_mainPath(i_mainPath)
{
    std::string dataPath = d_settings->getDatasetPath() + "/Genetic";//TODO:: Make general function
    if (FilesTools::isDirectoryExists(dataPath))
    {
        for (const auto& entry : std::filesystem::directory_iterator(dataPath))
        {
            d_foldersCount = std::max(d_foldersCount, getNumFolderFromString(entry.path()) + 1);
        }
    }
    else
    {
        std::cout << "Folder for data not exists, you need to create Directory \""
            << dataPath << "\"" << std::endl;
        std::exit(0);
    }
}

template<typename Type, typename ParametersType>
void GeneticGenerator<Type, ParametersType>::savePopulation(
    const std::vector<ChronosomeType<Type, ParametersType>>& i_population
)
{
    for (const auto& ttp : i_population)
    {
        Type tt;
        tt = ttp.getChronosomeType();

        SimpleGenerators tftt;
        std::vector<std::pair<std::string, std::vector<std::string>>> circs;
        circs.push_back({ d_settings->getGenerationMethodPrefix("Genetic") + std::to_string(d_foldersCount++), tftt.cnfFromTruthTable(tt, true) });
        circs.push_back({ d_settings->getGenerationMethodPrefix("Genetic") + std::to_string(d_foldersCount++), tftt.cnfFromTruthTable(tt, false) });

        for (const auto& nameexpr : circs)
        {
            std::string name = nameexpr.first;
            std::vector<std::string> expr = nameexpr.second;
            Parser pCNFT(expr);
            pCNFT.parseAll();
            OrientedGraph graph = pCNFT.getGraph();
            Circuit c(graph, expr);
            c.setTable(tt);
            c.setPath(d_mainPath + "/Genetic/");
            c.setCircuitName(name);
            c.generate();
        }
    }
}

template<typename Type, typename ParametersType>
void GeneticGenerator<Type, ParametersType>::createPopulation()
{
    d_population.clear();
    for (int i = 0; i < d_parameters.getPopulationSize(); ++i)
    {
        Type gen;
        gen.generateRandom(d_parameters);
        ChronosomeType<Type, ParametersType> ind("ind" + std::to_string(i), gen);
        d_population.push_back(ind);
    }
    savePopulation(d_population);
}

template<typename Type, typename ParametersType>
std::vector<ChronosomeType<Type, ParametersType>> GeneticGenerator<Type, ParametersType>::generate()
{   
    createPopulation();
    double d = endProcessFunction();
    for (int i = 0; (i < d_parameters.getNumOfCycles()) && (endProcessFunction() < d_parameters.getKeyEndProcessIndex()); ++i)
    {
        std::vector<ChronosomeType<Type, ParametersType>> newPopulation = RecombinationType<Type, ParametersType>(d_parameters.getRecombinationParameters(), d_population);
        std::vector<ChronosomeType<Type, ParametersType>> mutants = MutationType<Type, ParametersType>(d_parameters.getMutationParameters(), newPopulation);
        d_population = SelectionType<Type, ParametersType>(d_parameters.getSelectionParameters(), mutants);
        savePopulation(d_population);
    }

    return d_population;
}

template<typename Type, typename ParametersType>
double GeneticGenerator<Type, ParametersType>::endProcessFunction() const
{
    double max = 0;
    for (const auto& i : d_population)
    {
        max = std::max(max, i.getAdaptationIndex());
    }

    return max;
}
