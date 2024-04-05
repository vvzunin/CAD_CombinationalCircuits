#pragma once

#include "../generators/GenerationParameters.h"
#include "../Settings.h"

enum GenerationTypes
{
  FromRandomTruthTable,
  RandLevel,
  NumOperation,
  Genetic,
  Summator,
  Comparison,
  Encoder,
  Multiplier,
  Multiplexer,
  Parity,
  Decoder
};

class DataBaseGeneratorParameters //TODO: maybe it need heritance from GenerationParameters?
{
public:
  DataBaseGeneratorParameters(
    int i_minInputs,
    int i_maxInputs,
    int i_minOutputs,
    int i_maxOutputs,
    int i_eachIteration,
    GenerationTypes i_gt,
    GenerationParameters i_gp
  );

  void setInputs(int i_inputs);
  void setOutputs(int i_outputs);
  void setIteration(int i_iteration);
  void setName(const std::string& i_name);
  int getMinInputs() const;
  int getMaxInputs() const;
  int getMinOutputs() const;
  int getMaxOutputs() const;
  int getEachIteration() const;
  GenerationParameters getGenerationParameters() const;

  std::string getGenerationTypeString() const;

private:
  int d_minInputs = 0;
  int d_maxInputs = 0;
  int d_minOutputs = 0;
  int d_maxOutputs = 0;
  int d_eachIteration = 0;
  GenerationTypes d_generationTypes = GenerationTypes::FromRandomTruthTable;
  GenerationParameters d_generationParameters;
};
