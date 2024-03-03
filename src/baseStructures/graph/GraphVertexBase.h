#pragma once

#include <string>
#include <vector>

#include "Settings.h"
#include "OrientedGraph.h"

enum VertexTypes {input, output, constant, gate, graph};

class GraphVertexBase {
public:
  GraphVertexBase(OrientedGraph* const i_baseGraph, const VertexTypes i_type, const std::string i_name);
  GraphVertexBase(OrientedGraph* const i_baseGraph, VertexTypes i_type, const std::string i_name, int i_inputs, int i_outputs);
  virtual ~GraphVertexBase();
  
  // Get для типа вершины
  virtual VertexTypes getType() const final;
  // Get для типа вершины в фомате строки
  virtual std::string getTypeName() const final;

  // Get-Set для имен входов
  void setName(const std::string i_name);
  std::string getName() const;

  // Get для значения вершины
  virtual char getValue() const;
  virtual char updateValue() = 0;

  // Get-Set для уровня
  void setLevel(const unsigned i_name);
  unsigned getLevel() const;

  virtual void updateLevel();

  // Get-Set для базового графа
  void setBaseGraph(OrientedGraph* const i_baseGraph);
  OrientedGraph* setBaseGraph() const;

  // чтобы у OrientedGraph был доступ к переменной, в которой хранятся исходящие edges
  std::vector<GraphVertexBase*>& getOutConnections() const; // Убрать



protected:

  OrientedGraph* d_baseGraph = nullptr;
  
  std::string d_name;
  char d_value;
  unsigned d_level;
  std::vector<GraphVertexBase*> d_inConnections;
  std::vector<GraphVertexBase*> d_outConnections;

  std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertexBase");
private:
  // Определяем тип вершины: подграф, вход, выход, константа или одна из базовых логических операций.
  VertexTypes d_type;    

  static uint_fast64_t d_count;
};