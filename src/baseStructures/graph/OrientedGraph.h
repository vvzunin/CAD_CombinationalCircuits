#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "GraphVertex.h"
#include <generators/Genetic/GeneticParameters.h>

class GraphVertex;
class OrientedGraph : protected GraphVertexBase {
public:
  //friend class Circuit;
  OrientedGraph();
  OrientedGraph(int i_inputs, int i_outputs);

  virtual ~OrientedGraph();

  // Количество gate в графе, за исключением подграфов
  int baseSize() const;
  // Количество gate в графе, включая подграфы
  int fullSize() const;
  // Имеются ли gate в схеме, включая подграфы
  bool isEmpty() const;
  // Имеются ли в схеме какие-либо vertex
  bool isEmptyFull() const;

  // Проверяет, связный ли граф
  bool isConnected() const;
  
  virtual void updateLevel();
  int getMaxLevel();

  GraphVertexBase* addInput(const std::string i_name = "");
  GraphVertexBase* addOutput(const VertexTypes i_type);
  GraphVertexBase* addConst(const VertexTypes i_type);
  GraphVertexBase* addGate(const VertexTypes i_type);

  bool addEdge(GraphVertexBase* from, GraphVertexBase* to);
  bool addEdges(std::vector<GraphVertexBase*> from1, GraphVertexBase* to);

  // toVerilog
  // toAdjencyMatrix
  // toGraphML
  // 

private:
  bool d_needLevelUpdate = true;

  std::map<VertexTypes, std::vector<GraphVertexBase*>> d_vertexes{
    {VertexTypes::input, std::vector<GraphVertexBase*>()},
    {VertexTypes::output, std::vector<GraphVertexBase*>()},
    {VertexTypes::constant, std::vector<GraphVertexBase*>()},
    {VertexTypes::graph, std::vector<GraphVertexBase*>()}
  };
  
  std::shared_ptr<Settings> d_settings = Settings::getInstance("OrietedGraph");  
};