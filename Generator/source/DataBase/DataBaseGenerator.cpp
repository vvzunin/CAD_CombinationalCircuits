#include <iostream>
#include <filesystem>

#include "../generators/TruthTable.h"
#include "../generators/Genetic/GeneticParameters.h"
#include "../generators/SimpleGenerators.h"
#include "../generators/Parser.h"
#include "../generators/Genetic/GenGenerator.h"
#include "../circuits/CircuitsParameters.h"
#include "../circuits/Circuit.h"
#include "../FilesTools.h"
#include "DataBaseGenerator.h"


void DataBaseGenerator::generateType(
  const DataBaseGeneratorParameters& i_dbgp,
  bool parallel
)
{

  std::string s = i_dbgp.getGenerationTypeString();
  std::function<void(const GenerationParameters&)> generator = getGenerateMethod(s);
  //TODO: make normal code

  std::string dir = d_settings->getDatasetPath() + "/" + i_dbgp.getGenerationParameters().getName() + "/" + i_dbgp.getGenerationParameters().getRequestId();
  // Creating nested directories 
  std::filesystem::create_directories(d_settings->getDatasetPath() + "/" + i_dbgp.getGenerationParameters().getName());
  std::filesystem::create_directory(dir);

  d_mainPath = dir + "/";
  d_dirCount = 0;
  //std::cout << "Inside of generateType: " << std::endl;
  //std::cout << "dir = " << dir << std::endl;
  //std::cout << "i_dbgp.getGenerationParameters().getRequestId() returned: " << i_dbgp.getGenerationParameters().getRequestId() << std::endl;
  //std::cout << "i_dbgp.getMaxInputs() returned: " << i_dbgp.getMaxInputs() << std::endl;
  //std::cout << "i_dbgp.getMinInputs() returned: " << i_dbgp.getMinInputs() << std::endl;
  //std::cout << "i_dbgp.getMaxOutputs() returned: " << i_dbgp.getMaxOutputs() << std::endl;
  //std::cout << "i_dbgp.getMinOutsputs() returned: " << i_dbgp.getMinOutputs() << std::endl;
  if (std::filesystem::is_directory(dir))
  {
    for (const auto item : FilesTools::getDirectories(dir))
    {
      std::string s0 = item;
      auto pos = s0.find(d_settings->getGenerationMethodPrefix(s));
      s0.replace(pos, d_settings->getGenerationMethodPrefix(s).size(), "");
      
      auto jk = s0.find("_"); // TODO: maybe it need last find of "_"?
      if (jk == std::string::npos)
        jk = s0.size();
      
      s0 = s0.substr(0, jk);

      d_dirCount = std::max(d_dirCount, std::stoi(s0) + 1); // TODO: in general code n is int32, is it really need?
    }
  }

  for (int i = i_dbgp.getMinInputs(); i <= i_dbgp.getMaxInputs(); ++i)
  {
    for (int j = i_dbgp.getMinOutputs(); j <= i_dbgp.getMaxOutputs(); ++j)
    {
      d_parameters.setInputs(i);
      d_parameters.setOutputs(j);

      if (parallel)
      {
        exit(1); //TODO: write to multithreading
      }
      else
      {
        for (int tt = 0; tt < i_dbgp.getEachIteration(); ++tt)
        {
          //TODO: it is that Rustam told about iteration?
          d_parameters.setIteration(tt);
          d_parameters.setName(d_settings->getGenerationMethodPrefix(s) + std::to_string(d_dirCount));
	  //std::cout << "d_parameters.getGenerationParameters().getName() returned: " << d_parameters.getGenerationParameters().getName() << "	when tt == " << tt << std::endl;
	  //std::cout << "d_parameters.getGenerationParameters().getInputs() returned: " << d_parameters.getGenerationParameters().getInputs() << "	when tt == " << tt << std::endl;
	  //std::cout << "d_parameters.getGenerationParameters().getOutputs() returned: " << d_parameters.getGenerationParameters().getOutputs() << "	when tt == " << tt << std::endl;
	  //std::cout << "d_parameters.getGenerationParameters().getOutputs() returned: " << d_parameters.getGenerationParameters().getOutputs() << "	when tt == " << tt << std::endl;
	  //std::cout << "d_parameters.getGenerationParameters().getRequestId() returned: " << d_parameters.getGenerationParameters().getRequestId() << "		when tt == " << tt << std::endl;
	  //std::cout << "d_parameters.getGenerationParameters().getIteration() returned: " << d_parameters.getGenerationParameters().getIteration() << "		when tt == " << tt << std::endl;
	  generator(d_parameters.getGenerationParameters());
      d_dirCount++;
        }
      }
    }
    
  }
}


void DataBaseGenerator::generateDataBaseFromRandomTruthTable(const GenerationParameters& i_param)
{
  TruthTable tt(i_param.getInputs(), i_param.getOutputs(), {});
  tt.generateRandom({i_param.getInputs(), i_param.getOutputs()});

  SimpleGenerators tftt;

  std::vector<std::pair<std::string, std::vector<std::string>>> circs;

  if (i_param.getCNF().getCNFT()) 
    circs.push_back({"CNFT", tftt.cnfFromTruthTable(tt, true)});

  if (i_param.getCNF().getCNFF())
    circs.push_back({"CNFF", tftt.cnfFromTruthTable(tt, false)});

  if (i_param.getZhegalkin().getZhegalkin())
    circs.push_back({"Zhegalkin", tftt.zhegalkinFromTruthTable(tt)});
  for (const auto& [name, expr] : circs)
  {
    Parser pCNFT(expr);
    pCNFT.parseAll();
    
    OrientedGraph graph = pCNFT.getGraph();
    Circuit c(graph, expr);
    c.setTable(tt);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName() + "_" + name);
    c.generate();
  }
}

void DataBaseGenerator::generateDataBaseRandLevel(const GenerationParameters& i_param)
{
  SimpleGenerators generator;
  std::vector<std::pair<std::string, OrientedGraph>> circs;
  circs.push_back({"RandLevel",
                  generator.generatorRandLevel(i_param.getRandLevel().getMaxLevel(),
                                               i_param.getRandLevel().getMaxElements(),
                                               i_param.getInputs(),
                                               i_param.getOutputs()
                                               )
  });

  for (const auto& [name, graph] : circs)
  {
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
  }
}

void DataBaseGenerator::generateDataBaseNumOperations(const GenerationParameters& i_param)
{
  SimpleGenerators generator;
  std::vector<std::pair<std::string, OrientedGraph>> circs;
  circs.push_back({"RandLevel", 
                  generator.generatorNumOperation(
                    i_param.getInputs(),
                    i_param.getOutputs(),
                    i_param.getNumOperations().getLogicOpers(),
                    i_param.getNumOperations().getLeaveEmptyOut()
                  )
  });
  
  for (const auto &[name, graph] : circs)
  {
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
  }
  // TODO: remake all generates to return value and call graphToVerilog
}

void DataBaseGenerator::generateDataBaseGenetic(const GenerationParameters& i_param)
{
  i_param.getGenetic().setInputs(i_param.getInputs());
  i_param.getGenetic().setOutputs(i_param.getOutputs());

  GeneticGenerator<TruthTable, TruthTableParameters> gg(GeneticParameters(i_param.getGenetic()),
                                                         {i_param.getInputs(), i_param.getOutputs()},
                                                         d_mainPath);
  //gg.generate();
}

void DataBaseGenerator::generateDataBaseSummator(const GenerationParameters &i_param) {
    SimpleGenerators generator;
    int i_bits = i_param.getInputs();
    bool i_overflowIn = i_param.getSummator().getOverFlowIn();
    bool i_overflowOut = i_param.getSummator().getOverFlowOut();
    bool i_minus = i_param.getSummator().getMinus();
    OrientedGraph graph = generator.generatorSummator(i_bits, i_overflowIn, i_overflowOut, i_minus);
    Circuit c (graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseComparison(const GenerationParameters &i_param)
{
  SimpleGenerators sg;
  int bits = i_param.getInputs();
  bool compare0 = i_param.getComparison().getCompare0();
  bool compare1 = i_param.getComparison().getCompare1();
  bool compare2 = i_param.getComparison().getCompare2();
  OrientedGraph graph = sg.generatorComparison(bits, compare0, compare1, compare2);
  Circuit c(graph);
  c.setPath(d_mainPath);
  c.setCircuitName(i_param.getName());
  c.generate();
}

void DataBaseGenerator::generateDataBaseEncoder(const GenerationParameters &i_param)
{
    SimpleGenerators sg;
    int i_bits = i_param.getInputs();
    OrientedGraph graph = sg.generatorEncoder(i_bits);
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseMultiplier(const GenerationParameters &i_param) {
    SimpleGenerators generator;
    int i_bits = i_param.getInputs();
    OrientedGraph graph = generator.generatorMultiplier(i_bits);
    Circuit c (graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseMultiplexer(const GenerationParameters &i_param) 
{
    SimpleGenerators generator;
    int i_bits = i_param.getInputs();
    OrientedGraph graph = generator.generatorMultiplexer(i_bits);
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseParity(const GenerationParameters &i_param) 
{
    SimpleGenerators generator;
    int i_bits = i_param.getInputs();
    OrientedGraph graph = generator.generatorParity(i_bits);
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseDecoder(const GenerationParameters &i_param)
{
    SimpleGenerators sg;
    int i_bits = i_param.getInputs();
    OrientedGraph graph = sg.generatorDecoder(i_bits);
    Circuit c(graph);
    c.setPath(d_mainPath);
    c.setCircuitName(i_param.getName());
    c.generate();
}

void DataBaseGenerator::generateDataBaseDemultiplexer(const GenerationParameters &i_param)
{
  SimpleGenerators generator;
  int i_bits = i_param.getOutputs();
  OrientedGraph graph = generator.generatorDemultiplexer(i_bits);
  Circuit c(graph);
  c.setPath(d_mainPath);
  c.setCircuitName(i_param.getName());
  c.generate();
}

std::function<void(const GenerationParameters&)> DataBaseGenerator::getGenerateMethod(const std::string& i_methodName)
{
  if (i_methodName == "FromRandomTruthTable")
    return std::bind(&DataBaseGenerator::generateDataBaseFromRandomTruthTable, this, std::placeholders::_1);
  if (i_methodName == "RandLevel")
    return std::bind(&DataBaseGenerator::generateDataBaseRandLevel, this, std::placeholders::_1);
  if (i_methodName == "NumOperation")
    return std::bind(&DataBaseGenerator::generateDataBaseNumOperations, this, std::placeholders::_1);
  if (i_methodName == "Genetic")
    return std::bind(&DataBaseGenerator::generateDataBaseGenetic, this, std::placeholders::_1);
  if (i_methodName == "Summator")
      return std::bind(&DataBaseGenerator::generateDataBaseSummator, this, std::placeholders::_1);
  if (i_methodName == "Multiplier")
      return std::bind(&DataBaseGenerator::generateDataBaseMultiplier, this, std::placeholders::_1);
  if (i_methodName == "Comparison")
      return std::bind(&DataBaseGenerator::generateDataBaseComparison, this, std::placeholders::_1);
  if (i_methodName == "Encoder")
      return std::bind(&DataBaseGenerator::generateDataBaseEncoder, this, std::placeholders::_1);
  if (i_methodName == "Multiplexer")
      return std::bind(&DataBaseGenerator::generateDataBaseMultiplexer, this, std::placeholders::_1);
  if (i_methodName == "Parity")
      return std::bind(&DataBaseGenerator::generateDataBaseParity, this, std::placeholders::_1);
  if (i_methodName == "Decoder")
      return std::bind(&DataBaseGenerator::generateDataBaseDecoder, this, std::placeholders::_1);
  if (i_methodName == "Demultiplexer")
      return std::bind(&DataBaseGenerator::generateDataBaseDemultiplexer, this, std::placeholders::_1);
  std::cout << "UNDEFINED FUNC << " << i_methodName << std::endl;
  return std::bind(&DataBaseGenerator::generateDataBaseFromRandomTruthTable, this, std::placeholders::_1);

}
