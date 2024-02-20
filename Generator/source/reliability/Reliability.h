#pragma once

#include <map>
#include <string>
#include <memory>

#include <graph/OrientedGraph.h>


/// class OrientedGraph
///
/// 
/// */

class Reliability
{
public:
    Reliability(const OrientedGraph &i_graph, double i_p = 0.5);
    

    std::map<std::string, std::vector<bool>> calc(
        bool i_withErrorValues = false,
        bool i_withErrorStrings = false,
        int i_withOneValveError = -1);
    double calcReliabilityBase();
    std::map<std::string, double> runNadezhda(const std::string &i_path, const std::string i_circuitName);
    // reliability evaluation method



    int sumErrorBetweenReferenceAndOther(std::map<std::string, std::vector<bool>> i_errorStart, std::map<std::string, std::vector<bool>> i_otherTable);

    double valveRating(); // DOI: 10.18522 / 2311 - 3103 - 2016 - 7 - 149158
    double endToEndMethod();

private:
    OrientedGraph d_graph;
    double d_p;
    std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
};
