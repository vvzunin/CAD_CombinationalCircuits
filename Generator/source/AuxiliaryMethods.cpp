#include <algorithm>
#include <iostream>
#include <cassert>
#include <string_view>
#include <random>
#include <unordered_set>

#include "AuxiliaryMethods.h"


  int getRandInt(int i_lower, int i_upper)
  {
    assert(i_lower <= i_upper);
    int d = i_upper - i_lower + 1;
    return (rand() % d) + i_lower;
  }

  std::vector<std::string> splitString(const std::string& s, char delimiter)
  {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
  }



std::string AuxMethods::readAllFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
      throw std::runtime_error("Failed to open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::vector<int> AuxMethods::getRandomIntList(int i_n, int i_minNumber,
    int i_maxNumber, bool repite = false)
{
  std::vector<int> randomNumbers;
  std::random_device rd; // Получаем случайное число из аппаратного RNG.
  std::mt19937 gen(rd()); // Инициализируем Mersenne-Twister генератор.
  std::uniform_int_distribution<> distrib(i_minNumber, i_maxNumber - 1);

  if (repite) {
    // Если повторения разрешены, просто генерируем числа.
    for (int i = 0; i < i_n; ++i) {
            randomNumbers.push_back(distrib(gen));
    }
  } 
  else {
    // Если повторения запрещены, используем set для проверки существующих чисел.
    std::unordered_set<int> used;
    while (randomNumbers.size() < static_cast<size_t>(i_n)) {
      int number = distrib(gen);
      // Добавляем только уникальные числа.
      if (used.insert(number).second) {
        randomNumbers.push_back(number);
      }
    }
  }
  return randomNumbers;
}



std::string AuxMethods::removeSpaces(const std::string& i_s)
{
  std::string res = "";
  for (const auto c : i_s)
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
      res += c;

  return res;
}

int AuxMethods::skipSpaces(const std::string& i_s, int i_start)
{
  int res = i_start;
  while (res < i_s.size() && (i_s[res] == ' ' || i_s[res] == '\t' || i_s[res] == '\n' || i_s[res] == '\r'))
    ++res;
  return res;
}

Circuit AuxMethods::parseVerilog(const std::string& i_filepath)
{
  const int MODULE_WORD_SIZE = 7;
  const int INPUT_WORD_SIZE = 6;
  const int OUTPUT_WORD_SIZE = 7;
  const int WIRE_WORD_SIZE = 5;

  Circuit circuit({}, {});
  circuit.setPath(i_filepath);

  std::string verilog_module = readAllFile(i_filepath);

  int start = verilog_module.find("module ");
  // TODO would it work with "module NAME("
  std::string left_brace = " (";
  size_t io_names_start = verilog_module.find_first_of(left_brace, start + MODULE_WORD_SIZE);

  // Protection against missing left brace
  assert(io_names_start != std::string::npos);

  circuit.setCircuitName(
    removeSpaces(
      verilog_module.substr(start + MODULE_WORD_SIZE, io_names_start - start - MODULE_WORD_SIZE)
    )
  );

  start = skipSpaces(verilog_module, io_names_start + 1);
  
  std::vector<std::string> inputs;
  std::string inps = "";
  std::string outs = "";

  if (verilog_module.find("input", start) == start) {
    int i_names_start = verilog_module.find("input ", start) + INPUT_WORD_SIZE;
    int o_names_start = verilog_module.find("output ", i_names_start + 1) + OUTPUT_WORD_SIZE;

    inps = removeSpaces(verilog_module.substr(i_names_start, o_names_start - i_names_start - OUTPUT_WORD_SIZE));
    inps = inps.substr(0, inps.size() - 1);

    outs = removeSpaces(verilog_module.substr(
      o_names_start, verilog_module.find(");", o_names_start + 1) - o_names_start
    ));

    start = verilog_module.find(");", o_names_start + 1);
  }
  else
  {
    start = verilog_module.find(");", start);

    int i_names_start = verilog_module.find("input ", start) + INPUT_WORD_SIZE;
    int i_names_end = verilog_module.find(";", i_names_start);

    int o_names_start = verilog_module.find("output ", start) + OUTPUT_WORD_SIZE;
    int o_names_end = verilog_module.find(";", o_names_start);

    inps = removeSpaces(verilog_module.substr(i_names_start, i_names_end - i_names_start));
    outs = removeSpaces(verilog_module.substr(o_names_start, o_names_end - o_names_start));

    start = std::max(i_names_end, o_names_end);
  }

  for (const auto& input : splitString(inps, ','))
    circuit.addVertex(input, "input");

  for (const auto& output : splitString(outs, ','))
    circuit.addVertex(output, "output");

  {
    int wire_start = verilog_module.find("wire ", start) + WIRE_WORD_SIZE;
    int wire_end = verilog_module.find(";", wire_start);
    //TODO: we really need this? std::string wires = removeSpaces(s.substr(wire_start, wire_end - wire_start));
    start = wire_start + 1;
  }

  {
    verilog_module = verilog_module.substr(start);
    verilog_module = verilog_module.substr(0, verilog_module.find("endmodule"));
    verilog_module = verilog_module.substr(skipSpaces(verilog_module));

    int n = 0;

    while(verilog_module.size() != 0)
    {
      if (!verilog_module.starts_with("assign")) 
      {
        int end = verilog_module.find(";");

        std::string type = verilog_module.substr(0, verilog_module.find_first_of(left_brace));
        std::string wireList = removeSpaces(
          verilog_module.substr(
            verilog_module.find('(') + 1, end - 2 - verilog_module.find('(')
          )
        );
        std::vector<std::string> wires = splitString(wireList, ',');

        int tt = circuit.getIndexOfWireName(wires[0]);
        if (tt != -1 && circuit.getVertice(tt).getOperation() == "output")
        {
          std::string w = "ewr_" + std::to_string(n++);
          circuit.addVertex(w, "none", w);
          circuit.addEdge(w, wires[0]);
          wires[0] = w;
        }

        for (const std::string& wire : wires)
          if (circuit.getIndexOfWireName(wire) == -1)
            circuit.addVertex(wire, "none", wire);


        circuit.setVerticeOperation(circuit.getIndexOfWireName(wires[0]), type);
        if (wires.size() > 3) //TODO: what the magic number?
        {
          std::string prev = wires[1];

          for (int i = 2; i + 1 < wires.size(); ++i)
          {
            std::string wire = "ewr_" + std::to_string(n++);
            circuit.addVertex(wire, type, wire);
            circuit.addDoubleEdge(wires[i], prev, wire);
            prev = wire;
          }
          circuit.addDoubleEdge(wires.back(), prev, wires[0]);
        }
        else
          for (int i = 1; i < wires.size(); ++i)
            circuit.addEdge(wires[i], wires[0]);

        verilog_module.erase(0, end + 1);
      }
      else
      {
        int end = verilog_module.find(";");

        std::string left = removeSpaces(verilog_module.substr(6, verilog_module.find('=') - 6));
        std::string right = removeSpaces(
          verilog_module.substr(verilog_module.find('=') + 1, end - verilog_module.find('=') - 1)
        );
        
        bool f = circuit.addEdge(right, left, false);

        verilog_module.erase(0, end + 1);
      }

      verilog_module = verilog_module.erase(0, skipSpaces(verilog_module));
    }
  }
  return circuit;
}
