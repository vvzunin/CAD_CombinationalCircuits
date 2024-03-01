#pragma once

#include <string>
#include <memory>

#include <settings/Settings.h>

class GraphVertex
{
public:
    GraphVertex(const std::string &i_expr,
                const std::string &i_operation,
                bool i_value = false,
                const std::string &i_wireName = "");
    GraphVertex(const GraphVertex &other) = default;
    GraphVertex &operator=(const GraphVertex &other) = default;
    GraphVertex(GraphVertex &&other) = default;
    GraphVertex &operator=(GraphVertex &&other) = default;

    int getLevel() const;
    void setLevel(int i_level);
    bool getValue() const;
    void setWireName(std::string i_wireName);
    void setValue(bool i_value);
    void setLogicExpression(const std::string &i_logicExpression);
    std::string getLogicExpression() const;
    std::string getOperation() const;
    void setOperation(const std::string &i_operation);
    std::string getWireName() const;
    bool wrongVertex = false;
    bool operator== (const GraphVertex& r) const;

private:
    std::string d_logicExpression = "";
    std::string d_operation = "";
    int d_level = 0;
    bool d_value = false;
    std::string d_wireName = "";
    static int d_count;

    std::shared_ptr<Settings> d_settings = Settings::getInstance("GraphVertex");
};
