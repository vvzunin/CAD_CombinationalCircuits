#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "GraphVertexBase.hpp"

/// @file GraphVertex.hpp
/// TO DO: Maybe Description some virtual methods for Graph's classes,
/// such as GraphVertexGates

/// class GraphVertexInput Represents a vertex in a directed graph that
/// serves as an input vertex. It inherits from the GraphVertexBase class
/// and extends its functionality to work with input vertices
/// @param i_baseGraph A pointer to the oriented graph to which this vertex
/// belongs. It can be null ptr, which indicates the absence of a base graph
/// @param i_type Vertex type (VertexTypes enumeration). By default, it is
/// set to VertexTypes::input, indicating that this is the input vertex
/// @param i_name The name of the vertex provided during creation. This is
/// a string of type std::string that identifies a vertex
/// */

class GraphVertexInput : public GraphVertexBase {
public:
  GraphVertexInput(
      GraphPtr          i_baseGraph = nullptr,
      const VertexTypes i_type      = VertexTypes::input
  );

  GraphVertexInput(
      const std::string i_name,
      GraphPtr          i_baseGraph = nullptr,
      const VertexTypes i_type      = VertexTypes::input
  );

  /// @brief updateValue A virtual function for updating the vertex value.
  /// The implementation is provided in derived classes
  /// @return the value of the vertex after its update

  virtual char updateValue();

  /// @brief updateLevel It is designed to update the level of the current
  /// vertex in a directed graph based on the levels of its input connections

  virtual void updateLevel();

protected:
  GraphVertexInput(VertexTypes i_type);

private:
};

/// class GraphVertexConstant TO DO: Description
///
/// </summary>

class GraphVertexConstant : public GraphVertexInput {
public:
  GraphVertexConstant(char i_const, GraphPtr i_baseGraph = nullptr);

  GraphVertexConstant(
      char              i_const,
      const std::string i_name,
      GraphPtr          i_baseGraph = nullptr
  );

  /// @brief updateLevel updates the level of the current vertex in the graph
  /// based on the levels of its incoming connections

  virtual void updateLevel();
  std::string  getInstance();

private:
};

/// class GraphVertexSubGraph It is a class, having a subGruph pointer
/// inside. Is used for storing this pointer for providing Grpah
/// connectivity
class GraphVertexSubGraph : public GraphVertexBase {
public:
  GraphVertexSubGraph(GraphPtr i_subGraph, GraphPtr i_baseGraph = nullptr);

  GraphVertexSubGraph(
      GraphPtr           i_subGraph,
      const std::string& i_name,
      GraphPtr           i_baseGraph = nullptr
  );

  char        updateValue() override;

  std::string toVerilog() override;

  /// @brief This method is used as a substructure for
  /// OrientedGraph methods
  /// @param i_path path to future file storing. Do not add submodule here,
  /// folder would be created there
  /// @param i_filename name of file to be created (default is same as grpah
  /// name)
  /// @return pair, first is bool, meaning was file writing successful or not
  /// and second is string, for graph is empty, for subgraph is module instance
  std::pair<bool, std::string>
              toVerilog(std::string i_path, std::string i_filename = "");

  /// @brief This method is used as a substructure for
  /// OrientedGraph methods
  /// @param i_fileStream
  /// @return
  bool        toGraphML(std::ofstream& i_fileStream) const;

  /// @brief This method is used as a substructure for
  /// OrientedGraph methods
  /// @param i_nesting
  /// @return
  std::string toGraphML(int i_nesting = 0) const;

  /// @brief This method is used as a substructure for
  /// OrientedGraph methods
  /// @return
  std::string getInstance() override;

  std::string calculateHash(bool recalculate = false);

  void        setSubGraph(GraphPtr i_subGraph);
  GraphPtr    getSubGraph() const;

private:
  GraphPtr    d_subGraph;
  std::string hashed;
};

/// class GraphVertexOutput It is a vertex of the graph, specially designed for
/// data output. It inherits from the GraphVertexBase class and adds additional
/// functionality related to data output

class GraphVertexOutput : public GraphVertexBase {
public:
  GraphVertexOutput(GraphPtr i_baseGraph = nullptr);

  GraphVertexOutput(const std::string i_name, GraphPtr i_baseGraph = nullptr);

  /// @brief updateValue updates the value of the current vertex of the graph
  /// based on the values of its incoming connections and the type of logical
  /// element (or "gate"). Depending on the type of gate and the values of the
  /// incoming links, the method uses the truth tables to perform the
  /// corresponding logical operation and sets a new value for the current
  /// vertex
  /// @return A char that represents the new value of the current vertex after
  /// updating based on the values of its incoming connections and the type of
  /// logical element (or "gate").

  virtual char updateValue();

  /// @brief updateLevel updates the level of the current vertex in the graph
  /// based on the levels of its incoming connections. If a vertex has incoming
  /// connections, the method determines the highest level among all the
  /// vertices to which it is connected, and sets the level of the current
  /// vertex to one higher than the highest level

  virtual void updateLevel();

private:
};

class GraphVertexGates : public GraphVertexBase {
public:
  GraphVertexGates(Gates i_gate, GraphPtr i_baseGraph = nullptr);

  GraphVertexGates(
      Gates             i_gate,
      const std::string i_name,
      GraphPtr          i_baseGraph = nullptr
  );

  virtual char updateValue();
  std::string  calculateHash(bool recalculate = false);
  std::string  getVerilogString() const;
  Gates        getGate() const;

  std::string  toVerilog();

private:
  Gates       d_gate;
  std::string hashed;
  // Определяем тип вершины: подграф, вход, выход, константа или одна из базовых
  // логических операций.
};