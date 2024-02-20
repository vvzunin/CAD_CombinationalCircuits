#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "GraphVertex.h"
#include <generators/Genetic/GeneticParameters.h>

/// class OrientedGraph
/// 
/// @param d_listOfEdgesFromTo Stores a list of graph edges in the form of a two-dimensional vector, where each element of the vector is a list of vertices from which the edges originate to the corresponding vertex
/// @param d_listOfEdgesToFrom Stores a list of graph edges in the form of a two-dimensional vector, where each element of the vector is a list of vertices that include edges from the corresponding vertex
/// @param d_vertices Stores a list of graph vertices. This vector contains objects of the Graph Vertex class, each of which represents a vertex of a graph with certain properties and characteristics
/// @param d_inputs Stores indexes of vertices that are inputs to the graph. This vector contains the indices of the vertices from the vector d_vertices, which represent the inputs of the graph
/// @param d_outputs Stores indexes of vertices that are outputs for the graph. This vector contains the indices of the vertices from the vector d_vertices, which represent the outputs of the graph
/// @param d_consts Stores the indexes of the vertices, which are constants for the graph. This vector contains the indices of the vertices from the vector d_vertices, which represent constants in the graph
/// @param d_inputs_names Stores the names of the input vertices of the graph. This vector contains the names of the vertices corresponding to the vector d_inputs
/// @param d_outputs_names Stores the names of the output vertices of the graph. This vector contains the names of the vertices corresponding to the vector d_outputs
/// @param d_adjacencyMatrix Stores the adjacency matrix of the graph as a two-dimensional vector of Boolean values. This matrix represents information about the connections between the vertices of the graph: if an edge exists between the vertices, then the corresponding element of the matrix has the value true, otherwise false
/// @param d_error Stores error information in a graph as an associative container std::map. The key is the name of the vertex, and the value is a Boolean value representing the presence or absence of an error in this vertex
/// @param d_settings A pointer to the settings object for the graph. This pointer can be used to get or set graph-related settings
/// 
/// */

class OrientedGraph
{
public:
  friend class Circuit;
  OrientedGraph();
  std::vector<GraphVertex> getVertices() const;
  GraphVertex getVertice(int i) const;
  int size() const;
  bool empty() const;
  int getMaxLevel();
  std::vector<std::vector<bool>> getAdjacencyMatrix() const;
  bool getAdjacencyMatrix(int i, int j) const;
  // void generateRandom(OrientedGraphParameters i_gp);
  int getIndexOfExpression(const std::string &i_expression);
  int getIndexOfWireName(const std::string &i_wireName);
  bool addVertex(const std::string i_vertexName, const std::string &i_operation, const std::string &i_wireName = "");
  bool addEdge(const std::string &i_vertexFrom, const std::string &i_vertexTo, bool i_isExpression = true);
  bool addDoubleEdge(const std::string &i_vertexFromFirst, const std::string &i_vertexFromSecond, const std::string &i_vertexTo, bool i_isExpression = true);
  void printAdjacencyMatrix(bool i_isExpressions = false);              // TODO: rewrite to operator <<
  std::vector<std::string> getVerticesByType(const std::string i_type); // TODO: Why we have two func with equal behavior but different func?
  std::vector<std::string> getVerticesByTypeToWireName(const std::string &i_type);
  std::vector<std::string> getLogicVerticesToWireName();
  std::vector<std::string> getVerticesByLevel(int i_level);
  void updateLevels(bool i_isFull = true, int i_k = 0);
  std::map<std::string, bool> calcGraph(
      const std::map<std::string, bool> &i_inputValues,
      bool i_withErrorValues = false,
      const std::map<std::string, bool> &i_errorValues = {},
      bool i_withErrorSetting = false,
      const std::map<std::string, bool> &i_setError = {});
  std::vector<int> getVerticesByLevel_2(int i_level);
  void setVerticeOperation(int i_vertice, const std::string &i_operation);
  void numberVerticesCorrectly();

  std::vector<int> getVertices(const std::string &str)
  {
    if (str == "input")
      return d_inputs;
    if (str == "output")
      return d_outputs;
    if (str == "const")
      return d_consts;

    std::vector<int> vec_index = {};
    for (int i = 0; i < d_vertices.size(); i++)
    {
      if (d_vertices[i].getOperation() == str)
      {
        vec_index.push_back(i);
      }
    }
    return vec_index;
  }
  std::vector<std::vector<int>> d_listOfEdgesFromTo;
  std::vector<std::vector<int>> d_listOfEdgesToFrom;
  std::vector<GraphVertex> d_vertices;
  std::vector<int> d_inputs;
  std::vector<int> d_outputs;
  std::vector<int> d_consts;

  std::vector<std::string> d_inputs_names;
  std::vector<std::string> d_outputs_names;

private:
  std::vector<int> getConnectedTo(int k) const;
  std::vector<int> getConnectedFrom(int k);
  std::vector<bool> vertsToValues(std::vector<int> i_verts);
  bool calc(std::vector<bool> i_inputs, const std::string &i_op);

  std::vector<std::vector<bool>> d_adjacencyMatrix;

  std::map<std::string, bool> d_error;
  std::shared_ptr<Settings> d_settings = Settings::getInstance("OrietedGraph");
};
