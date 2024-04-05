#include "DataBaseGeneratorParameters.h"

DataBaseGeneratorParameters::DataBaseGeneratorParameters(
    int i_minInputs,
    int i_maxInputs,
    int i_minOutputs,
    int i_maxOutputs,
    int i_eachIteration,
    GenerationTypes i_gt,
    GenerationParameters i_gp
  ) : d_minInputs(i_minInputs),
      d_maxInputs(i_maxInputs),
      d_minOutputs(i_minOutputs),
      d_maxOutputs(i_maxOutputs),
      d_eachIteration(i_eachIteration),
      d_generationTypes(i_gt),
      d_generationParameters(i_gp) {};

void DataBaseGeneratorParameters::setInputs(int i_inputs)
{
  d_generationParameters.setInputs(i_inputs);
}

void DataBaseGeneratorParameters::setOutputs(int i_outputs)
{
  d_generationParameters.setOutputs(i_outputs);
}

void DataBaseGeneratorParameters::setIteration(int i_iteration)
{
  d_generationParameters.setIteration(i_iteration);
}

void DataBaseGeneratorParameters::setName(const std::string& i_name)
{
  d_generationParameters.setName(i_name);
}

int DataBaseGeneratorParameters::getMinInputs() const
{
  return d_minInputs;
}

int DataBaseGeneratorParameters::getMaxInputs() const
{
  return d_maxInputs;
}

int DataBaseGeneratorParameters::getMinOutputs() const
{
  return d_minOutputs;
}

int DataBaseGeneratorParameters::getMaxOutputs() const
{
  return d_maxOutputs;
}

int DataBaseGeneratorParameters::getEachIteration() const
{
  return d_eachIteration;
}

GenerationParameters DataBaseGeneratorParameters::getGenerationParameters() const
{
  return d_generationParameters;
}

std::string DataBaseGeneratorParameters::getGenerationTypeString() const
{
  if (d_generationTypes == GenerationTypes::FromRandomTruthTable)
    return "FromRandomTruthTable";
  if (d_generationTypes == GenerationTypes::RandLevel)
    return "RandLevel";
  if (d_generationTypes == GenerationTypes::NumOperation)
    return "NumOperation";
  if (d_generationTypes == GenerationTypes::Genetic)
    return "Genetic";
  if (d_generationTypes == GenerationTypes::Summator)
      return "Summator";
  if (d_generationTypes == GenerationTypes::Comparison)
    return "Comparison";
  if (d_generationTypes == GenerationTypes::Encoder)
      return "Encoder";
  if (d_generationTypes == GenerationTypes::Multiplier)
      return "Multiplier";
  if (d_generationTypes == GenerationTypes::Multiplexer)
      return "Multiplexer";
  if (d_generationTypes == GenerationTypes::Parity)
      return "Parity";
  if (d_generationTypes == GenerationTypes::Decoder)
      return "Decoder";
}
