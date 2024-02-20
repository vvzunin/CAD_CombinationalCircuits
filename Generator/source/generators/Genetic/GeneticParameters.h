#pragma once

#include "./Selections/SelectionParameters.h"
#include "./Recombination/RecombinationParameters.h"
#include "./Mutations/MutationParameters.h"

/// class GeneticParameters
/// @param d_inputs The number of input parameters for the genetic algorithm
/// @param d_outputs The number of output parameters for the genetic algorithm
/// @param d_populationSize The field that stores the selection parameters for the genetic algorithm
/// @param d_recombinationParameters The field that stores the recombination parameters for the genetic algorithm
/// @param d_mutationParameters The field that stores mutation parameters for the genetic algorithm
/// @param d_populationSize The field that stores the population size for the genetic algorithm
/// @param d_numOfCycles A field that stores the number of cycles of the genetic algorithm execution. This determines the number of iterations that the algorithm will perform before shutting down or returning a result.
/// @param d_keyEndProcessIndex The field that stores the key end process index for the genetic algorithm
/// 
/// */

class GeneticParameters
{
public:
  GeneticParameters(int i_inputs = 0, int outs = 0);
  int getInputs() const;
  void setInputs(int i_inputs);
  int getOutputs() const;
  void setOutputs(int i_outputs);
  SelectionParameters getSelectionParameters() const;
  void setSelectionParameters(const SelectionParameters& i_sp);
  RecombinationParameters getRecombinationParameters() const;
  void setRecombinationParameters(const RecombinationParameters& i_rp);
  MutationParameters getMutationParameters() const;
  void setMutationParameters(const MutationParameters& i_mp);
  void setPopulationSize(int i_populationSize);
  void setNumOfCycles(int i_numOfCycles);
  void setKeyEndProcessIndex(double i_keyEndProcessIndex);
  double getKeyEndProcessIndex() { return d_keyEndProcessIndex; }
  int getNumOfCycles() { return d_numOfCycles; }

 
protected:
  int d_inputs;
  int d_outputs;
  int d_populationSize = 0;
  int d_numOfCycles = 0;
  SelectionParameters d_selectionParameters;
  RecombinationParameters d_recombinationParameters;
  MutationParameters d_mutationParameters;
  double d_keyEndProcessIndex = 0;
};

/// class OrientedGraphParameters
/// @param d_maxLevel In the context of a Oriented Graph, the level can determine the depth or distance from the root node to the end node. This value can be used to control the depth of graph traversal algorithms or other operations.
/// @param d_maxElements This field stores the maximum number of elements in a directed graph. This value can be used to limit the number of nodes or edges in the graph
/// 
/// */

//TODO: is it really need to place it there and not in ../../graph/OrientedGraph.h?
class OrientedGraphParameters : public GeneticParameters
{
public:
  int getMaxLevel() const;
  int getMaxElements() const;
  bool empty() const;

private:
  int d_maxLevel = 0;
  int d_maxElements = 0;
};


class TruthTableParameters : public GeneticParameters
{
public:
  TruthTableParameters(int i_inputs = 0, int i_outputs = 0);
  TruthTableParameters(const GeneticParameters& i_gp);
  int getPopulationSize() { return size(); }
  int size() const;
  bool operator== (const TruthTableParameters& r);
};
