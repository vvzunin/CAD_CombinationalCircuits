#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "OrientedGraph.h"
// #include "../Generators/SimpleGenerators.h"

OrientedGraph::OrientedGraph(OrientedGraph &&other) 
{
    d_listOfEdgesFromTo = std::move(other.d_listOfEdgesFromTo);
    d_listOfEdgesToFrom = std::move(other.d_listOfEdgesToFrom);
    d_vertices = std::move(other.d_vertices);

    d_verticesOperations = std::move(other.d_verticesOperations);
    d_verticesWires = std::move(other.d_verticesWires);

    d_inputs = std::move(other.d_inputs);
    d_outputs = std::move(other.d_outputs);
    d_consts = std::move(other.d_consts);

    d_inputs_names = std::move(other.d_inputs_names);
    d_outputs_names = std::move(other.d_outputs_names);

    d_adjacencyMatrix = std::move(other.d_adjacencyMatrix);
    d_error = std::move(other.d_error);

    // Uncomment this code for fullSize tests
    // other.d_listOfEdgesFromTo.clear();
    // other.d_listOfEdgesToFrom.clear();
    // other.d_vertices.clear();

    // other.d_verticesOperations.clear();
    // other.d_verticesWires.clear();

    // other.d_inputs.clear();
    // other.d_outputs.clear();
    // other.d_consts.clear();

    // other.d_inputs_names.clear();
    // other.d_outputs_names.clear();

    // other.d_adjacencyMatrix.clear();
    // other.d_error.clear();

    // other.d_listOfEdgesFromTo.shrink_to_fit();
    // other.d_listOfEdgesToFrom.shrink_to_fit();
    // other.d_vertices.shrink_to_fit();

    // other.d_inputs.shrink_to_fit();
    // other.d_outputs.shrink_to_fit();
    // other.d_consts.shrink_to_fit();

    // other.d_inputs_names.shrink_to_fit();
    // other.d_outputs_names.shrink_to_fit();

    // other.d_adjacencyMatrix.shrink_to_fit();

    d_settings = other.d_settings;
    other.d_settings = nullptr;
}

OrientedGraph &OrientedGraph::operator=(OrientedGraph &&other)
{
    if (this == &other)
        return *this;

    d_listOfEdgesFromTo = std::move(other.d_listOfEdgesFromTo);
    d_listOfEdgesToFrom = std::move(other.d_listOfEdgesToFrom);
    d_vertices = std::move(other.d_vertices);

    d_verticesOperations = std::move(other.d_verticesOperations);
    d_verticesWires = std::move(other.d_verticesWires);

    d_inputs = std::move(other.d_inputs);
    d_outputs = std::move(other.d_outputs);
    d_consts = std::move(other.d_consts);

    d_inputs_names = std::move(other.d_inputs_names);
    d_outputs_names = std::move(other.d_outputs_names);

    d_adjacencyMatrix = std::move(other.d_adjacencyMatrix);
    d_error = std::move(other.d_error);

    d_settings = other.d_settings;
    other.d_settings = nullptr;

    return *this;
}

size_t OrientedGraph::fullSize() {
    size_t graphSize = 0;

    graphSize += d_vertices.capacity();
    graphSize += d_listOfEdgesFromTo.capacity();
    graphSize += d_listOfEdgesToFrom.capacity();

    graphSize += d_verticesOperations.size();
    graphSize += d_verticesWires.size();

    graphSize += d_inputs.capacity();
    graphSize += d_outputs.capacity();
    graphSize += d_consts.capacity();

    graphSize += d_inputs_names.capacity();
    graphSize += d_outputs_names.capacity();

    graphSize += d_adjacencyMatrix.capacity();
    graphSize += d_error.size();

    return graphSize;
}

void OrientedGraph::numberVerticesCorrectly()
{
    int counter = 1;
    for (auto &element : d_vertices)
    {
        if (element.getOperation() != "input" && element.getOperation() != "output" && element.getOperation() != "const")
            element.setWireName("d" + std::to_string(counter++));
    }
}

std::vector<GraphVertex> OrientedGraph::getVertices() const
{
    return d_vertices;
}

const std::vector<GraphVertex> &OrientedGraph::getVerticesReference() const
{
    return d_vertices;
}

size_t OrientedGraph::size() const
{
    return d_vertices.size();
}

bool OrientedGraph::empty() const
{
    return d_vertices.size() == 0;
}

GraphVertex OrientedGraph::getVertice(int i) const
{
    assert(i >= 0 && i < d_vertices.size());
    return d_vertices[i];
}

int OrientedGraph::getMaxLevel()
{
    std::vector<std::string> outputs = getVerticesByType("output");
    int maxLevel = 0;
    for (int i = 0; i < outputs.size(); ++i)
    {
        int n = getIndexOfExpression(outputs[i]); // TODO: What? Next line
        n = d_vertices[n].getLevel();
        maxLevel = n > maxLevel ? n : maxLevel;
    }
    return maxLevel;
}

std::vector<std::vector<bool>> OrientedGraph::getAdjacencyMatrix() const
{
    return d_adjacencyMatrix;
}

const std::vector<std::vector<bool>> &OrientedGraph::getAdjacencyMatrixReference() const
{
    return d_adjacencyMatrix;
}

bool OrientedGraph::getAdjacencyMatrix(int i, int j) const
{
    return d_adjacencyMatrix.at(i).at(j);
}

// void OrientedGraph::generateRandom(OrientedGraphParameters i_gp)
//{
//   std::srand(std::time(0));
//   int inputs;
//   int outputs;
//   int maxLevel;
//   int maxElements;
//   if (!i_gp.empty())
//   {
//     inputs = i_gp.getInputs();
//     outputs = i_gp.getOutputs();
//     maxLevel = i_gp.getMaxLevel();
//     maxElements = i_gp.getMaxElements();
//   }
//   else
//   {
//     inputs = (std::rand() % d_settings.getMaxInputs()) + 1; // in code without +1. It is need? https://github.com/RomeoMe5/CAD_Combinational_Circuits/blob/7ae1fe61eb61e1751147a8598b7a25c976d233b3/Generator/source/Graph/OrientedGraph.cs#L80
//     outputs = (std::rand() % d_settings.getMaxOutputs()) + 1;
//     maxLevel = i_gp.getMaxLevel(); //TODO: WTF?
//     maxElements = i_gp.getMaxElements(); //TODO: WTF2?
//   }
//
//   SimpleGenerators sg;
//   OrientedGraph grd = sg.generatorRandLevel(maxLevel, maxElements, inputs, outputs);
//
//   d_vertices = grd.getVertices();
//   d_adjacencyMatrix = grd.getAdjacencyMatrix();
// }

int OrientedGraph::getIndexOfExpression(const std::string &i_expression)
{
    if (!d_verticesOperations.contains(i_expression))
        return -1;
    return d_verticesOperations[i_expression];

    // TODO: провести поиск не по полномму совпадению, а по перестановкам.
    // for (int i = 0; i < d_vertices.size(); ++i)
    // {
    //     if (d_vertices[i].getLogicExpression() == i_expression)
    //         return i;
    // }

    // return -1;
}

int OrientedGraph::getIndexOfWireName(const std::string &i_wireName)
{
    if (!d_verticesWires.contains(i_wireName))
        return -1;
    return d_verticesWires[i_wireName];

    // for (int i = 0; i < d_vertices.size(); ++i)
    // {
    //     if (d_vertices[i].getWireName() == i_wireName)
    //         return i;
    // }

    // return -1;
}

bool OrientedGraph::addVertex(const std::string &i_vertexName, const std::string &i_operation, const std::string &i_wireName)
{
    if (getIndexOfExpression(i_vertexName) != -1)
        return false;

    if (i_wireName == "")
        d_vertices.push_back(GraphVertex(i_vertexName, i_operation));
    else
        d_vertices.push_back(GraphVertex(i_vertexName, i_operation, false, i_wireName));
    
    d_verticesOperations[d_vertices.back().getLogicExpression()] = d_vertices.size() - 1;
    d_verticesWires[d_vertices.back().getWireName()] = d_vertices.size() - 1;

    if (i_operation == "input")
    {
        d_inputs.push_back(d_vertices.size() - 1);
        d_inputs_names.push_back(d_vertices.back().getLogicExpression());
    }
    else if (i_operation == "output")
    {
        d_outputs.push_back(d_vertices.size() - 1);
        d_outputs_names.push_back(d_vertices.back().getLogicExpression());
    }
    else if (i_operation == "const")
    {
        d_consts.push_back(d_vertices.size() - 1);
    }

    for (int i = 0; i + 1 < d_vertices.size(); ++i)
    {
        d_adjacencyMatrix[i].push_back(false);
    }

    d_listOfEdgesFromTo.push_back({});
    d_listOfEdgesToFrom.push_back({});
    d_adjacencyMatrix.push_back({});
    for (int i = 0; i < d_adjacencyMatrix.size(); ++i)
        d_adjacencyMatrix.back().push_back(false);

    return true;
}

bool OrientedGraph::addEdge(const std::string &i_vertexFrom, const std::string &i_vertexTo, bool i_isExpression)
{
    int v1 = -1;
    int v2 = -1;

    if (i_isExpression)
    {
        v1 = getIndexOfExpression(i_vertexFrom);
        v2 = getIndexOfExpression(i_vertexTo);
    }
    else
    {
        v1 = getIndexOfWireName(i_vertexFrom);
        v2 = getIndexOfWireName(i_vertexTo);
    }

    if (v1 == -1 || v2 == -1)
        return false;

    d_vertices[v2].setLevel(std::max(d_vertices[v1].getLevel() + 1, d_vertices[v2].getLevel())); // TODO: Zunin ask max really and max without prev max, and +1 for v2?
    d_adjacencyMatrix[v1][v2] = true;
    d_listOfEdgesFromTo[v1].push_back(v2);
    d_listOfEdgesToFrom[v2].push_back(v1);
    return true;
}

bool OrientedGraph::addDoubleEdge(
    const std::string &i_vertexFromFirst, 
    const std::string &i_vertexFromSecond, 
    const std::string &i_vertexTo, 
    bool i_isExpression)
{
    int v1 = -1, v2 = -1, v3 = -1;

    if (i_isExpression)
    {
        v1 = getIndexOfExpression(i_vertexFromFirst);
        v2 = getIndexOfExpression(i_vertexFromSecond);
        v3 = getIndexOfExpression(i_vertexTo);
    }
    else
    {
        v1 = getIndexOfWireName(i_vertexFromFirst);
        v2 = getIndexOfWireName(i_vertexFromSecond);
        v3 = getIndexOfWireName(i_vertexTo);
    }

    if (v1 == -1 || v2 == -1 || v3 == -1)
        return false;

    d_vertices[v3].setLevel(std::max(d_vertices[v1].getLevel() + 1, d_vertices[v3].getLevel()));
    d_vertices[v3].setLevel(std::max(d_vertices[v2].getLevel() + 1, d_vertices[v3].getLevel())); // TODO: WTF? Line up why? https://github.com/RomeoMe5/CAD_Combinational_Circuits/blob/7ae1fe61eb61e1751147a8598b7a25c976d233b3/Generator/source/Graph/OrientedGraph.cs#L196
    d_adjacencyMatrix[v1][v3] = true;
    d_adjacencyMatrix[v2][v3] = true;
    d_listOfEdgesFromTo[v1].push_back(v3);
    d_listOfEdgesFromTo[v2].push_back(v3);
    d_listOfEdgesToFrom[v3].push_back(v1);
    d_listOfEdgesToFrom[v3].push_back(v2);
    return true;
}

bool OrientedGraph::addMultipleEdge(
    const std::vector<std::string> &i_vertexFrom, 
    const std::string &i_vertexTo, 
    bool i_isExpression) 
{
    std::vector<int> elements(i_vertexFrom.size(), -1);
    int toElem = -1;

    if (i_isExpression)
    {
        for (int i = 0; i < i_vertexFrom.size(); ++i) {
            elements[i] = getIndexOfExpression(i_vertexFrom[i]);
            if (elements[i] == -1)
                return false;
        }
        
        toElem = getIndexOfExpression(i_vertexTo);
    }
    else
    {
        for (int i = 0; i < i_vertexFrom.size(); ++i) {
            elements[i] = getIndexOfWireName(i_vertexFrom[i]);
            if (elements[i] == -1)
                return false;
        }
        
        toElem = getIndexOfWireName(i_vertexTo);
    }

    if (toElem == -1)
        return false;

    for (int fromElem : elements) {
        d_vertices[toElem].setLevel(std::max(d_vertices[fromElem].getLevel() + 1, d_vertices[toElem].getLevel()));
        d_adjacencyMatrix[fromElem][toElem] = true;

        d_listOfEdgesFromTo[fromElem].push_back(toElem);
        d_listOfEdgesToFrom[toElem].push_back(fromElem);
    }

    return true;
}

void OrientedGraph::printAdjacencyMatrix(bool i_isExpressions)
{
    std::cout << "This feature isn't complete.";
}

std::vector<std::string> OrientedGraph::getVerticesByType(const std::string i_type)
{
    if (i_type == "input")
        return d_inputs_names;
    if (i_type == "output")
        return d_outputs_names;

    std::vector<std::string> names; // TODO: rewrite this func to one line
    for (const auto &vert : d_vertices)
    {
        if (vert.getOperation() == i_type)
            names.push_back(vert.getLogicExpression());
    }

    return names;
}

std::vector<std::string> OrientedGraph::getVerticesByTypeToWireName(const std::string &i_type)
{
    std::vector<std::string> names; // TODO: rewrite this func to one line
    for (const auto &vert : d_vertices)
    {
        if (vert.getOperation() == i_type)
            names.push_back(vert.getWireName());
    }

    return names;
}

std::vector<std::string> OrientedGraph::getLogicVerticesToWireName()
{
    std::vector<std::string> names;
    for (const auto &vert : d_vertices)
    {
        if (vert.getOperation() != "input" && vert.getOperation() != "output" && vert.getOperation() != "const")
            names.push_back(vert.getWireName());
    }

    return names;
}

std::vector<int> OrientedGraph::getVerticesByLevel_2(int i_level)
{
    std::vector<int> vec_index;
    for (int i = 0; i < d_vertices.size(); i++)
    {
        if (d_vertices[i].getLevel() == i_level && d_vertices[i].getOperation() != "output" && d_vertices[i].getOperation() != "input" && d_vertices[i].getOperation() != "const")
        {
            vec_index.push_back(i);
        }
    }
    return vec_index;
}

std::vector<std::string> OrientedGraph::getVerticesByLevel(int i_level)
{
    // TODO: rewrite this func to one line
    std::vector<std::string> names;
    for (const auto &vert : d_vertices)
    {
        if (vert.getLevel() == i_level)
            names.push_back(vert.getLogicExpression());
    }
    return names;
}
/*
std::vector<int> OrientedGraph::getVerticesByLevel_2(int i_level)
{
    //TODO: rewrite this func to one line
    std::vector<std::string> names;
    for (int i = 0; i <  d_vertices.size(); i++)
    {
        if (d_vertices[i].getLevel() == i_level)
            names.push_back(i);
    }
    return names;
}

std::vector<int> OrientedGraph::getVerticesByType_2(int i_level)
{
    //TODO: rewrite this func to one line
    std::vector<std::string> names;
    for (int i = 0; i < d_vertices.size(); i++)
    {
        if (d_vertices[i].getLevel() == i_level)
            names.push_back(i);
    }
    return names;
}
*/

std::vector<int> OrientedGraph::getConnectedTo(int k) const
{
    /*
  std::vector<int> lst; //TODO: very usefull name

  for (int i = 0; i < d_vertices.size(); ++i)
  {
    if (d_adjacencyMatrix[k][i])
      lst.push_back(i);
  }
  */
    return d_listOfEdgesFromTo[k];
}

std::vector<int> OrientedGraph::getConnectedFrom(int k)
{
    /*
  std::vector<int> lst; //TODO: very usefull name

  for (int i = 0; i < d_vertices.size(); ++i)
  {
    if (d_adjacencyMatrix[i][k])
      lst.push_back(i);
  }
  */
    return d_listOfEdgesToFrom[k];
}

void OrientedGraph::updateLevels(bool i_isFull, int i_k) // TODO: maybe we need two different func?
// TODO: this is too slow
{
    if (i_isFull)
    {
        for (auto i : d_inputs)
        {
            d_vertices[i].setLevel(0);
            updateLevels(false, i);
        }
        for (auto i : d_consts)
        {
            d_vertices[i].setLevel(0);
            updateLevels(false, i);
        }
    }
    else
    {
        // std::vector<int> ver = getConnectedTo(i_k);
        //  Stops when we are at output
        for (int j : d_listOfEdgesFromTo[i_k])
        {
            d_vertices[j].setLevel(std::max(d_vertices[j].getLevel(), d_vertices[i_k].getLevel() + 1));
            updateLevels(i_isFull, j); // TODO: wtf - i_isFull is always false
        }
    }
}

std::vector<bool> OrientedGraph::vertsToValues(std::vector<int> i_verts)
{
    std::vector<bool> val;
    for (const auto &i : i_verts)
        val.push_back(d_vertices[i].getValue());
    return val;
}

bool OrientedGraph::operator== (const OrientedGraph& r) const
{
  return std::tie(d_vertices, d_settings, d_adjacencyMatrix)
  == std::tie(r.d_vertices, r.d_settings, r.d_adjacencyMatrix);
}

bool OrientedGraph::calc(std::vector<bool> i_inputs, const std::string &i_op)
{
    bool res = false;

    if (i_inputs.size() == 0)
        return false;

    if (i_op == "not")
    {
        res = !(i_inputs.front());
    }
    else if (i_op == "buf")
    {
        res = i_inputs.front();
    }
    else if (i_op == "and")
    {
        res = true;
        for (int i = 0; i < i_inputs.size(); ++i)
            res &= i_inputs[i]; // TODO: need to check time of first return
    }
    else if (i_op == "nand")
    {
        res = true;
        for (int i = 0; i < i_inputs.size(); ++i)
            res &= i_inputs[i]; // TODO: need to check time of first return
        res = !res;
    }
    else if (i_op == "or")
    {
        res = false;
        for (int i = 0; i < i_inputs.size(); ++i)
            res |= i_inputs[i]; // TODO: need to check time of first return
    }
    else if (i_op == "nor")
    {
        res = false;
        for (int i = 0; i < i_inputs.size(); ++i)
            res |= i_inputs[i]; // TODO: need to check time of first return
        res = !res;
    }
    else if (i_op == "xor")
    {
        res = false;
        for (int i = 0; i < i_inputs.size(); ++i)
            res ^= i_inputs[i]; // TODO: need to check time of first return
    }
    else if (i_op == "xnor")
    {
        res = false;
        for (int i = 0; i < i_inputs.size(); ++i)
            res ^= i_inputs[i]; // TODO: need to check time of first return
        res = !res;
    }
    return res;
}

std::map<std::string, bool> OrientedGraph::calcGraph(
    const std::map<std::string, bool> &i_inputValues,
    bool i_withErrorValues,
    const std::map<std::string, bool> &i_errorValues,
    bool i_withErrorSetting, // really sertting?
    const std::map<std::string, bool> &i_setError)
{
    updateLevels();
    std::map<std::string, bool> dict; // TODO: very usefull name

    if (i_inputValues.size() != d_inputs.size())
        return {};

    if (i_withErrorValues && i_errorValues.empty() && i_errorValues.size() != d_vertices.size() - (d_inputs.size() + d_consts.size() + d_outputs.size()))
        return {};

    if (i_withErrorSetting && !i_setError.empty() && i_setError.size() != d_vertices.size() - (d_inputs.size() + d_consts.size() + d_outputs.size()))
        return {};

    int n = getMaxLevel();
    for (int level = 0; level <= n; ++level) // TODO: Are we really need this fucking spaceship?
    {
        std::vector<std::string> verts = getVerticesByLevel(level);
        for (const auto &v : verts)
        {
            int i = getIndexOfExpression(v);
            if (d_vertices[i].getOperation() == "input")
                d_vertices[i].setValue(i_inputValues.at(d_vertices[i].getWireName()));
            else if (d_vertices[i].getOperation() == "output")
            {
                std::vector<int> from = getConnectedFrom(i);
                if (from.size() > 0)
                    dict[d_vertices[i].getWireName()] = d_vertices[from[0]].getValue(); // TODO: Zunin really from[0]?
            }
            else if (d_vertices[i].getOperation() == "const")
            {
                d_vertices[i].setValue(d_vertices[i].getWireName().find("1'b1") != std::string::npos ? true : false);
            }
            else
            {
                std::vector<int> from = getConnectedFrom(i);
                std::vector<bool> lst;
                if (i_withErrorValues)
                    lst.push_back(i_errorValues.at(d_vertices[i].getWireName()));
                std::vector<bool> inserts = vertsToValues(from);
                std::copy(inserts.begin(), inserts.end(), std::back_inserter(lst));
                d_vertices[i].setValue(calc(lst, d_vertices[i].getOperation()));
                if (i_withErrorSetting)
                    d_vertices[i].setValue(d_vertices[i].getValue() ^ i_setError.at(d_vertices[i].getWireName()));
            }
        }
    }
    return dict;
}

void OrientedGraph::setVerticeOperation(int i_vertice, const std::string &i_operation)
{
    d_vertices[i_vertice].setOperation(i_operation);
}

// TODO: отредактировать подсчет уровня каждой вершины
// TODO: проверка на корректность.
// TODO: оптимизация графа.
