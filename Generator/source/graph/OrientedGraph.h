#pragma once

#include <string>
#include <vector>
#include <map>
#include "GraphVertex.h"
#include "../generators/Genetic/GeneticParameters.h"

class OrientedGraph {
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
  //void generateRandom(OrientedGraphParameters i_gp);
  int getIndexOfExpression(const std::string& i_expression);
  int getIndexOfWireName(const std::string& i_wireName);
  bool addVertex(const std::string i_vertexName, const std::string& i_operation, const std::string& i_wireName = "");
  bool addEdge(const std::string& i_vertexFrom, const std::string& i_vertexTo, bool i_isExpression = true);
  bool addDoubleEdge(const std::string& i_vertexFromFirst, const std::string& i_vertexFromSecond, const std::string& i_vertexTo, bool i_isExpression = true);
  void printAdjacencyMatrix(bool i_isExpressions = false); //TODO: rewrite to operator <<
  std::vector<std::string> getVerticesByType(const std::string i_type); //TODO: Why we have two func with equal behavior but different func?
  std::vector<std::string> getVerticesByTypeToWireName(const std::string& i_type);
  std::vector<std::string> getLogicVerticesToWireName();
  std::vector<std::string> getVerticesByLevel(int i_level);
  
  bool Extend(OrientedGraph og = OrientedGraph());
  bool Delete (const std::string& i_vertex, bool i_isExpression = true);
  bool Substitute(const std::string& vertexNew, const std::string& vertexOld, bool i_isExpression = true);
  
  void updateLevels(bool i_isFull = true, int i_k = 0);
  std::map<std::string, bool> calcGraph(
    const std::map<std::string, bool>& i_inputValues,
    bool i_withErrorValues = false,
    const std::map<std::string, bool>& i_errorValues = {},
    bool i_withErrorSetting = false,
    const std::map<std::string, bool>& i_setError = {}
  );
  std::vector<int> getVerticesByLevel_2(int i_level);
  void setVerticeOperation(int i_vertice, const std::string& i_operation);
  void numberVerticesCorrectly();

  std::vector<int> getVertices(std::string str)
  {
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
  bool operator== (const OrientedGraph& r) const;
private:
  std::vector<int> getConnectedTo(int k) const;
  std::vector<int> getConnectedFrom(int k);
  std::vector<bool> vertsToValues(std::vector<int> i_verts);
  bool calc(std::vector<bool> i_inputs, const std::string& i_op);

  
  std::vector<std::vector<bool>> d_adjacencyMatrix;
  
  std::map<std::string, bool> d_error;
  Settings* d_settings = Settings::getInstance("OrietedGraph");
};
