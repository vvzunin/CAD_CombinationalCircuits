#pragma once

#include "../Parents/ParentsParameters.h"


enum RecombinationTypes
{
  CrossingEachExitInTurnMany, // relative to several reference points
  CrossingUniform, //
  CrossingTriadic,
  CrossingReducedReplacement,
  CrossingShuffling
};

/// class RecombinationParameters
/// @param d_recombinationType Stores the recombination type. This enumeration (RecombinationTypes) represents the various recombination methods used in genetic algorithms, for example, "single-point crossover", "multipoint crossover", "uniform crossover" ***
/// @param d_parentsParameters Stores the parameters of the parents (parents parameters). This is an object of the Parent's Parameters class, which probably contains information about the selection of parents for the recombination process, for example, the type of parent selection and the size of the tournament
/// @param d_refPoints Stores the number of reference points for multipoint recombination. This value is used in some multipoint crossover methods to determine the number of points at which genetic material is exchanged between parents***
/// @param d_maskProbability Stores the probability of masking (mask probability) for uniform crossover. This value represents the probability of using a gene from one of the parents when performing a uniform crossover
/// @param d_recombinationNumber Stores the number of recombinations (recombination number). This value indicates how many times the recombination process will be performed when using the recombination operator***
/// 
/// */

class RecombinationParameters
{
public:
  RecombinationTypes getRecombinationType() const;
  ParentsParameters getParentsParameters() const;
  int getRefPoints() const;
  void setRefPoints(int i_refPoints);
  double getMaskProbability() const;
  void setRecombinationParameters(int i_rp);
  int getRecombinationParameters() const;
  bool operator== (const RecombinationParameters& r) const;
  void setMaskProbability(double i_maskProbability);
  void setParentsParameters(ParentsTypes i_parentsType, const int i_tournamentSize);
  void setRecombinationType(RecombinationTypes i_recombinationType);
private:
  RecombinationTypes d_recombinationType = RecombinationTypes::CrossingEachExitInTurnMany;
  ParentsParameters d_parentsParameters = ParentsParameters();
  int d_refPoints = 1;
  double d_maskProbability = 0.5;
  int d_recombinationNumber = 1;
};
