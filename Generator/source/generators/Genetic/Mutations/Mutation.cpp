#include <iostream>

#include "Mutation.h"
#include "MutationTruthTable.h"

template<>
std::vector<ChronosomeType<TruthTable, TruthTableParameters>> MutationType(
    MutationParameters i_mutationParameters,
    std::vector<ChronosomeType<TruthTable, TruthTableParameters>> i_population)
{
  if (i_mutationParameters.getMutationType() == MutationTypes::Binary)
    return MutationTruthTable::MutationBinary(i_mutationParameters, i_population);

  else if (i_mutationParameters.getMutationType() == MutationTypes::Density)
    return MutationTruthTable::MutationDensity(i_mutationParameters, i_population);

  else if (i_mutationParameters.getMutationType() == MutationTypes::AccessionDel)
    return MutationTruthTable::MutationAccessionDel(i_mutationParameters, i_population);

  else if (i_mutationParameters.getMutationType() == MutationTypes::InsertDel)
    return MutationTruthTable::MutationInsertDel(i_mutationParameters, i_population);

  else if (i_mutationParameters.getMutationType() == MutationTypes::Exchange)
    return MutationTruthTable::MutationExchange(i_mutationParameters, i_population);
    
  else if (i_mutationParameters.getMutationType() == MutationTypes::Delete)
    return MutationTruthTable::MutationDelete(i_mutationParameters, i_population);

  std::cout << "UNRECOGNIZED MtationType AT Mutation.cpp" << std::endl;
}
