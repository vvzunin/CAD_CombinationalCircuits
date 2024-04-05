#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <nlohmann/json.hpp>

#include <unistd.h>
#include <getopt.h>

#include "./DataBase/DataBaseGenerator.h"
#include "./DataBase/DataBaseGeneratorParameters.h"
#include "./generators/GenerationParameters.h"

void runGenerationFromJson(std::string json_path);

int main(int argc, char **argv)
{
  std::string json_path;
  std::srand(std::time(nullptr));
  // Use getopt to parse command line arguments

  const char *const short_opts = "j:n:";
  const option long_opts[] = {
      {"json_path", required_argument, nullptr, 'j'},
      {"num_nodes", required_argument, nullptr, 'n'}};
  int c;

  int opt;
  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
  {
    switch (opt)
    {
    case 'j':
      json_path = optarg;
      break;
    case '?':
      // Unknown option or missing argument
      break;
    default:
      std::cerr << "Unknown error while parsing command line arguments." << std::endl;
      return 1;
    }
  }

  runGenerationFromJson(json_path);
}
void runGenerationFromJson(std::string json_path)
{
  std::srand(NULL);

  std::ifstream f(json_path);
  nlohmann::json DATA = nlohmann::json::parse(f);
  // Read all json objects in json file.
  for (auto it = DATA.begin(); it != DATA.end(); it++)
  {
    nlohmann::json data = *it;

    GenerationTypes gt;
    if (data["type_of_generation"] == "From Random Truth Table")
      gt = GenerationTypes::FromRandomTruthTable;
    if (data["type_of_generation"] == "Rand Level")
      gt = GenerationTypes::RandLevel;
    if (data["type_of_generation"] == "Num Operation")
      gt = GenerationTypes::NumOperation;
    if (data["type_of_generation"] == "Genetic")
      gt = GenerationTypes::Genetic;
    if (data["type_of_generation"] == "Summator")
      gt = GenerationTypes::Summator;
    if (data["type_of_generation"] == "Comparison")
      gt = GenerationTypes::Comparison;
    if (data["type_of_generation"] == "Multiplier")
        gt = GenerationTypes::Multiplier;
    if (data["type_of_generation"] == "Multiplier")
        gt = GenerationTypes::Multiplier;
    if (data["type_of_generation"] == "Encoder")
        gt = GenerationTypes::Encoder;
    if (data["type_of_generation"] == "Parity")
        gt = GenerationTypes::Parity;
    if (data["type_of_generation"] == "Decoder")
        gt = GenerationTypes::Decoder;
    int requestIdINT = data["id"];
    std::string requestId = std::to_string(requestIdINT);
    assert(requestId != "");

    std::cout << data.dump(4);

    int minInputs = data["min_in"];
    int maxInputs = data["max_in"];
    int minOutputs = data["min_out"];
    int maxOutputs = data["max_out"];
    int repeats = data["repeat_n"];
    int maxLevel = data["max_level"];
    int maxElement = data["max_elem"];
    //  bool limit = data["limit_generation"];
    bool CNFF = data["CNFF"];
    bool CNFT = data["CNFT"];
    bool LeaveEmptyOut = data["leave_empty_out"];
    int numOfSurv = data["surv_num"];
    std::string datasetId = data["dataset_id"];

    std::string mutType = data["mut_type"];
    MutationTypes mType;
    if (mutType == "Binary")
      mType = MutationTypes::Binary;
    if (mutType == "Density")
      mType = MutationTypes::Density;
    if (mutType == "AccessionDel")
      mType = MutationTypes::AccessionDel;
    if (mutType == "InsertDel")
      mType = MutationTypes::InsertDel;
    if (mutType == "Exchange")
      mType = MutationTypes::Exchange;
    if (mutType == "Delete")
      mType = MutationTypes::Delete;

    double mutChance = data["mut_chance"];
    int exchangeType = data["swap_type"];
    double outRatio = data["out_ratio"];
    double probabilityTruthTable = data["ratio_in_table"];
    int recNum = data["rec_num"];
    int refPoints = data["ref_points"];
    int tourSize = data["tour_size"];

    std::string selectionTypeParent = data["selection_type_parent"];
    ParentsTypes selecTypeParent;
    if (selectionTypeParent == "Panmixia")
      selecTypeParent = ParentsTypes::Panmixia;
    if (selectionTypeParent == "Inbringing")
      selecTypeParent = ParentsTypes::Inbringing;
    if (selectionTypeParent == "Outbrinding")
      selecTypeParent = ParentsTypes::Outbrinding;
    if (selectionTypeParent == "Tournament")
      selecTypeParent = ParentsTypes::Tournament;
    if (selectionTypeParent == "Roulette")
      selecTypeParent = ParentsTypes::Roulette;

    std::string recombinationType = data["playback_type"];
    RecombinationTypes recombType;
    if (recombinationType == "CrossingEachExitInTurnMany")
      recombType = RecombinationTypes::CrossingEachExitInTurnMany;
    if (recombinationType == "CrossingUniform")
      recombType = RecombinationTypes::CrossingUniform;
    if (recombinationType == "CrossingTriadic")
      recombType = RecombinationTypes::CrossingTriadic;
    if (recombinationType == "CrossingReducedReplacement")
      recombType = RecombinationTypes::CrossingReducedReplacement;
    if (recombinationType == "CrossingShuffling")
      recombType = RecombinationTypes::CrossingShuffling;

    double maskProb = data["mask_prob"];
    int populationSize = data["population_size"];
    int numOfCycles = data["cycles"];
    int inputs = minInputs;
    int outputs = minOutputs;

    std::string selectionType = data["selection_type"];
    SelectionTypes selType;
    if (selectionType == "Base")
      selType = SelectionTypes::Base;

    int survNum = data["surv_num"];

    bool overflowIn = data["overflowIn"];
    bool overflowOut = data["overflowOut"];
    bool minus = data["minus"];

    bool compare0 = data["="];
    bool compare1 = data["<"];
    bool compare2 = data[">"];

    std::map<std::string, int> m;
    std::vector<std::string> v =
        {"num_and", "num_nand", "num_or", "num_not",
         "num_nor", "num_buf", "num_xor", "num_xnor"};

    for (auto &el : data.items())
    {
      if (std::find(v.begin(), v.end(), el.key()) != v.end())
        m.insert({el.key().substr(4, 10), el.value()});
    }

    //JSON params = JSON::Read("params.json");

    // TODO:: make function that return DataBaseGeneratorParameters from json
    // Recording of json data to gp
    GenerationParameters gp(datasetId, requestId, inputs, outputs, repeats, maxLevel, maxElement);

    gp.setName(datasetId);
    gp.setCNFF(CNFF);
    gp.setCNFT(CNFT);
    //  gp.setLimit(limit);
    gp.setNumOperationParameters(m, LeaveEmptyOut);
    gp.setSummatorParameters(overflowIn, overflowOut, minus);
    gp.setComparisonParameters(compare0, compare1, compare2);
    gp.setPopulationSize(populationSize);
    gp.setNumOfCycles(numOfCycles);
    gp.setRecombinationParameters(selecTypeParent, tourSize, recombType, refPoints, maskProb, recNum);
    gp.setMutationParameters(mType, mutChance, exchangeType, probabilityTruthTable);
    gp.setSelectionParameters(selType, survNum);
    gp.setKeyEndProcessIndex(outRatio);
    //  gp.setGeneticParameters(numOfSurv, mutType, mutChance, swapType, ratioInTable, recNum, refPoints, tourSize,  selectionTypeParent);
    //
    DataBaseGeneratorParameters dbgp(minInputs, maxInputs, minOutputs, maxOutputs, repeats, gt, gp);

    DataBaseGenerator generator(dbgp);

    generator.generateType(dbgp, false);
  }
}
