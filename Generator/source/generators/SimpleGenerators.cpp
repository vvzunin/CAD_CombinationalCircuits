#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>

#include "SimpleGenerators.h"
#include "../graph/OrientedGraph.h"

namespace {
  int maxValueInMap(const std::map<std::string, int>& i_map)
  {
    if (i_map.size() == 0)
    {
      return -1;
    }
    int res = (*i_map.begin()).second;

    for (const auto &[key, value] : i_map)
    {
      res = std::max(res, value);
    }
    return res;
  }
}

SimpleGenerators::SimpleGenerators()
{
  std::srand(std::time(0));
}

std::vector<std::string> SimpleGenerators::cnfFromTruthTable(
  const TruthTable& i_table, 
  bool i_tp
)
{
  std::vector<std::string> fun;
  std::vector<std::vector<bool>> bin = i_table.convToBinary();

  for (int j = 0; j < i_table.getOutput(); ++j)
  {
    fun.push_back("f" + std::to_string(j) + " = ");
    int mem = 0;
    int tmp = 0;

    for (int i = 0; i < i_table.size(); ++i)
    {
      if (!(i_table.getOutTable(i, j) ^ i_tp))
        mem++;
    }

    if (mem == 0)
    {
      if (i_tp)
        fun[j] += "1'b0";
      else
        fun[j] += "1'b1";
      continue;
    }

    if (mem == i_table.size())
    {
      if (i_tp)
        fun[j] += "1'b1";
      else
        fun[j] += "1'b0";
      continue;
    }

    for (int i = 0; i < mem; ++i)
    {
      fun[j] += '(';
      while ((i_table.getOutTable(tmp, j) ^ i_tp) && tmp < i_table.size())
        tmp++;

      for (int k = 0; k < i_table.getInput(); ++k)
      {
        if (bin[tmp][k] ^ i_tp)
          fun[j] += d_settings->getLogicOperation("not").first + " ";

        fun[j] += 'x';
        fun[j] += std::to_string(k);

        if (k != i_table.getInput() - 1)
          fun[j] += " " + (i_tp ? 
                           d_settings->getLogicOperation("and").first : 
                           d_settings->getLogicOperation("or").first
                        ) + " ";
      }

      fun[j] += ')';

      if (i != mem - 1)
        fun[j] += " " + (i_tp ? 
                       d_settings->getLogicOperation("or").first : 
                       d_settings->getLogicOperation("and").first
                      ) + " ";

      tmp++;
    }
  }

  return fun;
}

OrientedGraph SimpleGenerators::generatorRandLevel(int i_maxLevel, int i_maxElements, int i_inputs, int i_outputs)
{
  int maxLevel;
  if (i_maxLevel != 0) maxLevel = rand() % i_maxLevel + 2; // TODO: Zunin , not +1?
  else maxLevel = 2;
  std::vector<int> elemLevel(maxLevel + 1);
  std::vector<std::string> logOper = d_settings->getLogicOperationsKeys();
  logOper.erase(std::find(logOper.begin(), logOper.end(), "input"));
  logOper.erase(std::find(logOper.begin(), logOper.end(), "output"));
  logOper.erase(std::find(logOper.begin(), logOper.end(), "const"));

  //maxLevel++ // what?
  elemLevel[0] = i_inputs;
  elemLevel[maxLevel] = i_outputs;
  for (int i = 1; i < maxLevel; ++i)
  { 
      if (i_maxElements > 1) elemLevel[i] = (rand() % (i_maxElements - 1)) + 2;
      else elemLevel[i] = 2;
  }

  int choice;
  std::string expr;
  OrientedGraph graph;
  int child1, child2;

  for (int i = 0; i < elemLevel[0]; ++i)
  {
    expr = "x" + std::to_string(i);
    graph.addVertex(expr, "input");
  }

  int currIndex = elemLevel[0];
  int prevIndex = 0;

  for (int i = 1; i < maxLevel; ++i)
  {
    int position = 0;
    for (int j = 0; j < elemLevel[i]; ++j)
    {
      choice = rand() % logOper.size();
      if (logOper[choice] == "not")
      {
        child1 = rand() % currIndex;
        expr = d_settings->fromOperationsToName(logOper[choice]) + " (" +
          graph.getVertice(child1).getLogicExpression() + ")";

        if (graph.addVertex(expr, "not"))
          graph.addEdge(graph.getVertice(child1).getLogicExpression(), graph.getVertice(currIndex + position).getLogicExpression());
        else
          --position;
      }
      else
      {
        child1 = (rand() % (currIndex - prevIndex)) + prevIndex;
        child2 = (rand() % (currIndex - prevIndex)) + prevIndex;

        expr = "(" + graph.getVertice(child2).getLogicExpression() + " )" +
          d_settings->fromOperationsToName(logOper[choice]) + " (" + graph.getVertice(child1).getLogicExpression() + ")";

        if (graph.addVertex(expr, logOper[choice]))
          graph.addDoubleEdge(graph.getVertice(child2).getLogicExpression(),
                              graph.getVertice(child1).getLogicExpression(),
                              graph.getVertice(currIndex + position).getLogicExpression());
        else
          --position;
      }
      ++position;
    }

    prevIndex += currIndex - prevIndex;
    currIndex += position;
  }

  //TODO: fix when elements less than outputs

  for (int i = 0; i < i_outputs; ++i)
  {
    child1 = (rand() % (currIndex - prevIndex)) + prevIndex;
    expr = "f" + std::to_string(i + 1);
    graph.addVertex(expr, "output");
    graph.addEdge(graph.getVertice(child1).getLogicExpression(),
                  graph.getVertice(currIndex + i).getLogicExpression()
    );
  }
  return graph;
}

OrientedGraph SimpleGenerators::generatorNumOperation(
  int i_input,
  int i_output,
  std::map<std::string, int> i_logicOper,
  bool i_leaveEmptyOut
)
{
  int sumOper = 0, maxLvl;
  std::string name;
  std::map<std::string, int> copyLogicOper, levelName;
  std::vector<std::string> nameOut, nameInput;

  for(const auto& elem : i_logicOper)
  {
    std::cout << elem.first << " " << elem.second << "\n";
  }

  copyLogicOper = i_logicOper;
  
  OrientedGraph graph;

  for (int i = 0; i < i_input; ++i)
  {
    name = "x" + std::to_string(i);
    graph.addVertex(name, "input");
    levelName[name] = graph.getVertice(graph.getIndexOfExpression(name)).getLevel();
    if (!i_leaveEmptyOut)
      nameInput.push_back(name);
  }

  for (int i = 0; i < i_output; ++i)
  {
    name = "f" + std::to_string(i);
    graph.addVertex(name, "output");
    nameOut.push_back(name);
  }

  for (const auto &[key, value] : copyLogicOper)
    sumOper += value;

  for (int i = 0; i < sumOper; ++i)
  {
    copyLogicOper = delNull(copyLogicOper); // TODO: optimize
    std::string oper = randomGenerator(copyLogicOper);
    copyLogicOper[oper]--;

    if (oper == "not")
    {
      std::string ver1 = randomGenerator(levelName);
      name = d_settings->getLogicOperation(oper).first + "(" + ver1 + ")";
      if (graph.addVertex(name, oper))
      {
        graph.addEdge(ver1, name);
      }
      else
      {
        copyLogicOper[oper]++;
        sumOper++;
      }
    }
    else
    {
      std::string ver1 = randomGenerator(levelName);
      std::string ver2 = randomGenerator(levelName);
      name = "(" + ver1 + ") " + d_settings->getLogicOperation(oper).first + "(" + ver2 + ")";
      std::string reserveName = "(" + ver2 + ") " + d_settings->getLogicOperation(oper).first + "(" + ver1 + ")";
      if (graph.addVertex(name, oper))
      {
        graph.addDoubleEdge(ver1, ver2, name);
        levelName[name] = graph.getVertice(graph.getIndexOfExpression(name)).getLevel();
      }
      else
      {
        copyLogicOper[oper]++;
        sumOper++;
      }
    }
  }

  while ((nameOut.size() > 0) & ((levelName.size() > 0 || i_leaveEmptyOut == false)))
  {
    if (levelName.size() > 0)
    {
      std::vector<std::string> help;
      maxLvl = maxValueInMap(levelName);
      
      for (const auto &[key, value] : levelName)
      {
        if (value == maxLvl)
          help.push_back(key);
      }

      while (help.size() > 0 && nameOut.size() > 0)
      {
        int R1 = rand() % help.size();
        int R2 = rand() % nameOut.size();
        graph.addEdge(help[R1], nameOut[R2]);
        levelName.erase(help[R1]);
        help.erase(help.begin() + R1);
        nameOut.erase(nameOut.begin() + R2);
      }
    }
    else
    {
      int R1 = rand() % nameInput.size();
      int R2 = rand() % nameOut.size();

      graph.addEdge(nameInput[R1], nameOut[R2]);
      nameOut.erase(nameOut.begin() + R2);
    }
  }
  return graph;
}

std::map<std::string, int> SimpleGenerators::delNull(std::map<std::string, int> i_copyLogicOper)
{
  std::vector<std::string> delList;
  for (const auto &[key, value] : i_copyLogicOper)
    if (value == 0)
      delList.push_back(key);

  for (const auto& op : delList)
    i_copyLogicOper.erase(op);
  return i_copyLogicOper;
}

std::string SimpleGenerators::randomGenerator(const std::map<std::string, int>& i_map)
{
  int i = rand() % i_map.size();

  auto p = i_map.begin();

  while(i--)
    p++;

  return (*p).first;

}

OrientedGraph SimpleGenerators::generatorSummator(int bits, bool overflowIn, bool overflowOut, bool minus, bool act)
{
    OrientedGraph graph;
    if (overflowIn)
        graph.addVertex("p0", "input");
    if (act)
        graph.addVertex("1", "const");
    std::string pi;
    std::string x;
    std::string y;
    std::string z;
    std::string cond = std::string(overflowIn ? "t" : "f") + (overflowOut ? "t" : "f") + (minus ? "t" : "f");
    z = std::string(minus ? "n" : "") + "s" + (!overflowIn && !overflowOut ? "0" : (!overflowIn && overflowOut ? "1" : (overflowIn && !overflowOut ? "2" : "3")));
    for (int i = 0; i < bits; i++){
        std::string S = std::to_string(i);
        x = "suma" + cond + S;
        y = "sumb" + cond + S;
        graph.addVertex(x, "input");
        graph.addVertex(y, "input");
        if (minus)
        {
            graph.addVertex("not (" + x + ")", "not", "na" + S);
            graph.addVertex("not (" + y + ")", "not", "nb" + S);
            graph.addEdge(x, "na" + S, false);
            graph.addEdge(y, "nb" + S, false);
            x = "na" + S;
            y = "nb" + S;
        }
        if (!act)
        {
            graph.addVertex(z + S, "output");
        }

        graph.addVertex("(" + x + " and " + y + ")", "and", "andab" + S);

        std::string NextS = std::to_string(i + 1);
        pi = "p" + S;
        graph.addVertex("(" + x + " and " + pi + ")", "and", "anda" + pi);
        graph.addVertex("(" + y + " and " + pi + ")", "and", "andb" + pi);

        graph.addDoubleEdge(x, y, "andab" + S, false);
        graph.addDoubleEdge(x, pi, "anda" + pi, false);
        graph.addDoubleEdge(y, pi, "andb" + pi, false);

        graph.addVertex("((andab" + S + ")" + " or " + "(anda" + pi + ")" + " or " + "(andb" + pi + "))", "or", "p" + NextS);
        graph.addEdge("andab" + S, "p" + NextS, false);
        graph.addEdge("anda" + pi, "p" + NextS, false);
        graph.addEdge("andb" + pi, "p" + NextS, false);
        if (overflowOut && i + 1 == bits)
        {
            if (act)
            {
                graph.addVertex("(1 and p" + NextS + ")", "and", z + "and1_" + NextS);
                graph.addDoubleEdge("1", "p" + NextS, z + "and1_" + NextS, false);
            }
            else
            {
                graph.addVertex(z + std::to_string(bits), "output");
                graph.addEdge("p" + NextS, z + std::to_string(bits), false);
            }
        }
        graph.addVertex("not (p" + NextS + ")", "not", "np" + NextS);
        graph.addEdge("p" + NextS, "np" + NextS, false);

        graph.addVertex("(" + x + " or " + y + " or " + pi + ")", "or", "abpor" + S);
        graph.addEdge(x, "abpor" + S, false);
        graph.addEdge(y, "abpor" + S, false);
        graph.addEdge(pi, "abpor" + S, false);

        graph.addVertex("(abpor" + S + " and np" + NextS + ")", "and", "andnp" + NextS);
        graph.addDoubleEdge("abpor" + S, "np" + NextS, "andnp" + NextS, false);

        graph.addVertex("(" + x + " and " + y + " and " + pi + ")", "and", "abpand" + S);
        graph.addEdge(x, "abpand" + S, false);
        graph.addEdge(y, "abpand" + S, false);
        graph.addEdge(pi, "abpand" + S, false);

        graph.addVertex("(abpand" + S + " or " + "andnp" + NextS + ")", "or", "pS" + S);
        graph.addDoubleEdge("abpand" + S, "andnp" + NextS, "pS" + S, false);
        if (act)
        {
            graph.addVertex("(1 and pS" + S + ")", "and", z + "and1_" + S);
            graph.addDoubleEdge("1", "pS" + S, z + "and1_" + S, false);
        }
        else
        {
            graph.addEdge("pS" + S, z + S, false);
        }
    }
    return graph;
}

OrientedGraph SimpleGenerators::generatorСomparison(int bits, bool compare0, bool compare1, bool compare2, bool act)
{
    OrientedGraph graph;

    std::string cond = std::string(compare0 ? "t" : "f") + (compare1 ? "t" : "f") + (compare2 ? "t" : "f");
    for (int i = bits - 1; i >= 0; i--)
    {
        std::string C = std::to_string(i);
        std::string NextC = std::to_string(i - 1);
        std::string x = "coma" + cond + C;
        std::string y = "comb" + cond + C;
        if (i == 0)
        {
            NextC = "X";
        }
        graph.addVertex(x, "input");
        graph.addVertex(y, "input");
        graph.addVertex("not (" + y + C + ")", "not", "nb" + C);
        graph.addEdge(y, "nb" + C, false);
        graph.addVertex("not (" + x + C + ")", "not", "na" + C);
        graph.addEdge(x, "na" + C, false);
        if (act)
        {
            graph.addVertex("1", "const");
        }
        if (compare0)
        {
            if (!act)
            {
                graph.addVertex("E0_" + C, "output");
            }
            graph.addVertex("(na" + C + " and nb" + C + ")", "and", "nab" + C);
            graph.addVertex("(" + x + C + " and " + y + C + ")", "and", "ab" + C);
            graph.addVertex("(nab" + C + " or ab" + C + ")", "or", "p0_" + NextC);
            graph.addDoubleEdge("na" + C, "nb" + C, "nab" + C, false);
            graph.addDoubleEdge(x, y, "ab" + C, false);
            graph.addDoubleEdge("nab" + C, "ab" + C, "p0_" + NextC, false);

            if (i == bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p0_" + NextC + ")", "and", "E0and1_" + C);
                    graph.addDoubleEdge("1", "p0_" + NextC, "E0and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p0_" + NextC, "E0_" + C, false);
                }
            }
            else
            {
                graph.addVertex("(p0_" + C + " and p0_" + NextC + ")", "and", "pE0_" + C);
                graph.addDoubleEdge("p0_" + C, "p0_" + NextC, "pE0_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE0_" + C + ")", "and", "E0and1_" + C);
                    graph.addDoubleEdge("1", "pE0_" + C, "E0and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE0_" + C, "E0_" + C, false);
                }
            }
        }
        if (compare1)
        {
            if (act)
            {
                graph.addVertex("1", "const");
            }
            else
            {
                graph.addVertex("E1_" + C, "output");
            }

            graph.addVertex("(" + x + C + " and " + "(not (" + y + C + "))", "and", "p1_" + NextC);
            graph.addDoubleEdge(x, "nb" + C, "p1_" + NextC, false);

            if (i == bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p1_" + NextC + ")", "and", "E1and1_" + C);
                    graph.addDoubleEdge("1", "p1_" + NextC, "E1and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p1_" + NextC, "E1_" + C, false);
                }
            }
            else
            {
                graph.addVertex("not (p1_" + C + ")", "not", "np1_" + C);
                graph.addVertex("not (p1_" + NextC + ")", "not", "np1_" + NextC);
                graph.addEdge("p1_" + C, "np1_" + C, false);
                graph.addEdge("p1_" + NextC, "np1_" + NextC, false);
                graph.addVertex("(np1_" + C + " and p1_" + NextC + ")", "and", "P11_" + C);
                graph.addDoubleEdge("np1_" + C, "p1_" + NextC, "P11_", false);
                graph.addVertex("(p1_" + C + " and np1_" + NextC + ")", "and", "P12_" + C);
                graph.addDoubleEdge("p1_" + C, "np1_" + NextC, "P12_", false);
                graph.addVertex("(P11_" + C + " or P12_" + C + ")", "or", "pE1_" + C);
                graph.addDoubleEdge("P11_" + C, "P12_" + C, "pE1_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE1_" + C + ")", "and", "E1and1_" + C);
                    graph.addDoubleEdge("1", "pE1_" + C, "E1and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE1_" + C, "E1_" + C, false);
                }
            }
        }
        if (compare2)
        {
            if (act)
            {
                graph.addVertex("1", "const");
            }
            else
            {
                graph.addVertex("E2_" + C, "output");
            }
            graph.addVertex("(" + y + C + " and " + "(not (" + x + C + "))", "and", "p2_" + NextC);
            graph.addDoubleEdge(y, "na" + C, "p2_" + NextC, false);

            if (i == bits - 1)
            {
                if (act)
                {
                    graph.addVertex("(1 and p2_" + NextC + ")", "and", "E2and1_" + C);
                    graph.addDoubleEdge("1", "p2_" + NextC, "E2and1_" + C, false);
                }
                else
                {
                    graph.addEdge("p2_" + NextC, "E2_" + C, false);
                }
            }
            else
            {
                graph.addVertex("not (p2_" + C + ")", "not", "np2_" + C);
                graph.addVertex("not (p2_" + NextC + ")", "not", "np2_" + NextC);
                graph.addEdge("p2_" + C, "np2_" + C, false);
                graph.addEdge("p2_" + NextC, "np2_" + NextC, false);
                graph.addVertex("(np2_" + C + " and p2_" + NextC + ")", "and", "P21_" + C);
                graph.addDoubleEdge("np2_" + C, "p2_" + NextC, "P21_", false);
                graph.addVertex("(p2_" + C + " and np2_" + NextC + ")", "and", "P22_" + C);
                graph.addDoubleEdge("p2_" + C, "np2_" + NextC, "P22_", false);
                graph.addVertex("(P21_" + C + " or P22_" + C + ")", "or", "pE2_" + C);
                graph.addDoubleEdge("P21_" + C, "P22_" + C, "pE2_" + C, false);
                if (act)
                {
                    graph.addVertex("(1 and pE2_" + C + ")", "and", "E2and1_" + C);
                    graph.addDoubleEdge("1", "pE2_" + C, "E2and1_" + C, false);
                }
                else
                {
                    graph.addEdge("pE2_" + C, "E2_" + C, false);
                }
            }
        }
    }
    return graph;
}

 OrientedGraph SimpleGenerators::generatorEncoder(int bits)
{
    OrientedGraph graph ;
    int k = 0;
    for (int t = 0; t <= bits; t++)
        if (bits - 1 >= pow(2, t))
        {
            k = k + 1;
        }
    for (int l = 0; l <= bits - 1; l++)
    {
        std::string Z = std::to_string(l);
        graph.addVertex("x" + Z, "input");
    }
    if (bits > 1)
        for (int p = k - 1; p >= 0; p--)
        {
            std::string L = "";
            std::string P = "";
            std::string M = "";
            std::string K = "";
            std::string S = std::to_string(p);
            graph.addVertex("a" + S, "output");

            for (int i = 0; i <= bits - 1; i++)
                for (double t = pow(2, p); t <= pow(2, p + 1) - 1; t++)
                    if (pow(2, p + 1) * i + t <= bits - 1)
                    {
                        std::string R = std::to_string(pow(2, p + 1) * i + t);
                        K = M + " or x" + R;
                        L = P + "orx" + R;
                        //graph.addEdge("x" + R, "a" + S, false);
                        P = L;
                        L = "";
                        M = K;
                        K = "";
                    }
            M = M.erase(0, 3);
            graph.addVertex(M, "or", P);
            for (int i = 0; i <= bits - 1; i++)
                for (double t = pow(2, p); t <= pow(2, p + 1) - 1; t++)
                    if (pow(2, p + 1) * i + t <= bits - 1)
                    {
                        std::string R = std::to_string(pow(2, p + 1) * i + t);
                        graph.addEdge("x" + R, P, false);
                    }
            graph.addEdge(P, "a" + S, false);
        }
    else
       std::cout << "Недостаточно входных сигналов\n";

    return graph;
}