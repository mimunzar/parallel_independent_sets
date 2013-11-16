#pragma once

#include "graphml.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/named_graph.hpp>
#include <vector>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

inline std::vector<int> GetIndependentSet(const std::vector<int>& independent_set)
{
    std::vector<int> reduced;
    for (unsigned v = 0; v < independent_set.size(); v++)
    {
        if (independent_set[v])
        {
            reduced.push_back(v);
        }
    }
    return reduced;
}
