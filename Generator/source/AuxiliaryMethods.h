#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>

#include "./circuits/Circuit.h"

namespace AuxMethods
{
  std::string readAllFile(const std::string& filename);
  std::vector<int> getRandomIntList(int i_n, int i_minNumber, int i_maxNumber, bool repite);
  template<typename Key, typename Value>
  std::vector<std::pair<Key, Value>> sortDictByValue(const std::map<Key, Value>& i_dict, bool up = true)
  {
  std::vector<std::pair<Key, Value>> pairs(i_dict.begin(), i_dict.end());

  // Define a lambda function to compare values
  auto cmp = [](const std::pair<Key, Value>& lhs, const std::pair<Key, Value>& rhs) {
      return lhs.second < rhs.second;
  };

  // Sort the vector of pairs based on the values
  std::sort(pairs.begin(), pairs.end(), cmp);
  if (!up)
    std::reverse(pairs.begin(), pairs.end());

  return pairs;
  };
  //TODO: ToEnum
  //TODO: if need LineReader
  std::string removeSpaces(const std::string& i_s);
  int skipSpaces(const std::string& i_s, int i_start = 0);
  //TODO: if need CopyDirectory
  Circuit parseVerilog(const std::string& i_filepath);
}
// template<typename Key, typename Value>
// std::vector<std::pair<Key, Value>> AuxMethods::sortDictByValue(const std::map<Key, Value>& i_dict, bool up = true)
// {
//   std::vector<std::pair<Key, Value>> pairs(i_dict.begin(), i_dict.end());

//   // Define a lambda function to compare values
//   auto cmp = [](const std::pair<Key, Value>& lhs, const std::pair<Key, Value>& rhs) {
//       return lhs.second < rhs.second;
//   };

//   // Sort the vector of pairs based on the values
//   std::sort(pairs.begin(), pairs.end(), cmp);
//   if (!up)
//     std::reverse(pairs.begin(), pairs.end());

//   return pairs;
// }